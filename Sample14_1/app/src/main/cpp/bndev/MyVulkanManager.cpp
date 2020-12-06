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
VkRenderPass MyVulkanManager::renderPassScreen;
VkRenderPass MyVulkanManager::renderPassSelf;
VkClearValue MyVulkanManager::clear_values_screen[2];
VkClearValue MyVulkanManager::clear_values_self[5];
VkRenderPassBeginInfo MyVulkanManager::rp_begin_screen;
VkRenderPassBeginInfo MyVulkanManager::rp_begin_self;
VkFence MyVulkanManager::taskFinishFence;
VkPresentInfoKHR MyVulkanManager::present;
VkFormat MyVulkanManager::colorFormat;
VkFormatProperties MyVulkanManager::colorFormatProps;
VkImage MyVulkanManager::colorImage[4];
VkDeviceMemory MyVulkanManager::memColor[4];
VkImageView MyVulkanManager::colorImageView[4];
VkDescriptorImageInfo MyVulkanManager::colorImageInfo[4];
VkFramebuffer*  MyVulkanManager::framebuffers;
VkFramebuffer MyVulkanManager::selfTexFramebuffer;
ShaderQueueSuit_CommonTexLight* MyVulkanManager::sqsCTL;
ShaderQueueSuit_Dashboard2D* MyVulkanManager::sqsD2D;
ObjObject*  MyVulkanManager::CHForDraw;
ObjObject*  MyVulkanManager::carForDraw;
ObjObject*  MyVulkanManager::treeForDraw;
Dashboard2DObject* MyVulkanManager::d2dA;
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
    VkPipelineStageFlags pipe_stage_flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    submit_info[0].pNext = NULL;
    submit_info[0].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info[0].pWaitDstStageMask = &pipe_stage_flags;
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
void MyVulkanManager::create_render_pass_screen()
{
    VkSemaphoreCreateInfo imageAcquiredSemaphoreCreateInfo;
    imageAcquiredSemaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    imageAcquiredSemaphoreCreateInfo.pNext = NULL;
    imageAcquiredSemaphoreCreateInfo.flags = 0;
    VkResult result = vk::vkCreateSemaphore(device, &imageAcquiredSemaphoreCreateInfo, NULL, &imageAcquiredSemaphore);
    assert(result == VK_SUCCESS);
    VkAttachmentDescription attachments[2];
    attachments[0].format = formats[0];
    attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
    attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    attachments[0].flags = 0;
    attachments[1].format = depthFormat;
    attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
    attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    attachments[1].flags = 0;
    VkAttachmentReference color_reference = {};
    color_reference.attachment = 0;
    color_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    VkAttachmentReference depth_reference = {};
    depth_reference.attachment = 1;
    depth_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.flags = 0;
    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = NULL;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_reference;
    subpass.pResolveAttachments = NULL;
    subpass.pDepthStencilAttachment = &depth_reference;
    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments = NULL;
    VkRenderPassCreateInfo rp_info = {};
    rp_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    rp_info.pNext = NULL;
    rp_info.attachmentCount = 2;
    rp_info.pAttachments = attachments;
    rp_info.subpassCount = 1;
    rp_info.pSubpasses = &subpass;
    rp_info.dependencyCount = 0;
    rp_info.pDependencies = NULL;
    result = vk::vkCreateRenderPass(device, &rp_info, NULL, &renderPassScreen);
    assert(result == VK_SUCCESS);
    clear_values_screen[0].color.float32[0] = 0.2f;
    clear_values_screen[0].color.float32[1] = 0.2f;
    clear_values_screen[0].color.float32[2] = 0.2f;
    clear_values_screen[0].color.float32[3] = 0.2f;
    clear_values_screen[1].depthStencil.depth = 1.0f;
    clear_values_screen[1].depthStencil.stencil = 0;
    rp_begin_screen.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rp_begin_screen.pNext = NULL;
    rp_begin_screen.renderPass = renderPassScreen;
    rp_begin_screen.renderArea.offset.x = 0;
    rp_begin_screen.renderArea.offset.y = 0;
    rp_begin_screen.renderArea.extent.width = screenWidth;
    rp_begin_screen.renderArea.extent.height = screenHeight;
    rp_begin_screen.clearValueCount = 2;
    rp_begin_screen.pClearValues = clear_values_screen;
}
void MyVulkanManager::destroy_render_pass_screen()
{
    vk::vkDestroyRenderPass(device, renderPassScreen, NULL);
    vk::vkDestroySemaphore(device, imageAcquiredSemaphore, NULL);
}
void MyVulkanManager::create_render_pass_self()
{
    VkAttachmentDescription attachments[5]; //附件描述信息数组
    for(int i=0;i<OUT_TEX_COUNT;i++){ //遍历所有的颜色附件
        attachments[i].format = formats[0]; //设置颜色附件的格式
        attachments[i].samples = VK_SAMPLE_COUNT_1_BIT;
        attachments[i].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachments[i].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachments[i].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachments[i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachments[i].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachments[i].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        attachments[i].flags = 0;
    }
    attachments[4].format = depthFormat;//设置深度附件的格式
    attachments[4].samples = VK_SAMPLE_COUNT_1_BIT;
    attachments[4].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[4].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[4].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[4].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[4].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[4].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    attachments[4].flags = 0;
    VkAttachmentReference color_reference[OUT_TEX_COUNT]; //颜色附件引用数组
    for(int i=0;i<OUT_TEX_COUNT;i++){ //遍历所有颜色附件
        color_reference[i].attachment = i; //对应附件描述信息数组元素下标
        color_reference[i].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    }
    VkAttachmentReference depth_reference = {}; //深度附件引用
    depth_reference.attachment = 4; //对应附件描述信息数组元素下标
    depth_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    VkSubpassDescription subpass = {};//构建渲染子通道描述结构体实例
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.flags = 0;
    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = NULL;
    subpass.colorAttachmentCount = OUT_TEX_COUNT;//颜色附件数量
    subpass.pColorAttachments = color_reference;
    subpass.pResolveAttachments = NULL;
    subpass.pDepthStencilAttachment = &depth_reference;
    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments = NULL;
    VkRenderPassCreateInfo rp_info = {};//构建渲染通道创建信息结构体实例
    rp_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;//结构体类型
    rp_info.pNext = NULL; //自定义数据的指针
    rp_info.attachmentCount = 5; //附件的数量（包括颜色附件和深度附件）
    rp_info.pAttachments = attachments;
    rp_info.subpassCount = 1;
    rp_info.pSubpasses = &subpass;
    rp_info.dependencyCount = 0;
    rp_info.pDependencies = NULL;
    VkResult result = vk::vkCreateRenderPass(device, &rp_info, NULL, &renderPassSelf);//创建渲染通道
    assert(result == VK_SUCCESS); //检查是否创建成功
    for(int i=0;i<OUT_TEX_COUNT;i++){ //遍历所有颜色附件
        clear_values_self[i].color.float32[0] = 0.2f; //帧缓冲清除用 R 分量值
        clear_values_self[i].color.float32[1] = 0.2f; //帧缓冲清除用 G 分量值
        clear_values_self[i].color.float32[2] = 0.2f; //帧缓冲清除用 B 分量值
        clear_values_self[i].color.float32[3] = 0.2f; //帧缓冲清除用 A 分量值
    }
    clear_values_self[4].depthStencil.depth = 1.0f; //帧缓冲清除用深度值
    clear_values_self[4].depthStencil.stencil = 0; //帧缓冲清除用模板值
    rp_begin_self.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rp_begin_self.pNext = NULL;
    rp_begin_self.renderPass = renderPassSelf; //指定要启动的渲染通道
    rp_begin_self.renderArea.offset.x = 0;
    rp_begin_self.renderArea.offset.y = 0;
    rp_begin_self.renderArea.extent.width = screenWidth;
    rp_begin_self.renderArea.extent.height = screenHeight;
    rp_begin_self.clearValueCount = 5; //帧缓冲清除值数量
    rp_begin_self.pClearValues = clear_values_self; //帧缓冲清除值数组
}
void MyVulkanManager::destroy_render_pass_self()
{
    vk::vkDestroyRenderPass(device, renderPassSelf, NULL);
}
void MyVulkanManager::init_queue()
{
    vk::vkGetDeviceQueue(device, queueGraphicsFamilyIndex, 0,&queueGraphics);
}
void MyVulkanManager::create_frame_buffer_screen()
{
    VkImageView attachments[2];
    attachments[1] = depthImageView;
    VkFramebufferCreateInfo fb_info = {};
    fb_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    fb_info.pNext = NULL;
    fb_info.renderPass = renderPassScreen;
    fb_info.attachmentCount = 2;
    fb_info.pAttachments = attachments;
    fb_info.width = screenWidth;
    fb_info.height = screenHeight;
    fb_info.layers = 1;
    uint32_t i;
    framebuffers = (VkFramebuffer *)malloc(swapchainImageCount * sizeof(VkFramebuffer));
    assert(framebuffers);
    for (i = 0; i < swapchainImageCount; i++)
    {
        attachments[0] = swapchainImageViews[i];
        VkResult result = vk::vkCreateFramebuffer(device, &fb_info, NULL, &framebuffers[i]);
        assert(result == VK_SUCCESS);
        LOGE("[创建帧缓冲%d成功！]",i);
    }
}
void MyVulkanManager::create_frame_buffer_self()//创建帧缓冲的方法（服务于 MRT）
{
    VkImageView attachments[5]; //深度附件和颜色附件的图像视图数组
    attachments[4] = depthImageView; //指定深度图像视图
    VkFramebufferCreateInfo fb_info = {};//构建帧缓冲创建信息结构体实例
    fb_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    fb_info.pNext = NULL;
    fb_info.renderPass = renderPassSelf;//指定渲染通道
    fb_info.attachmentCount = 5; //附件数量
    fb_info.pAttachments = attachments; //附件图像视图数组
    fb_info.width = screenWidth;
    fb_info.height = screenHeight;
    fb_info.layers = 1;
    attachments[0] = colorImageView[0]; //第 0 个颜色附件的图像视图
    attachments[1] = colorImageView[1]; //第 1 个颜色附件的图像视图
    attachments[2] = colorImageView[2]; //第 2 个颜色附件的图像视图
    attachments[3] = colorImageView[3]; //第 3 个颜色附件的图像视图
    VkResult result = vk::vkCreateFramebuffer(device, &fb_info, NULL, &selfTexFramebuffer); //创建帧缓冲
    assert(result == VK_SUCCESS);//检查是否创建成功
}
void MyVulkanManager::destroy_frame_buffer()
{
    for (int i = 0; i < swapchainImageCount; i++)
    {
        vk::vkDestroyFramebuffer(device, framebuffers[i], NULL);
    }
    free(framebuffers);
    vk::vkDestroyFramebuffer(device, selfTexFramebuffer, NULL);
    LOGE("销毁帧缓冲成功！");
}
void MyVulkanManager::createDrawableObject()
{
    treeForDraw=LoadUtil::loadFromFile("model/tree.obj",device, memoryroperties);
    CHForDraw=LoadUtil::loadFromFile("model/ch_t.obj",device, memoryroperties);
    carForDraw=LoadUtil::loadFromFile("model/car.obj",device, memoryroperties);
    float factorTemp=screenWidth*0.5/screenHeight;
    float* vdataIn=new float[30]
    {
       -factorTemp,0.5,0,0,0, factorTemp,-0.5,0,1,1, factorTemp,0.5,0,1,0,
       -factorTemp,0.5,0,0,0, -factorTemp,-0.5,0,0,1, factorTemp,-0.5,0,1,1
    };
    d2dA=new Dashboard2DObject(vdataIn,30*4,6,device, memoryroperties);
}
void MyVulkanManager::destroyDrawableObject()
{
    delete treeForDraw;
    delete CHForDraw;
    delete carForDraw;
    delete d2dA;
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
    LightManager::setLightPosition(0,100,100);
    LightManager::setlightAmbient(0.1f,0.1f,0.1f,0.1f);
    LightManager::setlightDiffuse(0.6f,0.6f,0.6f,0.6f);
    LightManager::setlightSpecular(0.4f,0.4f,0.4f,0.4f);
}
void MyVulkanManager::flushUniformBufferForToTex()
{
    float vertexUniformData[20]=
    {
            CameraUtil::camera9Para[0],CameraUtil::camera9Para[1],CameraUtil::camera9Para[2],1.0,                                   
            LightManager::lx,LightManager::ly,LightManager::lz,1.0,                                                                 
            LightManager::lightAmbientR,LightManager::lightAmbientG,LightManager::lightAmbientB,LightManager::lightAmbientA,        
            LightManager::lightDiffuseR,LightManager::lightDiffuseG,LightManager::lightDiffuseB,LightManager::lightDiffuseA,        
            LightManager::lightSpecularR,LightManager::lightSpecularG,LightManager::lightSpecularB,LightManager::lightSpecularA     
    };
    uint8_t *pData;
    VkResult result = vk::vkMapMemory(device, sqsCTL->memUniformBuf, 0, sqsCTL->bufferByteCount, 0, (void **)&pData);
    assert(result==VK_SUCCESS);
    memcpy(pData, vertexUniformData, sqsCTL->bufferByteCount);
    vk::vkUnmapMemory(device,sqsCTL->memUniformBuf);
}
void MyVulkanManager::flushTexToDesSetForToTex()
{
    for(int i=0;i<TextureManager::texNamesForCommonTexLight.size();i++)
    {
        sqsCTL->writes[0].dstSet = sqsCTL->descSet[i];
        sqsCTL->writes[1].dstSet = sqsCTL->descSet[i];
        sqsCTL->writes[1].pImageInfo = &(TextureManager::texImageInfoList[TextureManager::texNamesForCommonTexLight[i]]);
        vk::vkUpdateDescriptorSets(device, 2, sqsCTL->writes, 0, NULL);
    }
}
void MyVulkanManager::drawSceneToTex(){ //将场景绘制到纹理的方法（第一轮）
    rp_begin_self.framebuffer = selfTexFramebuffer; //将自定义帧缓冲设置为渲染通道的当前帧缓冲
    vk::vkResetCommandBuffer(cmdBuffer, 0); //恢复命令缓冲到初始状态
    VkResult result = vk::vkBeginCommandBuffer(cmdBuffer, &cmd_buf_info); //启动命令缓冲
    LightManager::move(); //移动光源位置
    MyVulkanManager::flushUniformBufferForToTex(); //将一致变量数据送入渲染管线
    MyVulkanManager::flushTexToDesSetForToTex(); //更新绘制用描述集
    vk::vkCmdBeginRenderPass(cmdBuffer, &rp_begin_self, VK_SUBPASS_CONTENTS_INLINE);
    MatrixState3D::pushMatrix();
    MatrixState3D::translate(0,0,-100);
    MatrixState3D::scale(1.6,1.6,1.6);
    CHForDraw->drawSelf(cmdBuffer,sqsCTL->pipelineLayout,sqsCTL->pipeline,
                        &(sqsCTL->descSet[TextureManager::getVkDescriptorSetIndexForCommonTexLight("texture/ghxp.bntex")]));
    MatrixState3D::popMatrix();
    MatrixState3D::pushMatrix();
    MatrixState3D::translate(-87,0,50);
    treeForDraw->drawSelf(cmdBuffer,sqsCTL->pipelineLayout,sqsCTL->pipeline,
                          &(sqsCTL->descSet[TextureManager::getVkDescriptorSetIndexForCommonTexLight("texture/tree.bntex")]));
    MatrixState3D::popMatrix();
    MatrixState3D::pushMatrix();
    MatrixState3D::translate(87,0,50);
    carForDraw->drawSelf(cmdBuffer,sqsCTL->pipelineLayout,sqsCTL->pipeline,
                         &(sqsCTL->descSet[TextureManager::getVkDescriptorSetIndexForCommonTexLight("texture/car.bntex")]));
    MatrixState3D::popMatrix();
    vk::vkCmdEndRenderPass(cmdBuffer);
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
}
void MyVulkanManager::drawSceneToScreen()//将图像绘制到屏幕的方法（第二轮）
{
    VkResult result = vk::vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAcquiredSemaphore, VK_NULL_HANDLE,&currentBuffer);
    rp_begin_screen.framebuffer = framebuffers[currentBuffer];
    vk::vkResetCommandBuffer(cmdBuffer, 0);
    result = vk::vkBeginCommandBuffer(cmdBuffer, &cmd_buf_info);
    MyVulkanManager::flushUniformBufferForToScreen();
    MyVulkanManager::flushTexToDesSetForToScreen();
    vk::vkCmdBeginRenderPass(cmdBuffer, &rp_begin_screen, VK_SUBPASS_CONTENTS_INLINE);
    MatrixState2D::pushMatrix(); //保护现场
    MatrixState2D::translate(-ratio/2.0f,0.5f,0); //沿 x 轴、y 轴平移
    d2dA->drawSelf(cmdBuffer,sqsD2D->pipelineLayout,sqsD2D->pipeline,//左上角平面矩形的绘制
                   &(sqsD2D->descSet[0])); //使用第 0 个描述集
    MatrixState2D::popMatrix(); //恢复现场
    MatrixState2D::pushMatrix(); //保护现场
    MatrixState2D::translate(ratio/2.0f,0.5f,0); //沿 x 轴、y 轴平移
    d2dA->drawSelf(cmdBuffer,sqsD2D->pipelineLayout,sqsD2D->pipeline, //右上角平面矩形的绘制
                   &(sqsD2D->descSet[1])); //使用第 1 个描述集
    MatrixState2D::popMatrix(); //恢复现场
    MatrixState2D::pushMatrix(); //保护现场
    MatrixState2D::translate(-ratio/2.0f,-0.5f,0); //沿 x 轴、y 轴平移
    d2dA->drawSelf(cmdBuffer,sqsD2D->pipelineLayout,sqsD2D->pipeline, //左下角平面矩形的绘制
                   &(sqsD2D->descSet[2])); //使用第 2 个描述集
    MatrixState2D::popMatrix(); //恢复现场
    MatrixState2D::pushMatrix(); //保护现场
    MatrixState2D::translate(ratio/2.0f,-0.5f,0); //沿 x 轴、y 轴平移
    d2dA->drawSelf(cmdBuffer,sqsD2D->pipelineLayout,sqsD2D->pipeline, //右下角平面矩形的绘制
                   &(sqsD2D->descSet[3])); //使用第 3 个描述集
    MatrixState2D::popMatrix(); //恢复现场
    vk::vkCmdEndRenderPass(cmdBuffer); //结束渲染通道
    result = vk::vkEndCommandBuffer(cmdBuffer);
    submit_info[0].waitSemaphoreCount = 1;
    submit_info[0].pWaitSemaphores =  &imageAcquiredSemaphore;
    result = vk::vkQueueSubmit(queueGraphics, 1, submit_info, taskFinishFence);
    do
    {
        result = vk::vkWaitForFences(device, 1, &taskFinishFence, VK_TRUE, FENCE_TIMEOUT);
    }
    while (result == VK_TIMEOUT);
    vk::vkResetFences(device,1,&taskFinishFence);
    present.pImageIndices = &currentBuffer;
    result = vk::vkQueuePresentKHR(queueGraphics, &present);
}
void MyVulkanManager::drawObject()
{
    FPSUtil::init();
    while(MyVulkanManager::loopDrawFlag)
    {
        FPSUtil::calFPS();
        FPSUtil::before();
        CameraUtil::flushCameraToMatrix();
        drawSceneToTex();
        drawSceneToScreen();
        FPSUtil::after(60);
    }
}
void MyVulkanManager::doVulkan()
{
    ThreadTask* tt=new ThreadTask();
    thread t1(&ThreadTask::doTask,tt);
    t1.detach();
}
void MyVulkanManager::init_texture(){ //初始化纹理的方法
    TextureManager::initTextures(device,gpus[0], //调用方法初始化纹理
                                 memoryroperties,cmdBuffer,queueGraphics);
    for(int i=0;i<OUT_TEX_COUNT;i++){ //遍历所有颜色附件
        colorImageInfo[i].imageView=colorImageView[i]; //采用的图像视图
        colorImageInfo[i].imageLayout=VK_IMAGE_LAYOUT_GENERAL; //图像布局
        colorImageInfo[i].sampler=TextureManager::samplerList[0]; //采用的采样器
    }}

