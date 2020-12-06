#include <vulkan/vulkan.h>
#include "MatrixState3D.h"
#include "MatrixState2D.h"
#include "MyVulkanManager.h"
#include "../util/FileUtil.h"
#include "../util/TextureManager.h"
#include "../util/HelpFunction.h"
#include <thread>
#include <iostream>
#include <assert.h>
#include <chrono>
#include "ThreadTask.h"
#include "../util/LoadUtil.h"
#include "../util/CameraUtil.h"
#include "../util/LightManager.h"
#include "../util/FPSUtil.h"
#include <sys/time.h>
android_app* MyVulkanManager::Android_application;
bool MyVulkanManager::loopDrawFlag=true;
std::vector<const char *>  MyVulkanManager::instanceExtensionNames;
VkInstance MyVulkanManager::instance;
uint32_t MyVulkanManager::gpuCount;
std::vector<VkPhysicalDevice> MyVulkanManager::gpus;
uint32_t MyVulkanManager::queueFamilyCount;
std::vector<VkQueueFamilyProperties> MyVulkanManager::queueFamilyprops;
uint32_t MyVulkanManager::queueGraphicsFamilyIndex;
VkQueue MyVulkanManager::queueGraphics;
uint32_t MyVulkanManager::queuePresentFamilyIndex;
std::vector<const char *> MyVulkanManager::deviceExtensionNames;
VkDevice MyVulkanManager::device;
VkCommandPool MyVulkanManager::cmdPool;
VkCommandBuffer MyVulkanManager::cmdBuffer;
VkCommandBufferBeginInfo MyVulkanManager::cmd_buf_info;
VkCommandBuffer  MyVulkanManager::cmd_bufs[1];
VkSubmitInfo MyVulkanManager::submit_info[1];
uint32_t MyVulkanManager::screenWidth;
uint32_t MyVulkanManager::screenHeight;
float  MyVulkanManager::ratio;
VkSurfaceKHR MyVulkanManager::surface;
std::vector<VkFormat> MyVulkanManager::formats;
VkSurfaceCapabilitiesKHR MyVulkanManager::surfCapabilities;
uint32_t MyVulkanManager::presentModeCount;
std::vector<VkPresentModeKHR> MyVulkanManager::presentModes;
VkExtent2D MyVulkanManager::swapchainExtent;
VkSwapchainKHR MyVulkanManager::swapChain;
uint32_t MyVulkanManager::swapchainImageCount;
std::vector<VkImage> MyVulkanManager::swapchainImages;
std::vector<VkImageView> MyVulkanManager::swapchainImageViews;
VkFormat MyVulkanManager::depthFormat;
VkFormatProperties MyVulkanManager::depthFormatProps;
VkImage MyVulkanManager::depthImage;
VkPhysicalDeviceMemoryProperties MyVulkanManager::memoryroperties;
VkDeviceMemory MyVulkanManager::memDepth;
VkImageView MyVulkanManager::depthImageView;
VkSemaphore MyVulkanManager::imageAcquiredSemaphore;
uint32_t MyVulkanManager::currentBuffer;
VkRenderPass MyVulkanManager::renderPass;
VkClearValue MyVulkanManager::clear_values[5];
VkRenderPassBeginInfo MyVulkanManager::rp_begin;
VkFence MyVulkanManager::taskFinishFence;
VkPresentInfoKHR MyVulkanManager::present;
VkImage MyVulkanManager::colorImage[SELF_COLOR_COUNT];
VkDeviceMemory MyVulkanManager::memColor[SELF_COLOR_COUNT];
VkImageView MyVulkanManager::colorImageView[SELF_COLOR_COUNT];
VkDescriptorImageInfo MyVulkanManager::colorImageInfo[SELF_COLOR_COUNT];
VkFramebuffer*  MyVulkanManager::framebuffers;
ShaderQueueSuit_PreProcess* MyVulkanManager::sqsCTL;
ShaderQueueSuit_Light* MyVulkanManager::sqsL;
ObjObject*  MyVulkanManager::CHForDraw;
ObjObject*  MyVulkanManager::carForDraw;
ObjObject*  MyVulkanManager::treeForDraw;
ObjObject*  MyVulkanManager::floorForDraw;
void MyVulkanManager::init_vulkan_instance()
{
    AAssetManager* aam=MyVulkanManager::Android_application->activity->assetManager;
    FileUtil::setAAssetManager(aam);
    LoadUtil::setAAssetManager(aam);
    if (!vk::loadVulkan())
    {
        LOGI("加载Vulkan API失败!");
        return ;
    }
    instanceExtensionNames.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
    instanceExtensionNames.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pNext = NULL;
    app_info.pApplicationName = "HelloVulkan";
    app_info.applicationVersion = 1;
    app_info.pEngineName = "HelloVulkan";
    app_info.engineVersion = 1;
    app_info.apiVersion = VK_API_VERSION_1_0;
    VkInstanceCreateInfo inst_info = {};
    inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    inst_info.pNext = NULL;
    inst_info.flags = 0;
    inst_info.pApplicationInfo = &app_info;
    inst_info.enabledExtensionCount = instanceExtensionNames.size();
    inst_info.ppEnabledExtensionNames = instanceExtensionNames.data();;
    inst_info.enabledLayerCount = 0;
    inst_info.ppEnabledLayerNames = NULL;
    VkResult result;
    result = vk::vkCreateInstance(&inst_info, NULL, &instance);
    if(result== VK_SUCCESS)
    {
        LOGE("Vulkan实例创建成功!");
    }
    else
    {
        LOGE("Vulkan实例创建失败!");
    }
}
void MyVulkanManager::destroy_vulkan_instance()
{
    vk::vkDestroyInstance(instance, NULL);
    LOGE("Vulkan实例销毁完毕!");
}
void MyVulkanManager::enumerate_vulkan_phy_devices()
{
    gpuCount=0;
    VkResult result = vk::vkEnumeratePhysicalDevices(instance, &gpuCount, NULL);
    assert(result==VK_SUCCESS);
    LOGE("[Vulkan硬件设备数量为%d个]",gpuCount);
    gpus.resize(gpuCount);
    result = vk::vkEnumeratePhysicalDevices(instance, &gpuCount, gpus.data());
    assert(result==VK_SUCCESS);
    vk::vkGetPhysicalDeviceMemoryProperties(gpus[0],&memoryroperties);
}
void MyVulkanManager::create_vulkan_devices()
{
    vk::vkGetPhysicalDeviceQueueFamilyProperties(gpus[0], &queueFamilyCount, NULL);
    LOGE("[Vulkan硬件设备0支持的队列家族数量为%d]",queueFamilyCount);
    queueFamilyprops.resize(queueFamilyCount);
    vk::vkGetPhysicalDeviceQueueFamilyProperties(gpus[0], &queueFamilyCount, queueFamilyprops.data());
    LOGE("[成功获取Vulkan硬件设备0支持的队列家族属性列表]");
    VkDeviceQueueCreateInfo queueInfo = {};
    bool found = false;
    for (unsigned int i = 0; i < queueFamilyCount; i++)
    {
        if (queueFamilyprops[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            queueInfo.queueFamilyIndex = i;
            queueGraphicsFamilyIndex=i;
            LOGE("[支持GRAPHICS工作的一个队列家族的索引为%d]",i);
            LOGE("[此家族中的实际队列数量是%d]",queueFamilyprops[i].queueCount);
            found = true;
            break;
        }
    }
    float queue_priorities[1] = {0.0};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.pNext = NULL;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = queue_priorities;
    queueInfo.queueFamilyIndex = queueGraphicsFamilyIndex;
    deviceExtensionNames.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    VkDeviceCreateInfo deviceInfo = {};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.pNext = NULL;
    deviceInfo.queueCreateInfoCount = 1;
    deviceInfo.pQueueCreateInfos = &queueInfo;
    deviceInfo.enabledExtensionCount = deviceExtensionNames.size();
    deviceInfo.ppEnabledExtensionNames = deviceExtensionNames.data();
    deviceInfo.enabledLayerCount = 0;
    deviceInfo.ppEnabledLayerNames = NULL;
    deviceInfo.pEnabledFeatures = NULL;
    VkResult result = vk::vkCreateDevice(gpus[0], &deviceInfo, NULL, &device);
    assert(result==VK_SUCCESS);
}
void MyVulkanManager::destroy_vulkan_devices()
{
    vk::vkDestroyDevice(device, NULL);
    LOGE("逻辑设备销毁完毕！");
}
void MyVulkanManager::create_vulkan_CommandBuffer()
{
    VkCommandPoolCreateInfo cmd_pool_info = {};
    cmd_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmd_pool_info.pNext = NULL;
    cmd_pool_info.queueFamilyIndex = queueGraphicsFamilyIndex;
    cmd_pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    VkResult result = vk::vkCreateCommandPool(device, &cmd_pool_info, NULL, &cmdPool);
    assert(result==VK_SUCCESS);
    VkCommandBufferAllocateInfo cmdBAI = {};
    cmdBAI.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdBAI.pNext = NULL;
    cmdBAI.commandPool = cmdPool;
    cmdBAI.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmdBAI.commandBufferCount = 1;
    result = vk::vkAllocateCommandBuffers(device, &cmdBAI, &cmdBuffer);
    assert(result==VK_SUCCESS);
    cmd_buf_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmd_buf_info.pNext = NULL;
    cmd_buf_info.flags = 0;
    cmd_buf_info.pInheritanceInfo = NULL;
    cmd_bufs[0] = cmdBuffer;
    VkPipelineStageFlags* pipe_stage_flags = new VkPipelineStageFlags();
    *pipe_stage_flags=VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    submit_info[0].pNext = NULL;
    submit_info[0].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info[0].pWaitDstStageMask = pipe_stage_flags;
    submit_info[0].commandBufferCount = 1;
    submit_info[0].pCommandBuffers = cmd_bufs;
    submit_info[0].signalSemaphoreCount = 0;
    submit_info[0].pSignalSemaphores = NULL;
}
void MyVulkanManager::destroy_vulkan_CommandBuffer()
{
    VkCommandBuffer cmdBufferArray[1] = {cmdBuffer};
    vk::vkFreeCommandBuffers
    (
         device, 
         cmdPool,
         1,      
         cmdBufferArray
    );
    vk::vkDestroyCommandPool(device, cmdPool, NULL);
}
void MyVulkanManager::create_vulkan_swapChain()
{
    screenWidth = ANativeWindow_getWidth(Android_application->window);
    screenHeight = ANativeWindow_getHeight(Android_application->window);
    LOGE("窗体宽度%d 窗体高度%d",screenWidth,screenHeight);
    VkAndroidSurfaceCreateInfoKHR createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.window = Android_application->window;
    PFN_vkCreateAndroidSurfaceKHR fpCreateAndroidSurfaceKHR=(PFN_vkCreateAndroidSurfaceKHR)vk::vkGetInstanceProcAddr(instance, "vkCreateAndroidSurfaceKHR");
    if (fpCreateAndroidSurfaceKHR == NULL)
    {
            LOGE( "找不到vkvkCreateAndroidSurfaceKHR扩展函数！" );
    }
    VkResult result = fpCreateAndroidSurfaceKHR(instance, &createInfo, nullptr, &surface);
    assert(result==VK_SUCCESS);
    VkBool32 *pSupportsPresent = (VkBool32 *)malloc(queueFamilyCount * sizeof(VkBool32));
    for (uint32_t i = 0; i < queueFamilyCount; i++)
    {
        vk::vkGetPhysicalDeviceSurfaceSupportKHR(gpus[0], i, surface, &pSupportsPresent[i]);
        LOGE("队列家族索引=%d %s显示",i,(pSupportsPresent[i]==1?"支持":"不支持"));
    }
    queueGraphicsFamilyIndex = UINT32_MAX;
    queuePresentFamilyIndex = UINT32_MAX;
    for (uint32_t i = 0; i <queueFamilyCount; ++i)
    {
        if ((queueFamilyprops[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
        {
            if (queueGraphicsFamilyIndex== UINT32_MAX) queueGraphicsFamilyIndex = i;
            if (pSupportsPresent[i] == VK_TRUE)
            {
                queueGraphicsFamilyIndex = i;
                queuePresentFamilyIndex = i;
                LOGE("队列家族索引=%d 同时支持Graphis（图形）和Present（显示）工作",i);
                break;
            }
        }
    }
    if (queuePresentFamilyIndex == UINT32_MAX)
    {
        for (size_t i = 0; i < queueFamilyCount; ++i)
        {
            if (pSupportsPresent[i] == VK_TRUE)
            {
                queuePresentFamilyIndex= i;
                break;
            }
        }
    }
    free(pSupportsPresent);
    if (queueGraphicsFamilyIndex == UINT32_MAX || queuePresentFamilyIndex == UINT32_MAX)
    {
        LOGE("没有找到支持Graphis（图形）或Present（显示）工作的队列家族");
        assert(false);
    }
    uint32_t formatCount;
    result = vk::vkGetPhysicalDeviceSurfaceFormatsKHR(gpus[0], surface, &formatCount, NULL);
    LOGE("支持的格式数量为 %d",formatCount);
    VkSurfaceFormatKHR *surfFormats = (VkSurfaceFormatKHR *)malloc(formatCount * sizeof(VkSurfaceFormatKHR));
    formats.resize(formatCount);
    result = vk::vkGetPhysicalDeviceSurfaceFormatsKHR(gpus[0], surface, &formatCount, surfFormats);
    for(int i=0;i<formatCount;i++)
    {
        formats[i]=surfFormats[i].format;
        LOGE("[%d]支持的格式为%d",i,formats[i]);
    }
    if (formatCount == 1 && surfFormats[0].format == VK_FORMAT_UNDEFINED)
    {
        formats[0] = VK_FORMAT_B8G8R8A8_UNORM;
    }
    free(surfFormats);
    result = vk::vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpus[0], surface, &surfCapabilities);
    assert(result == VK_SUCCESS);
    result = vk::vkGetPhysicalDeviceSurfacePresentModesKHR(gpus[0], surface, &presentModeCount, NULL);
    assert(result == VK_SUCCESS);
    LOGE("显示模式数量为%d",presentModeCount);
    presentModes.resize(presentModeCount);
    result = vk::vkGetPhysicalDeviceSurfacePresentModesKHR(gpus[0], surface, &presentModeCount, presentModes.data());
    for(int i=0;i<presentModeCount;i++)
    {
        LOGE("显示模式[%d]编号为%d",i,presentModes[i]);
    }
    VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
    for (size_t i = 0; i < presentModeCount; i++)
    {
        if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
            break;
        }
        if ((swapchainPresentMode != VK_PRESENT_MODE_MAILBOX_KHR)&&(presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR))
        {
            swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
        }
    }
    if (surfCapabilities.currentExtent.width == 0xFFFFFFFF)
    {
        swapchainExtent.width = screenWidth;
        swapchainExtent.height = screenHeight;
        if (swapchainExtent.width < surfCapabilities.minImageExtent.width)
        {
            swapchainExtent.width = surfCapabilities.minImageExtent.width;
        }
        else if (swapchainExtent.width > surfCapabilities.maxImageExtent.width)
        {
            swapchainExtent.width = surfCapabilities.maxImageExtent.width;
        }
        if (swapchainExtent.height < surfCapabilities.minImageExtent.height)
        {
            swapchainExtent.height = surfCapabilities.minImageExtent.height;
        } else if (swapchainExtent.height > surfCapabilities.maxImageExtent.height)
        {
            swapchainExtent.height = surfCapabilities.maxImageExtent.height;
        }
        LOGE("使用自己设置的 宽度 %d 高度 %d",swapchainExtent.width,swapchainExtent.height);
    }
    else
    {
        swapchainExtent = surfCapabilities.currentExtent;
        LOGE("使用获取的surface能力中的 宽度 %d 高度 %d",swapchainExtent.width,swapchainExtent.height);
    }
    screenWidth=swapchainExtent.width;
    screenHeight=swapchainExtent.height;
    uint32_t desiredMinNumberOfSwapChainImages = surfCapabilities.minImageCount+1;
    if ((surfCapabilities.maxImageCount > 0) &&(desiredMinNumberOfSwapChainImages > surfCapabilities.maxImageCount))
    {
        desiredMinNumberOfSwapChainImages = surfCapabilities.maxImageCount;
    }
    VkSurfaceTransformFlagBitsKHR preTransform;
    if (surfCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
    {
        preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    }
    else
    {
        preTransform = surfCapabilities.currentTransform;
    }
    VkSwapchainCreateInfoKHR swapchain_ci = {};
    swapchain_ci.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_ci.pNext = NULL;
    swapchain_ci.surface = surface;
    swapchain_ci.minImageCount = desiredMinNumberOfSwapChainImages;
    swapchain_ci.imageFormat = formats[0];
    swapchain_ci.imageExtent.width = swapchainExtent.width;
    swapchain_ci.imageExtent.height = swapchainExtent.height;
    swapchain_ci.preTransform = preTransform;
    swapchain_ci.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchain_ci.imageArrayLayers = 1;
    swapchain_ci.presentMode = swapchainPresentMode;
    swapchain_ci.oldSwapchain = VK_NULL_HANDLE;
    swapchain_ci.clipped = true;
    swapchain_ci.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    swapchain_ci.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchain_ci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchain_ci.queueFamilyIndexCount = 0;
    swapchain_ci.pQueueFamilyIndices = NULL;
    if (queueGraphicsFamilyIndex != queuePresentFamilyIndex)
    {
        swapchain_ci.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchain_ci.queueFamilyIndexCount = 2;
        uint32_t queueFamilyIndices[2] = {queueGraphicsFamilyIndex,queuePresentFamilyIndex};
        swapchain_ci.pQueueFamilyIndices = queueFamilyIndices;
    }
    result = vk::vkCreateSwapchainKHR(device, &swapchain_ci, NULL, &swapChain);
    assert(result == VK_SUCCESS);
    result = vk::vkGetSwapchainImagesKHR(device, swapChain, &swapchainImageCount, NULL);
    assert(result == VK_SUCCESS);
    LOGE("[SwapChain中的Image数量为%d]",swapchainImageCount);
    swapchainImages.resize(swapchainImageCount);
    result = vk::vkGetSwapchainImagesKHR(device, swapChain, &swapchainImageCount, swapchainImages.data());
    assert(result == VK_SUCCESS);
    swapchainImageViews.resize(swapchainImageCount);
    for (uint32_t i = 0; i < swapchainImageCount; i++)
    {
        VkImageViewCreateInfo color_image_view = {};
        color_image_view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        color_image_view.pNext = NULL;
        color_image_view.flags = 0;
        color_image_view.image = swapchainImages[i];
        color_image_view.viewType = VK_IMAGE_VIEW_TYPE_2D;
        color_image_view.format = formats[0];
        color_image_view.components.r = VK_COMPONENT_SWIZZLE_R;
        color_image_view.components.g = VK_COMPONENT_SWIZZLE_G;
        color_image_view.components.b = VK_COMPONENT_SWIZZLE_B;
        color_image_view.components.a = VK_COMPONENT_SWIZZLE_A;
        color_image_view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        color_image_view.subresourceRange.baseMipLevel = 0;
        color_image_view.subresourceRange.levelCount = 1;
        color_image_view.subresourceRange.baseArrayLayer = 0;
        color_image_view.subresourceRange.layerCount = 1;
        result = vk::vkCreateImageView(device, &color_image_view, NULL, &swapchainImageViews[i]);
        assert(result == VK_SUCCESS);
    }
}
void MyVulkanManager::destroy_vulkan_swapChain()
{
    for (uint32_t i = 0; i < swapchainImageCount; i++)
    {
        vk::vkDestroyImageView(device, swapchainImageViews[i], NULL);
        LOGE("[销毁SwapChain ImageView %d 成功]",i);
    }
    vk::vkDestroySwapchainKHR(device, swapChain, NULL);
    LOGE("销毁SwapChain成功!");
}
void MyVulkanManager::create_vulkan_DepthBuffer()
{
    depthFormat = VK_FORMAT_D16_UNORM;
    VkImageCreateInfo image_info = {};
    vk::vkGetPhysicalDeviceFormatProperties(gpus[0], depthFormat, &depthFormatProps);
    if (depthFormatProps.linearTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
    {
        image_info.tiling = VK_IMAGE_TILING_LINEAR;
        LOGE("tiling为VK_IMAGE_TILING_LINEAR！");
    }
    else if (depthFormatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
    {
        image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
        LOGE("tiling为VK_IMAGE_TILING_OPTIMAL！");
    }
    else
    {
        LOGE("不支持VK_FORMAT_D16_UNORM！");
    }
    image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_info.pNext = NULL;
    image_info.imageType = VK_IMAGE_TYPE_2D;
    image_info.format = depthFormat;
    image_info.extent.width = screenWidth;
    image_info.extent.height =screenHeight;
    image_info.extent.depth = 1;
    image_info.mipLevels = 1;
    image_info.arrayLayers = 1;
    image_info.samples = VK_SAMPLE_COUNT_1_BIT;
    image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    image_info.queueFamilyIndexCount = 0;
    image_info.pQueueFamilyIndices = NULL;
    image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    image_info.flags = 0;
    VkMemoryAllocateInfo mem_alloc = {};
    mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    mem_alloc.pNext = NULL;
    mem_alloc.allocationSize = 0;
    mem_alloc.memoryTypeIndex = 0;
    VkImageViewCreateInfo view_info = {};
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.pNext = NULL;
    view_info.image = VK_NULL_HANDLE;
    view_info.format = depthFormat;
    view_info.components.r = VK_COMPONENT_SWIZZLE_R;
    view_info.components.g = VK_COMPONENT_SWIZZLE_G;
    view_info.components.b = VK_COMPONENT_SWIZZLE_B;
    view_info.components.a = VK_COMPONENT_SWIZZLE_A;
    view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.levelCount = 1;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount = 1;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.flags = 0;
    VkResult result = vk::vkCreateImage(device, &image_info, NULL, &depthImage);
    assert(result == VK_SUCCESS);
    VkMemoryRequirements mem_reqs;
    vk::vkGetImageMemoryRequirements(device, depthImage, &mem_reqs);
    mem_alloc.allocationSize = mem_reqs.size;
    VkFlags requirements_mask=0;
    bool flag=memoryTypeFromProperties(memoryroperties, mem_reqs.memoryTypeBits,requirements_mask,&mem_alloc.memoryTypeIndex);
    assert(flag);
    LOGE("确定内存类型成功 类型索引为%d",mem_alloc.memoryTypeIndex);
    result = vk::vkAllocateMemory(device, &mem_alloc, NULL, &memDepth);
    assert(result == VK_SUCCESS);
    result = vk::vkBindImageMemory(device, depthImage, memDepth, 0);
    assert(result == VK_SUCCESS);
    view_info.image = depthImage;
    result = vk::vkCreateImageView(device, &view_info, NULL, &depthImageView);
    assert(result == VK_SUCCESS);
}
void MyVulkanManager::destroy_vulkan_DepthBuffer()
{
    vk::vkDestroyImageView(device, depthImageView, NULL);
    vk::vkDestroyImage(device, depthImage, NULL);
    vk::vkFreeMemory(device, memDepth, NULL);
    LOGE("销毁深度缓冲相关成功!");
}
void MyVulkanManager::create_render_pass()
{
    VkSemaphoreCreateInfo imageAcquiredSemaphoreCreateInfo;
    imageAcquiredSemaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    imageAcquiredSemaphoreCreateInfo.pNext = NULL;
    imageAcquiredSemaphoreCreateInfo.flags = 0;
    VkResult result = vk::vkCreateSemaphore(device, &imageAcquiredSemaphoreCreateInfo, NULL, &imageAcquiredSemaphore);
    assert(result == VK_SUCCESS);
    VkAttachmentDescription attachments[5]; //附件描述信息数组
    VkFormat colorFormat[4] = { //颜色附件格式数组
            VK_FORMAT_R8G8B8A8_UNORM,VK_FORMAT_R32G32B32A32_SFLOAT,
            VK_FORMAT_R32G32B32A32_SFLOAT,VK_FORMAT_R8G8B8A8_UNORM };
    for(int i=0;i<COLOR_ATTACH_COUNT;i++){ //遍历所有颜色附件(服务于各个子通道)
        attachments[i].format = colorFormat[i]; //设置颜色附件的格式
        attachments[i].storeOp=(i==3)?VK_ATTACHMENT_STORE_OP_STORE:VK_ATTACHMENT_STORE_OP_DONT_CARE;//设置渲染通道结束时附件的存储操作
        attachments[i].finalLayout=(i==3)? VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;//设置结束时的最终布局
        attachments[i].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachments[i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachments[i].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachments[i].flags = 0;
    }
    attachments[4].format = depthFormat;
    attachments[4].samples = VK_SAMPLE_COUNT_1_BIT;
    attachments[4].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[4].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[4].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[4].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[4].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[4].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    attachments[4].flags = 0;
    VkAttachmentReference color_reference[COLOR_ATTACH_COUNT];
    for(int i=0;i<COLOR_ATTACH_COUNT;i++)
    {
        color_reference[i].attachment = i;
        color_reference[i].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    }
    VkAttachmentReference depth_reference = {};
    depth_reference.attachment = 4;
    depth_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    VkSubpassDescription subpass[2]; //渲染子通道描述结构体数组
    subpass[0].colorAttachmentCount = 3; //第1 个子通道的颜色附件数量
    subpass[0].pColorAttachments = color_reference; //第1 个子通道的颜色附件引用
    subpass[0].pDepthStencilAttachment = &depth_reference; //第1 个子通道的深度附件引用
    subpass[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass[0].flags = 0;
    subpass[0].inputAttachmentCount = 0;
    subpass[0].pInputAttachments = NULL;
    subpass[0].pResolveAttachments = NULL;
    subpass[0].preserveAttachmentCount = 0;
    subpass[0].pPreserveAttachments = NULL;
    VkAttachmentReference inputReferences[3]; //输入附件引用数组
    inputReferences[0]= {0,VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};//第1 个输入附件的信息设置
    inputReferences[1]= {1,VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};//第2 个输入附件的信息设置
    inputReferences[2]={2,VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};//第3 个输入附件的信息设置
    subpass[1].inputAttachmentCount = 3; //第2 个子通道的输入附件数量
    subpass[1].pInputAttachments = inputReferences; //第2 个子通道的输入附件引用
    subpass[1].colorAttachmentCount = 1; //第2 个子通道的颜色附件数量
    subpass[1].pColorAttachments = &color_reference[3]; //第2 个子通道的颜色附件引用
    subpass[1].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass[1].flags = 0;
    subpass[1].pDepthStencilAttachment = &depth_reference;
    subpass[1].pResolveAttachments = NULL;
    subpass[1].preserveAttachmentCount = 0;
    subpass[1].pPreserveAttachments = NULL;
    VkSubpassDependency dependencies[3]; //子通道依赖关系结构体数组
    dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL; //被依赖子通道的索引(隐含子通道)
    dependencies[0].dstSubpass = 0; //依赖子通道的索引(第1 个子通道)
    dependencies[0].srcStageMask = //被依赖子通道执行操作的阶段(所有管线操作完成阶段
            VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    dependencies[0].dstStageMask = //依赖子通道执行操作的阶段(颜色附件输出阶段)
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[0].srcAccessMask = //被依赖子通道执行的内存操作类型(内存读取)
            VK_ACCESS_MEMORY_READ_BIT;
    dependencies[0].dstAccessMask = //依赖子通道执行的内存操作类型(颜色附件读取或写入
            VK_ACCESS_COLOR_ATTACHMENT_READ_BIT|VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT; //描述依赖类型的标记
    dependencies[1].srcSubpass = 0; //被依赖子通道的索引(第1 个子通道)
    dependencies[1].dstSubpass = 1; //依赖子通道的索引(第2 个子通道)
    dependencies[1].srcStageMask = //被依赖子通道执行操作的阶段(颜色附件输出阶段)
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[1].dstStageMask = //依赖子通道执行操作的阶段(片元着色器阶段)
            VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    dependencies[1].srcAccessMask = //被依赖子通道执行的内存操作类型(写入颜色附件)
            VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[1].dstAccessMask = //依赖子通道执行的内存操作类型(着色器读取)
            VK_ACCESS_SHADER_READ_BIT;
    dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT; //描述依赖类型的标记
    dependencies[2].srcSubpass = 0;
    dependencies[2].dstSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[2].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[2].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    dependencies[2].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[2].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    dependencies[2].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
    VkRenderPassCreateInfo rp_info = {}; //构建渲染通道创建信息结构体实例
    rp_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    rp_info.pNext = NULL;
    rp_info.attachmentCount = 5;
    rp_info.pAttachments = attachments;
    rp_info.subpassCount = 2; //渲染子通道数量
    rp_info.pSubpasses = subpass; //渲染子通道列表
    rp_info.dependencyCount = 3; //子通道依赖数量
    rp_info.pDependencies = dependencies; //子通道依赖列表
    result = vk::vkCreateRenderPass(device, &rp_info, NULL, &renderPass);
    assert(result == VK_SUCCESS);
    for(int i=0;i<COLOR_ATTACH_COUNT;i++)
    {
        clear_values[i].color.float32[0] = 0.2f;
        clear_values[i].color.float32[1] = 0.2f;
        clear_values[i].color.float32[2] = 0.2f;
        clear_values[i].color.float32[3] = 0.2f;
    }
    clear_values[4].depthStencil.depth = 1.0f;
    clear_values[4].depthStencil.stencil = 0;
    rp_begin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rp_begin.pNext = NULL;
    rp_begin.renderPass = renderPass;
    rp_begin.renderArea.offset.x = 0;
    rp_begin.renderArea.offset.y = 0;
    rp_begin.renderArea.extent.width = screenWidth;
    rp_begin.renderArea.extent.height = screenHeight;
    rp_begin.clearValueCount = 5;
    rp_begin.pClearValues = clear_values;
}
void MyVulkanManager::destroy_render_pass()
{
    vk::vkDestroyRenderPass(device, renderPass, NULL);
    vk::vkDestroySemaphore(device, imageAcquiredSemaphore, NULL);
}
void MyVulkanManager::init_queue()
{
    vk::vkGetDeviceQueue(device, queueGraphicsFamilyIndex, 0,&queueGraphics);
}
void MyVulkanManager::create_frame_buffer()
{
    VkImageView attachments[5];
    attachments[4] = depthImageView;
    VkFramebufferCreateInfo fb_info = {};
    fb_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    fb_info.pNext = NULL;
    fb_info.renderPass = renderPass;
    fb_info.attachmentCount = 5;
    fb_info.pAttachments = attachments;
    fb_info.width = screenWidth;
    fb_info.height = screenHeight;
    fb_info.layers = 1;
    attachments[0] = colorImageView[0];
    attachments[1] = colorImageView[1];
    attachments[2] = colorImageView[2];
    uint32_t i;
    framebuffers = (VkFramebuffer *)malloc(swapchainImageCount * sizeof(VkFramebuffer));
    assert(framebuffers);
    LOGE("[创建帧缓冲！-----]");
    for (i = 0; i < swapchainImageCount; i++)
    {
        attachments[3] = swapchainImageViews[i];
        VkResult result = vk::vkCreateFramebuffer(device, &fb_info, NULL, &framebuffers[i]);
        assert(result == VK_SUCCESS);
        LOGE("[创建帧缓冲%d成功！]", i);
    }
}
void MyVulkanManager::destroy_frame_buffer()
{
    for (int i = 0; i < swapchainImageCount; i++)
    {
        vk::vkDestroyFramebuffer(device, framebuffers[i], NULL);
    }
    free(framebuffers);
    LOGE("销毁帧缓冲成功！");
}
void MyVulkanManager::createDrawableObject()
{
    treeForDraw=LoadUtil::loadFromFile("model/tree.obj",device, memoryroperties);
    CHForDraw=LoadUtil::loadFromFile("model/ch_t.obj",device, memoryroperties);
    carForDraw=LoadUtil::loadFromFile("model/car.obj",device, memoryroperties);
    floorForDraw=LoadUtil::loadFromFile("model/pm.obj",device, memoryroperties);
}
void MyVulkanManager::destroyDrawableObject()
{
    delete treeForDraw;
    delete CHForDraw;
    delete carForDraw;
}
void MyVulkanManager::createFence()
{
    VkFenceCreateInfo fenceInfo;
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.pNext = NULL;
    fenceInfo.flags = 0;
    vk::vkCreateFence(device, &fenceInfo, NULL, &taskFinishFence);
}
void MyVulkanManager::destroyFence()
{
    vk::vkDestroyFence(device, taskFinishFence, NULL);
}
void MyVulkanManager::initPresentInfo()
{
    present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present.pNext = NULL;
    present.swapchainCount = 1;
    present.pSwapchains = &swapChain;
    present.pWaitSemaphores = NULL;
    present.waitSemaphoreCount = 0;
    present.pResults = NULL;
}
void MyVulkanManager::initMatrixAndLight()
{
    CameraUtil::calCamera(0,0);
    MatrixState3D::setInitStack();
    ratio=(float)screenWidth/(float)screenHeight;
    MatrixState3D::setProjectFrustum(-ratio,ratio,-1,1,1.5f,1000);
    MatrixState2D::setInitStack();
    MatrixState2D::setProjectOrtho(-ratio,ratio,-1,1,1.5f,1000);
    MatrixState2D::setCamera(0,0,5,0,0,0,0,1,0);
    for (int i = 0;i < LIGHTS_COUNT;i++)
    {
        LightManager::setLightRange(i,350+80*i);
        LightManager::setLightPosition(i, 0, 100, 100);
        LightManager::setlightAmbient(i, 0.025f, 0.025f, 0.025f, 0.025f);
        LightManager::setlightDiffuse(i, 0.4f, 0.4f, 0.4f, 0.4f);
        LightManager::setlightSpecular(i, 0.4f, 0.4f, 0.4f, 0.4f);
    }
}
void MyVulkanManager::flushUniformBuffer()
{
    float* vertexUniformData = new float[(sqsL->bufferByteCount)/4];
    vertexUniformData[0] = CameraUtil::camera9Para[0];
    vertexUniformData[1] = CameraUtil::camera9Para[1];
    vertexUniformData[2] = CameraUtil::camera9Para[2];
    vertexUniformData[3] = 1.0;
    for(int i=0;i<LIGHTS_COUNT;i++)
    {
        memcpy(vertexUniformData+4+i*16, &(LightManager::lightArray[i]),sizeof(float)*16);
    }
    uint8_t *pData;
    VkResult result = vk::vkMapMemory(device, sqsL->memUniformBuf, 0, sqsL->bufferByteCount, 0, (void **)&pData);
    assert(result==VK_SUCCESS);
    memcpy(pData, vertexUniformData, sqsL->bufferByteCount);
    vk::vkUnmapMemory(device,sqsL->memUniformBuf);
}
void MyVulkanManager::flushTexToDesSet()
{
    for(int i=0;i<TextureManager::texNames.size();i++)
    {
        sqsCTL->writes[0].dstSet = sqsCTL->descSet[i];
        sqsCTL->writes[0].pImageInfo = &(TextureManager::texImageInfoList[TextureManager::texNames[i]]);
        vk::vkUpdateDescriptorSets(device, 1, sqsCTL->writes, 0, NULL);
    }
    vk::vkUpdateDescriptorSets(device, 4, sqsL->writes, 0, NULL);
}
void MyVulkanManager::drawObject()
{
    FPSUtil::init();
    while(MyVulkanManager::loopDrawFlag)
    {
        FPSUtil::calFPS();
        FPSUtil::before();
        CameraUtil::flushCameraToMatrix();
        VkResult result = vk::vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAcquiredSemaphore, VK_NULL_HANDLE, &currentBuffer);
        rp_begin.framebuffer = framebuffers[currentBuffer];
        vk::vkResetCommandBuffer(cmdBuffer, 0);
        result = vk::vkBeginCommandBuffer(cmdBuffer, &cmd_buf_info);
        LightManager::move();
        MyVulkanManager::flushUniformBuffer();
        MyVulkanManager::flushTexToDesSet();
        vk::vkCmdBeginRenderPass(cmdBuffer, &rp_begin, VK_SUBPASS_CONTENTS_INLINE);
        MatrixState3D::pushMatrix(); //保护现场
        MatrixState3D::translate(0,0,-100); //沿Z 轴平移
        MatrixState3D::scale(1.6,1.6,1.6); //进行缩放
        CHForDraw->drawSelf(cmdBuffer,sqsCTL->pipelineLayout,sqsCTL->pipeline, //绘制茶壶
                            &(sqsCTL->descSet[TextureManager::getVkDescriptorSetIndex("texture/ghxp.bntex")]));
        MatrixState3D::popMatrix(); //恢复现场
        MatrixState3D::pushMatrix();
        MatrixState3D::translate(-87,0,50);
        treeForDraw->drawSelf(cmdBuffer,sqsCTL->pipelineLayout,sqsCTL->pipeline,
                              &(sqsCTL->descSet[TextureManager::getVkDescriptorSetIndex("texture/tree.bntex")]));
        MatrixState3D::popMatrix();
        MatrixState3D::pushMatrix();
        MatrixState3D::translate(87,0,50);
        carForDraw->drawSelf(cmdBuffer,sqsCTL->pipelineLayout,sqsCTL->pipeline,
                             &(sqsCTL->descSet[TextureManager::getVkDescriptorSetIndex("texture/car.bntex")]));
        MatrixState3D::popMatrix();
        MatrixState3D::pushMatrix();
        floorForDraw->drawSelf(cmdBuffer,sqsCTL->pipelineLayout,sqsCTL->pipeline,
                               &(sqsCTL->descSet[TextureManager::getVkDescriptorSetIndex("texture/floor.bntex")]));
        MatrixState3D::popMatrix();
        vk::vkCmdNextSubpass(cmdBuffer, VK_SUBPASS_CONTENTS_INLINE);//跳转到下一个子通道
        vk::vkCmdBindPipeline(cmdBuffer //绑定第二个子通道的管线
                ,VK_PIPELINE_BIND_POINT_GRAPHICS,sqsL->pipeline);
        vk::vkCmdBindDescriptorSets(cmdBuffer, //绑定描述集
                                    VK_PIPELINE_BIND_POINT_GRAPHICS,sqsL->pipelineLayout,0,1, &(sqsL->descSet[0]), 0, NULL);
        vk::vkCmdDraw(cmdBuffer, 3, 1, 0, 0); //绘制一个三角形
        vk::vkCmdEndRenderPass(cmdBuffer); //结束渲染通道
        result = vk::vkEndCommandBuffer(cmdBuffer);
        submit_info[0].waitSemaphoreCount = 0;
        submit_info[0].pWaitSemaphores = NULL;
        result = vk::vkQueueSubmit(queueGraphics, 1, submit_info, taskFinishFence);
        do
        {
            result = vk::vkWaitForFences(device, 1, &taskFinishFence, VK_TRUE, FENCE_TIMEOUT);
        }
        while (result == VK_TIMEOUT);
        vk::vkResetFences(device,1,&taskFinishFence);
        present.pImageIndices = &currentBuffer;
        result = vk::vkQueuePresentKHR(queueGraphics, &present);
        FPSUtil::after(60);
    }
}
void MyVulkanManager::doVulkan()
{
    ThreadTask* tt=new ThreadTask();
    thread t1(&ThreadTask::doTask,tt);
    t1.detach();
}
void MyVulkanManager::init_texture()
{
    TextureManager::initTextures(device,gpus[0],memoryroperties,cmdBuffer,queueGraphics);
    for(int i=0;i<SELF_COLOR_COUNT;i++)
    {
        colorImageInfo[i].imageView=colorImageView[i];
        colorImageInfo[i].imageLayout=VK_IMAGE_LAYOUT_GENERAL;
        colorImageInfo[i].sampler=TextureManager::samplerList[0];
    }
}
void MyVulkanManager::destroy_textures()
{
    TextureManager::deatroyTextures(device);
}
void MyVulkanManager::initPipeline()
{
    sqsCTL=new ShaderQueueSuit_PreProcess(&device,renderPass,memoryroperties);
    sqsL= new ShaderQueueSuit_Light(&device, renderPass, memoryroperties);
}
void MyVulkanManager::destroyPipeline()
{
    delete sqsCTL;
    delete sqsL;
}
void MyVulkanManager::create_vulkan_SelfColorBufferSpec(VkFormat colorFormat, int index)
{
    VkFormatProperties colorFormatProps;
    VkImageCreateInfo image_info = {};
    vk::vkGetPhysicalDeviceFormatProperties(gpus[0], colorFormat, &colorFormatProps);
    if (colorFormatProps.linearTilingFeatures & VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT)
    {
        image_info.tiling = VK_IMAGE_TILING_LINEAR;
        printf("tiling为VK_IMAGE_TILING_LINEAR！");
    }
    else if (colorFormatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT)
    {
        image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
        printf("tiling为VK_IMAGE_TILING_OPTIMAL！");
    }
    else
    {
        printf("不支持%d！", VK_FORMAT_R8G8B8A8_UNORM);
    }
    image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_info.pNext = NULL;
    image_info.imageType = VK_IMAGE_TYPE_2D;
    image_info.format = colorFormat;
    image_info.extent.width = screenWidth;
    image_info.extent.height = screenHeight;
    image_info.extent.depth = 1;
    image_info.mipLevels = 1;
    image_info.arrayLayers = 1;
    image_info.samples = VK_SAMPLE_COUNT_1_BIT;
    image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_info.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT|VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;;
    image_info.queueFamilyIndexCount = 0;
    image_info.pQueueFamilyIndices = NULL;
    image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    image_info.flags = 0;
    VkResult result = vk::vkCreateImage(device, &image_info, NULL, &(colorImage[index]));
    assert(result == VK_SUCCESS);
    VkMemoryAllocateInfo mem_alloc = {};
    mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    mem_alloc.pNext = NULL;
    mem_alloc.allocationSize = 0;
    mem_alloc.memoryTypeIndex = 0;
    VkMemoryRequirements mem_reqs;
    vk::vkGetImageMemoryRequirements(device, colorImage[index], &mem_reqs);
    mem_alloc.allocationSize = mem_reqs.size;
    VkFlags requirements_mask = 0;
    bool flag = memoryTypeFromProperties(memoryroperties, mem_reqs.memoryTypeBits, requirements_mask, &mem_alloc.memoryTypeIndex);
    assert(flag);
    printf("确定内存类型成功 类型索引为%d", mem_alloc.memoryTypeIndex);
    result = vk::vkAllocateMemory(device, &mem_alloc, NULL, &(memColor[index]));
    assert(result == VK_SUCCESS);
    result = vk::vkBindImageMemory(device, colorImage[index], memColor[index], 0);
    assert(result == VK_SUCCESS);
    VkImageViewCreateInfo view_info = {};
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.pNext = NULL;
    view_info.image = VK_NULL_HANDLE;
    view_info.format = colorFormat;
    view_info.components.r = VK_COMPONENT_SWIZZLE_R;
    view_info.components.g = VK_COMPONENT_SWIZZLE_G;
    view_info.components.b = VK_COMPONENT_SWIZZLE_B;
    view_info.components.a = VK_COMPONENT_SWIZZLE_A;
    view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.levelCount = 1;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount = 1;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.flags = 0;
    view_info.image = colorImage[index];
    result = vk::vkCreateImageView(device, &view_info, NULL, &(colorImageView[index]));
    assert(result == VK_SUCCESS);
}
void MyVulkanManager::create_vulkan_SelfColorBuffer()
{
    VkFormat colorFormat[3] = { VK_FORMAT_R8G8B8A8_UNORM,VK_FORMAT_R32G32B32A32_SFLOAT,VK_FORMAT_R32G32B32A32_SFLOAT};
    for(int i=0;i<SELF_COLOR_COUNT;i++)
    {
        create_vulkan_SelfColorBufferSpec(colorFormat[i], i);
    }
}
void MyVulkanManager::destroy_vulkan_SelfColorBuffer()
{
    for(int i=0;i<SELF_COLOR_COUNT;i++)
    {
        vk::vkDestroyImageView(device, colorImageView[i], NULL);
        vk::vkDestroyImage(device, colorImage[i], NULL);
        vk::vkFreeMemory(device, memColor[i], NULL);
    }
}