void MyVulkanManager::destroy_textures()
{
    TextureManager::deatroyTextures(device);
}
void MyVulkanManager::initPipeline()
{
    sqsCTL=new ShaderQueueSuit_CommonTexLight(&device,renderPassSelf,memoryroperties);
    sqsD2D=new ShaderQueueSuit_Dashboard2D(&device,renderPassScreen,memoryroperties);
}
void MyVulkanManager::destroyPipeline()
{
    delete sqsCTL;
    delete sqsD2D;
}
void MyVulkanManager::create_vulkan_SelfColorBuffer()//创建颜色缓冲附件图像相关的方法
{
    colorFormat = VK_FORMAT_R8G8B8A8_UNORM; //指定颜色图像的格式
    VkImageCreateInfo image_info = {}; //构建颜色图像创建信息结构体实例
    vk::vkGetPhysicalDeviceFormatProperties(gpus[0], colorFormat, &colorFormatProps);
    if (colorFormatProps.linearTilingFeatures & VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT)
    {
        image_info.tiling = VK_IMAGE_TILING_LINEAR;
        LOGE("tiling为VK_IMAGE_TILING_LINEAR！");
    }
    else if (colorFormatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT)
    {
        image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
        LOGE("tiling为VK_IMAGE_TILING_OPTIMAL！");
    }
    else
    {
        LOGE("不支持VK_FORMAT_R8G8B8A8_UNORM！");
    }
    image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_info.pNext = NULL;
    image_info.imageType = VK_IMAGE_TYPE_2D;
    image_info.format = colorFormat;//图像格式
    image_info.extent.width = screenWidth;
    image_info.extent.height =screenHeight;
    image_info.extent.depth = 1;
    image_info.mipLevels = 1;
    image_info.arrayLayers = 1;
    image_info.samples = VK_SAMPLE_COUNT_1_BIT;
    image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_info.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT|VK_IMAGE_USAGE_SAMPLED_BIT;
    image_info.queueFamilyIndexCount = 0;
    image_info.pQueueFamilyIndices = NULL;
    image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    image_info.flags = 0;
    for(int i=0;i<OUT_TEX_COUNT;i++){ //遍历所有的颜色附件
        VkResult result = vk::vkCreateImage(device, &image_info, NULL, &(colorImage[i])); //创建图像
        assert(result == VK_SUCCESS); //检查是否创建成功
        VkMemoryAllocateInfo mem_alloc = {};
        mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        mem_alloc.pNext = NULL;
        mem_alloc.allocationSize = 0;
        mem_alloc.memoryTypeIndex = 0;
        VkMemoryRequirements mem_reqs;
        vk::vkGetImageMemoryRequirements(device, colorImage[i], &mem_reqs);
        mem_alloc.allocationSize = mem_reqs.size;
        VkFlags requirements_mask=0;
        bool flag=memoryTypeFromProperties(memoryroperties, mem_reqs.memoryTypeBits,requirements_mask,&mem_alloc.memoryTypeIndex);
        assert(flag);
        LOGE("确定内存类型成功 类型索引为%d",mem_alloc.memoryTypeIndex);
        result = vk::vkAllocateMemory(device, &mem_alloc, NULL, &(memColor[i]));
        assert(result == VK_SUCCESS);
        result = vk::vkBindImageMemory(device, colorImage[i], memColor[i], 0);
        assert(result == VK_SUCCESS);
        VkImageViewCreateInfo view_info = {}; //构建颜色图像视图创建信息结构体实例
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
        view_info.image = colorImage[i];//指定图像
        result = vk::vkCreateImageView(device, &view_info, NULL, &(colorImageView[i])); //创建图像视图
        assert(result == VK_SUCCESS); //检查是否创建成功
    }
}
void MyVulkanManager::destroy_vulkan_SelfColorBuffer()
{
    for(int i=0;i<OUT_TEX_COUNT;i++){ //遍历所有颜色附件
        vk::vkDestroyImageView(device, colorImageView[i], NULL); //销毁颜色图像视图对象
        vk::vkDestroyImage(device, colorImage[i], NULL); //销毁颜色图像
        vk::vkFreeMemory(device, memColor[i], NULL); //释放颜色图像内存
    }
}
void MyVulkanManager::flushUniformBufferForToScreen()
{
    float fragmentUniformData[1]={0.9};
    uint8_t *pData;
    VkResult result = vk::vkMapMemory(device, sqsD2D->memUniformBuf, 0, sqsD2D->bufferByteCount, 0, (void **)&pData);
    assert(result==VK_SUCCESS);
    memcpy(pData, fragmentUniformData, sqsD2D->bufferByteCount);
    vk::vkUnmapMemory(device,sqsD2D->memUniformBuf);
}
void MyVulkanManager::flushTexToDesSetForToScreen(){//将绘制到屏幕时的纹理等数据与描述集关联
    for(int i=0;i<OUT_TEX_COUNT;i++) { //遍历所有的颜色附件
        sqsD2D->writes[0].dstSet = sqsD2D->descSet[i]; //描述集对应的写入属性 0（一致变量）
        sqsD2D->writes[1].dstSet = sqsD2D->descSet[i]; //描述集对应的写入属性 1（纹理）
        sqsD2D->writes[1].pImageInfo =&(colorImageInfo[i]); //写入属性 1 对应的纹理图像信息
        vk::vkUpdateDescriptorSets(device, 2, sqsD2D->writes, 0, NULL); //更新描述集
    }}