#include <vulkan/vulkan.h>
#include "MatrixState3D.h"
#include "MyVulkanManager.h"
#include "../util/FileUtil.h"
#include "../util/TextureManager.h"
#include "../util/HelpFunction.h"
#include <thread>
#include <iostream>
#include <assert.h>
#include <chrono>
#include "ThreadTask.h"
#include "../util/CameraUtil.h"
#include "../util/LightManager.h"
#include "../util/FPSUtil.h"
#include <sys/time.h>
#include <LoadUtil.h>
#include "VertData.h"
#include "RectData.h"


//静态成员实现
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
VkSubmitInfo MyVulkanManager::submit_info[2];
uint32_t MyVulkanManager::screenWidth;
uint32_t MyVulkanManager::screenHeight;
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
VkClearValue MyVulkanManager::clear_values[2];
VkRenderPassBeginInfo MyVulkanManager::rp_begin;
VkFence MyVulkanManager::taskFinishFence;
VkPresentInfoKHR MyVulkanManager::present;

VkFramebuffer*  MyVulkanManager::framebuffers;

ShaderQueueSuit_SingleTexLight* MyVulkanManager::sqsSTL;
ShaderQueueSuit_ComputeBD*  MyVulkanManager::sqsBD;
ShaderQueueSuit_ComputeNormal*  MyVulkanManager::sqsNormal;
ShaderQueueSuit_CommonTex* MyVulkanManager::sqsTex;
ShaderQueueSuit_CommonTexLoad* MyVulkanManager::sqsLoad;

TexLightObject*  MyVulkanManager::waterForDraw;
TexDrawableObject* MyVulkanManager::texForDraw;
ObjObject* MyVulkanManager::objPool;

float MyVulkanManager::qsj1=0;
float MyVulkanManager::qsj2=45;
float MyVulkanManager::qsj3=90;

//创建vulkan实例的方法
void MyVulkanManager::init_vulkan_instance()
{
    //初始化AAssetManager
    AAssetManager* aam=MyVulkanManager::Android_application->activity->assetManager;
    FileUtil::setAAssetManager(aam);

    //如果加载Vulkan动态库不成功则返回
    if (!vk::loadVulkan())
    {
        LOGI("加载Vulkan API失败!");
        return ;
    }

    //初始化所需扩展列表，下列两个扩展是Android下做渲染必须要的
    instanceExtensionNames.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
    instanceExtensionNames.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);

    //创建VkApplicationInfo结构体实例
    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pNext = NULL;
    app_info.pApplicationName = "HelloVulkan";
    app_info.applicationVersion = 1;
    app_info.pEngineName = "HelloVulkan";
    app_info.engineVersion = 1;
    app_info.apiVersion = VK_API_VERSION_1_0;

    //创建VkInstanceCreateInfo结构体实例
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

    //创建Vulkan实例
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

//销毁vulkan实例的方法
void MyVulkanManager::destroy_vulkan_instance()
{
    //最后销毁Vulkan实例
    vk::vkDestroyInstance(instance, NULL);
    LOGE("Vulkan实例销毁完毕!");
}

//获取硬件设备的方法
void MyVulkanManager::enumerate_vulkan_phy_devices()
{
    //获取设备数量
    gpuCount=0;
    VkResult result = vk::vkEnumeratePhysicalDevices(instance, &gpuCount, NULL);
    assert(result==VK_SUCCESS);
    LOGE("[Vulkan硬件设备数量为%d个]",gpuCount);
    //获取设备列表
    gpus.resize(gpuCount);
    result = vk::vkEnumeratePhysicalDevices(instance, &gpuCount, gpus.data());
    assert(result==VK_SUCCESS);
    //获取物理设备的的内存属性(目前不考虑有多个GPU的设备)
    vk::vkGetPhysicalDeviceMemoryProperties(gpus[0],&memoryroperties);
}

//创建逻辑设备的方法
void MyVulkanManager::create_vulkan_devices()
{
    //获取Vulkan硬件设备0支持的队列家族数量
    vk::vkGetPhysicalDeviceQueueFamilyProperties(gpus[0], &queueFamilyCount, NULL);
    LOGE("[Vulkan硬件设备0支持的队列家族数量为%d]",queueFamilyCount);

    //获取Vulkan硬件设备0支持的队列家族属性列表
    queueFamilyprops.resize(queueFamilyCount);
    vk::vkGetPhysicalDeviceQueueFamilyProperties(gpus[0], &queueFamilyCount, queueFamilyprops.data());
    LOGE("[成功获取Vulkan硬件设备0支持的队列家族属性列表]");

    //创建VkDeviceQueueCreateInfo实例
    VkDeviceQueueCreateInfo queueInfo = {};

    //扫描所有的队列家族，找到其中支持GRAPHICS工作的一个队列家族的索引
    bool found = false;
    for (unsigned int i = 0; i < queueFamilyCount; i++)
    {
        if (queueFamilyprops[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            //记录找到的支持GRAPHICS工作的一个队列家族的索引
            queueInfo.queueFamilyIndex = i;
            queueGraphicsFamilyIndex=i;
            LOGE("[支持GRAPHICS工作的一个队列家族的索引为%d]",i);
            LOGE("[此家族中的实际队列数量是%d]",queueFamilyprops[i].queueCount);
            found = true;
            break;
        }
    }

    //完善kDeviceQueueCreateInfo实例的其他属性
    float queue_priorities[1] = {0.0};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.pNext = NULL;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = queue_priorities;
    queueInfo.queueFamilyIndex = queueGraphicsFamilyIndex;

    //设置逻辑设备所需的扩展,不打开此扩展后面基于逻辑设备创建SwapChain不能真正成功
    deviceExtensionNames.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    //创建VkDeviceCreateInfo实例
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

    //创建设备
    VkResult result = vk::vkCreateDevice(gpus[0], &deviceInfo, NULL, &device);
    assert(result==VK_SUCCESS);
}

//销毁逻辑设备的方法
void MyVulkanManager::destroy_vulkan_devices()
{
    vk::vkDestroyDevice(device, NULL);
    LOGE("逻辑设备销毁完毕！");
}

//创建命令缓冲的方法
void MyVulkanManager::create_vulkan_CommandBuffer()
{
    //创建VkCommandPoolCreateInfo实例
    VkCommandPoolCreateInfo cmd_pool_info = {};
    cmd_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmd_pool_info.pNext = NULL;
    cmd_pool_info.queueFamilyIndex = queueGraphicsFamilyIndex;
    cmd_pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    //创建命令池
    VkResult result = vk::vkCreateCommandPool(device, &cmd_pool_info, NULL, &cmdPool);
    assert(result==VK_SUCCESS);

    //创建VkCommandBufferAllocateInfo实例
    VkCommandBufferAllocateInfo cmdBAI = {};
    cmdBAI.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdBAI.pNext = NULL;
    cmdBAI.commandPool = cmdPool;
    cmdBAI.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmdBAI.commandBufferCount = 1;
    //分配命令缓冲
    result = vk::vkAllocateCommandBuffers(device, &cmdBAI, &cmdBuffer);
    assert(result==VK_SUCCESS);

    //一级命令缓冲启动信息
    cmd_buf_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmd_buf_info.pNext = NULL;
    cmd_buf_info.flags = 0;
    cmd_buf_info.pInheritanceInfo = NULL;

    //要提交到队列执行的CommandBuffer数组
    cmd_bufs[0] = cmdBuffer;
    const VkPipelineStageFlags pipe_stage_flagsRender = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    //渲染用任务提交信息
    submit_info[0].pNext = NULL;
    submit_info[0].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info[0].pWaitDstStageMask = &pipe_stage_flagsRender;
    submit_info[0].commandBufferCount = 1;
    submit_info[0].pCommandBuffers = cmd_bufs;
    submit_info[0].signalSemaphoreCount = 0;
    submit_info[0].pSignalSemaphores = NULL;

    //计算用任务提交信息
    const VkPipelineStageFlags pipe_stage_flagsCompute = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
    submit_info[1].pNext = NULL;
    submit_info[1].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info[1].pWaitDstStageMask = &pipe_stage_flagsCompute;
    submit_info[1].commandBufferCount = 1;
    submit_info[1].pCommandBuffers = cmd_bufs;
    submit_info[1].signalSemaphoreCount = 0;
    submit_info[1].pSignalSemaphores = NULL;
    submit_info[1].waitSemaphoreCount = 0;
    submit_info[1].pWaitSemaphores = NULL;
}

//销毁命令缓冲的方法
void MyVulkanManager::destroy_vulkan_CommandBuffer()
{
    //创建要释放的命令缓冲数组
    VkCommandBuffer cmdBufferArray[1] = {cmdBuffer};
    //释放命令缓冲
    vk::vkFreeCommandBuffers
    (
         device, //所属逻辑设备
         cmdPool,//所属命令池
         1,      //要销毁的命令缓冲数量
         cmdBufferArray//要销毁的命令缓冲数组
    );
    //销毁命令池
    vk::vkDestroyCommandPool(device, cmdPool, NULL);
}

//创建绘制用swapChain的方法
void MyVulkanManager::create_vulkan_swapChain()
{
    //获取屏幕宽度高度
    screenWidth = ANativeWindow_getWidth(Android_application->window);
    screenHeight = ANativeWindow_getHeight(Android_application->window);
    LOGE("窗体宽度%d 窗体高度%d",screenWidth,screenHeight);

    //创建VkAndroidSurfaceCreateInfoKHR实例，为创建surface做准备
    VkAndroidSurfaceCreateInfoKHR createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.window = Android_application->window;

    //获取vkCreateAndroidSurfaceKHR方法入口地址
    PFN_vkCreateAndroidSurfaceKHR fpCreateAndroidSurfaceKHR=(PFN_vkCreateAndroidSurfaceKHR)vk::vkGetInstanceProcAddr(instance, "vkCreateAndroidSurfaceKHR");
    if (fpCreateAndroidSurfaceKHR == NULL)
    {
            LOGE( "找不到vkvkCreateAndroidSurfaceKHR扩展函数！" );
    }
    //创建surface
    VkResult result = fpCreateAndroidSurfaceKHR(instance, &createInfo, nullptr, &surface);
    assert(result==VK_SUCCESS);

    //遍历物理设备对应的队列家族列表，检查每个队列家族是否支持显示工作（有一些队列家族是仅仅支持计算的）
    VkBool32 *pSupportsPresent = (VkBool32 *)malloc(queueFamilyCount * sizeof(VkBool32));
    for (uint32_t i = 0; i < queueFamilyCount; i++)
    {
        vk::vkGetPhysicalDeviceSurfaceSupportKHR(gpus[0], i, surface, &pSupportsPresent[i]);
        LOGE("队列家族索引=%d %s显示",i,(pSupportsPresent[i]==1?"支持":"不支持"));
    }

    //遍历物理设备对应的队列家族列表，找到其中既支持Graphis（图形）又支持Present（显示）工作的队列家族索引
    queueGraphicsFamilyIndex = UINT32_MAX;
    queuePresentFamilyIndex = UINT32_MAX;
    for (uint32_t i = 0; i <queueFamilyCount; ++i)
    {
        //如果当前遍历到的队列家族支持Graphis（图形）工作
        if ((queueFamilyprops[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
        {
            //记录支持Graphis（图形）工作的队列家族索引
            if (queueGraphicsFamilyIndex== UINT32_MAX) queueGraphicsFamilyIndex = i;
            //如果当前遍历到的队列家族支持Present（显示工作）工作
            if (pSupportsPresent[i] == VK_TRUE)
            {
                queueGraphicsFamilyIndex = i;
                queuePresentFamilyIndex = i;
                LOGE("队列家族索引=%d 同时支持Graphis（图形）和Present（显示）工作",i);
                break;
            }
        }
    }

    //如果没有同时支持Graphis（图形）和Present（显示）工作的队列家族
    //则单独记录支持Present（显示）工作的队列家族索引
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

    //没有找到支持Graphis（图形）或Present（显示）工作的队列家族
    if (queueGraphicsFamilyIndex == UINT32_MAX || queuePresentFamilyIndex == UINT32_MAX)
    {
        LOGE("没有找到支持Graphis（图形）或Present（显示）工作的队列家族");
        assert(false);
    }

    //支持的格式数量
    uint32_t formatCount;
    //获取支持的格式数量
    result = vk::vkGetPhysicalDeviceSurfaceFormatsKHR(gpus[0], surface, &formatCount, NULL);
    LOGE("支持的格式数量为 %d",formatCount);
    //分配对应数量的空间
    VkSurfaceFormatKHR *surfFormats = (VkSurfaceFormatKHR *)malloc(formatCount * sizeof(VkSurfaceFormatKHR));
    formats.resize(formatCount);
    //获取支持的格式信息
    result = vk::vkGetPhysicalDeviceSurfaceFormatsKHR(gpus[0], surface, &formatCount, surfFormats);
    //记录支持的格式
    for(int i=0;i<formatCount;i++)
    {
        formats[i]=surfFormats[i].format;
        LOGE("[%d]支持的格式为%d",i,formats[i]);
    }
    //特殊情况处理
    if (formatCount == 1 && surfFormats[0].format == VK_FORMAT_UNDEFINED)
    {
        formats[0] = VK_FORMAT_B8G8R8A8_UNORM;
    }
    //释放辅助空间
    free(surfFormats);

    //获取surface的能力
    result = vk::vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpus[0], surface, &surfCapabilities);
    assert(result == VK_SUCCESS);

    //获取支持的显示模式数量
    result = vk::vkGetPhysicalDeviceSurfacePresentModesKHR(gpus[0], surface, &presentModeCount, NULL);
    assert(result == VK_SUCCESS);
    LOGE("显示模式数量为%d",presentModeCount);

    //调整空间大小
    presentModes.resize(presentModeCount);
    //获取支持的显示模式列表
    result = vk::vkGetPhysicalDeviceSurfacePresentModesKHR(gpus[0], surface, &presentModeCount, presentModes.data());
    for(int i=0;i<presentModeCount;i++)
    {
        LOGE("显示模式[%d]编号为%d",i,presentModes[i]);
    }

    //确定swapChain使用的显示模式
    //VK_PRESENT_MODE_FIFO_KHR是所有实现都肯定支持的模式，因此先选他
    VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
    for (size_t i = 0; i < presentModeCount; i++)
    {
        //如果也支持VK_PRESENT_MODE_MAILBOX_KHR模式，由于其效率高，便选用
        if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
            break;
        }
        if ((swapchainPresentMode != VK_PRESENT_MODE_MAILBOX_KHR)&&(presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR))
        {
            //如果没能用上VK_PRESENT_MODE_MAILBOX_KHR模式，但有VK_PRESENT_MODE_IMMEDIATE_KHR模式
            //也比VK_PRESENT_MODE_FIFO_KHR模式强，故选用
            swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
        }
    }

    //确定surface的宽度、高度
    //如果surface能力中的尺寸没有定义（宽度为0xFFFFFFFF表示没定义）
    if (surfCapabilities.currentExtent.width == 0xFFFFFFFF)
    {
        //设置宽度高度为获取的设备窗体宽度高度
        swapchainExtent.width = screenWidth;
        swapchainExtent.height = screenHeight;
        //宽度设置值限制到最大值与最小值之间
        if (swapchainExtent.width < surfCapabilities.minImageExtent.width)
        {
            swapchainExtent.width = surfCapabilities.minImageExtent.width;
        }
        else if (swapchainExtent.width > surfCapabilities.maxImageExtent.width)
        {
            swapchainExtent.width = surfCapabilities.maxImageExtent.width;
        }
        //高度设置值限制到最大值与最小值之间
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
        //如果surface能力中明确定义了尺寸
        swapchainExtent = surfCapabilities.currentExtent;
        LOGE("使用获取的surface能力中的 宽度 %d 高度 %d",swapchainExtent.width,swapchainExtent.height);
    }

    //记录实际采用的尺寸，以后绘制用Image也采用此尺寸
    screenWidth=swapchainExtent.width;
    screenHeight=swapchainExtent.height;

    //期望的SwapChain中的最少Image数量
    uint32_t desiredMinNumberOfSwapChainImages = surfCapabilities.minImageCount+1;
    //将数量限制到范围内
    if ((surfCapabilities.maxImageCount > 0) &&(desiredMinNumberOfSwapChainImages > surfCapabilities.maxImageCount))
    {
        desiredMinNumberOfSwapChainImages = surfCapabilities.maxImageCount;
    }

    //确定Surface的变换设置标志值
    VkSurfaceTransformFlagBitsKHR preTransform;
    if (surfCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
    {//若支持变换，并且支持VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR
        preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    }
    else
    {//若不支持变换
        preTransform = surfCapabilities.currentTransform;
    }

    //创建VkSwapchainCreateInfoKHR实例
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
        //如果支持Graphis（图形）和Present（显示）工作的队列家族不是一个
        //则在绘制过程中需要将Image在两个队列家族的队列之间进行传输
        //故需要使用VK_SHARING_MODE_CONCURRENT
        swapchain_ci.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchain_ci.queueFamilyIndexCount = 2;
        uint32_t queueFamilyIndices[2] = {queueGraphicsFamilyIndex,queuePresentFamilyIndex};
        swapchain_ci.pQueueFamilyIndices = queueFamilyIndices;
    }
    //创建SwapChain
    result = vk::vkCreateSwapchainKHR(device, &swapchain_ci, NULL, &swapChain);
    assert(result == VK_SUCCESS);

    //获取SwapChain中的Image数量
    result = vk::vkGetSwapchainImagesKHR(device, swapChain, &swapchainImageCount, NULL);
    assert(result == VK_SUCCESS);
    LOGE("[SwapChain中的Image数量为%d]",swapchainImageCount);
    //调整空间
    swapchainImages.resize(swapchainImageCount);
    //获取SwapChain中的Image列表
    result = vk::vkGetSwapchainImagesKHR(device, swapChain, &swapchainImageCount, swapchainImages.data());
    assert(result == VK_SUCCESS);
    //调整尺寸
    swapchainImageViews.resize(swapchainImageCount);
    //循环为每一个swapchain中的Image创建ImageView
    for (uint32_t i = 0; i < swapchainImageCount; i++)
    {
        //创建VkImageViewCreateInfo结构体
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
        //创建ImageView
        result = vk::vkCreateImageView(device, &color_image_view, NULL, &swapchainImageViews[i]);
        assert(result == VK_SUCCESS);
    }
}

//销毁wapChain相关的方法
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

//创建深度缓冲相关
void MyVulkanManager::create_vulkan_DepthBuffer()
{
    depthFormat = VK_FORMAT_D16_UNORM;
    //创建VkImageCreateInfo结构体
    VkImageCreateInfo image_info = {};
    //获取物理设备支持的格式属性
    vk::vkGetPhysicalDeviceFormatProperties(gpus[0], depthFormat, &depthFormatProps);
    //确定tiling方式
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

    //创建VkMemoryAllocateInfo实例
    VkMemoryAllocateInfo mem_alloc = {};
    mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    mem_alloc.pNext = NULL;
    mem_alloc.allocationSize = 0;
    mem_alloc.memoryTypeIndex = 0;

    //创建VkImageViewCreateInfo实例
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

    //获取内存需求
    VkMemoryRequirements mem_reqs;
    vk::vkGetImageMemoryRequirements(device, depthImage, &mem_reqs);
    //获取内存分配尺寸
    mem_alloc.allocationSize = mem_reqs.size;

    //需要的内存类型掩码，0表示没有特殊需要
    VkFlags requirements_mask=0;
    //获取所需内存类型索引
    bool flag=memoryTypeFromProperties(memoryroperties, mem_reqs.memoryTypeBits,requirements_mask,&mem_alloc.memoryTypeIndex);
    assert(flag);
    LOGE("确定内存类型成功 类型索引为%d",mem_alloc.memoryTypeIndex);

    //分配内存
    result = vk::vkAllocateMemory(device, &mem_alloc, NULL, &memDepth);
    assert(result == VK_SUCCESS);

    //绑定内存
    result = vk::vkBindImageMemory(device, depthImage, memDepth, 0);
    assert(result == VK_SUCCESS);

    //创建对应的ImageView
    view_info.image = depthImage;
    result = vk::vkCreateImageView(device, &view_info, NULL, &depthImageView);
    assert(result == VK_SUCCESS);
}

//销毁深度缓冲相关
void MyVulkanManager::destroy_vulkan_DepthBuffer()
{
    vk::vkDestroyImageView(device, depthImageView, NULL);
    vk::vkDestroyImage(device, depthImage, NULL);
    vk::vkFreeMemory(device, memDepth, NULL);
    LOGE("销毁深度缓冲相关成功!");
}

//创建渲染通道
void MyVulkanManager::create_render_pass()
{
    //创建VkSemaphoreCreateInfo实例
    VkSemaphoreCreateInfo imageAcquiredSemaphoreCreateInfo;
    imageAcquiredSemaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    imageAcquiredSemaphoreCreateInfo.pNext = NULL;
    imageAcquiredSemaphoreCreateInfo.flags = 0;
    //创建信号量
    VkResult result = vk::vkCreateSemaphore(device, &imageAcquiredSemaphoreCreateInfo, NULL, &imageAcquiredSemaphore);
    assert(result == VK_SUCCESS);

    //准备颜色、深度附件描述信息
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

    //颜色附件引用
    VkAttachmentReference color_reference = {};
    color_reference.attachment = 0;
    color_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    //深度附件引用
    VkAttachmentReference depth_reference = {};
    depth_reference.attachment = 1;
    depth_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    //渲染子通道描述
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

    //渲染通道创建用信息
    VkRenderPassCreateInfo rp_info = {};
    rp_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    rp_info.pNext = NULL;
    rp_info.attachmentCount = 2;
    rp_info.pAttachments = attachments;
    rp_info.subpassCount = 1;
    rp_info.pSubpasses = &subpass;
    rp_info.dependencyCount = 0;
    rp_info.pDependencies = NULL;

    //创建渲染通道
    result = vk::vkCreateRenderPass(device, &rp_info, NULL, &renderPass);
    assert(result == VK_SUCCESS);

    //清除帧缓冲的颜色分量
    clear_values[0].color.float32[0] = 0.0f;
    clear_values[0].color.float32[1] = 0.0f;
    clear_values[0].color.float32[2] = 0.0f;
    clear_values[0].color.float32[3] = 0.0f;
    //清除帧缓冲的深度数据、模板数据
    clear_values[1].depthStencil.depth = 1.0f;
    clear_values[1].depthStencil.stencil = 0;

    //创建VkRenderPassBeginInfo实例
    rp_begin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rp_begin.pNext = NULL;
    rp_begin.renderPass = renderPass;
    rp_begin.renderArea.offset.x = 0;
    rp_begin.renderArea.offset.y = 0;
    rp_begin.renderArea.extent.width = screenWidth;
    rp_begin.renderArea.extent.height = screenHeight;
    rp_begin.clearValueCount = 2;
    rp_begin.pClearValues = clear_values;
}

//销毁渲染通道相关
void MyVulkanManager::destroy_render_pass()
{
    vk::vkDestroyRenderPass(device, renderPass, NULL);
    vk::vkDestroySemaphore(device, imageAcquiredSemaphore, NULL);
}

//获取设备中支持Graphics工作的Queue
void MyVulkanManager::init_queue()
{
    //获取指定家族中索引为0的队列
    vk::vkGetDeviceQueue(device, queueGraphicsFamilyIndex, 0,&queueGraphics);
}

//创建帧缓冲
void MyVulkanManager::create_frame_buffer()
{
    //深度附件和颜色附件Image数组
    VkImageView attachments[2];
    attachments[1] = depthImageView;

    //帧缓冲创建信息
    VkFramebufferCreateInfo fb_info = {};
    fb_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    fb_info.pNext = NULL;
    fb_info.renderPass = renderPass;
    fb_info.attachmentCount = 2;
    fb_info.pAttachments = attachments;
    fb_info.width = screenWidth;
    fb_info.height = screenHeight;
    fb_info.layers = 1;

    uint32_t i;
    framebuffers = (VkFramebuffer *)malloc(swapchainImageCount * sizeof(VkFramebuffer));
    assert(framebuffers);

    //为swapchain中的所有元素创建帧缓冲
    for (i = 0; i < swapchainImageCount; i++)
    {
        attachments[0] = swapchainImageViews[i];
        VkResult result = vk::vkCreateFramebuffer(device, &fb_info, NULL, &framebuffers[i]);
        assert(result == VK_SUCCESS);
        LOGE("[创建帧缓冲%d成功！]",i);
    }
}

//销毁帧缓冲
void MyVulkanManager::destroy_frame_buffer()
{
    //循环销毁swap chain中每个元素对应的帧缓冲
    for (int i = 0; i < swapchainImageCount; i++)
    {
        vk::vkDestroyFramebuffer(device, framebuffers[i], NULL);
    }
    free(framebuffers);
    LOGE("销毁帧缓冲成功！");
}

void MyVulkanManager::createDrawableObject()
{
    VertData::genVertData();
    waterForDraw=new TexLightObject(VertData::vdata,VertData::dataByteCount,VertData::vCount,VertData::indexData,VertData::indexDataByteCount,VertData::indexCount,device, memoryroperties);
    RectData::getVertexData();
    texForDraw = new TexDrawableObject(RectData::vData, RectData::dataByteCount, RectData::vCount, device, memoryroperties);
    objPool=LoadUtil::loadFromFile("model/poolnew.obj", device, memoryroperties);

}

//销毁绘制用物体
void MyVulkanManager::destroyDrawableObject()
{
    delete waterForDraw;
}

void MyVulkanManager::createFence()
{
    //创建用于等待指定任务执行完毕的Fence
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
    //初始化呈现描述信息
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
    //3D摄像机初始化=====================begin=============================
    //初始化摄像机管理类
    CameraUtil::calCamera(0,0);
    //初始化基本变换矩阵
    MatrixState3D::setInitStack();
    //设置投影参数
    float ratio=(float)screenWidth/(float)screenHeight;
    MatrixState3D::setProjectFrustum(-ratio, ratio, -1, 1, 1.5f, 5000);
    //3D摄像机初始化=====================end===============================

    //初始化光照参数
    LightManager::setLightPosition(0,100,100);
    LightManager::setlightAmbient(0.1f,0.1f,0.1f,0.1f);
    LightManager::setlightDiffuse(0.9f,0.9f,0.5f,0.8f);
    LightManager::setlightSpecular(0.4f,0.4f,0.25f,0.4f);
}

//将当前帧的uniform数据送入uniformBuffer
void MyVulkanManager::flushUniformBufferForDraw()
{
    //单纹理光照物体的UniformBuffer数据========================================================begin=====================================================
    //摄像机位置XYZ 光源位置XYZ 环境光强度rgba 散射光强度rgba 镜面光强度rgba(必须是4的整数倍)
    float vertexUniformData[20]=
    {
            CameraUtil::camera9Para[0],CameraUtil::camera9Para[1],CameraUtil::camera9Para[2],1.0,                                   //摄像机位置XYZ
            LightManager::lx,LightManager::ly,LightManager::lz,1.0,                                                                 //光源位置XYZ
            LightManager::lightAmbientR,LightManager::lightAmbientG,LightManager::lightAmbientB,LightManager::lightAmbientA,        //环境光强度rgba
            LightManager::lightDiffuseR,LightManager::lightDiffuseG,LightManager::lightDiffuseB,LightManager::lightDiffuseA,        //散射光强度rgba
            LightManager::lightSpecularR,LightManager::lightSpecularG,LightManager::lightSpecularB,LightManager::lightSpecularA     //镜面光强度rgba
    };
    //映射的内存地址
    uint8_t *pData;
    //将开辟的内存映射为CPU可访问
    VkResult result = vk::vkMapMemory(device, sqsSTL->memUniformBuf, 0, sqsSTL->bufferByteCount, 0, (void **)&pData);
    assert(result==VK_SUCCESS);
    //将光源位置数据拷贝进显存
    memcpy(pData, vertexUniformData, sqsSTL->bufferByteCount);
    //解除内存映射
    vk::vkUnmapMemory(device,sqsSTL->memUniformBuf);
    //单纹理光照物体的UniformBuffer数据========================================================end=======================================================
}

float myMod(float a,float mod)
{
    while(a>=mod)
    {
        a=a-mod;
    }
    return a;
}

void MyVulkanManager::flushUniformBufferForBD()
{
    //第1个波的起始角
    qsj1=myMod((qsj1+0.09),360);
    //第1个波的起始角
    qsj2=myMod((qsj2+0.18),360);
    //第1个波的起始角
    qsj3=myMod((qsj3+0.04),360);

    float vertexUniformData[16] =
            {
                    50,150,80,0.8,(float)qsj1,
                    10,40,60,1,(float)qsj2,
                    200,200,100,2,(float)qsj3,
                    0
            };
    //映射的内存地址
    uint8_t *pData;
    //将开辟的内存映射为CPU可访问
    VkResult result = vk::vkMapMemory(device, sqsBD->memUniformBuf, 0, sqsBD->bufferByteCount, 0, (void **)&pData);
    assert(result==VK_SUCCESS);
    //将光源位置数据拷贝进显存
    memcpy(pData, vertexUniformData, sqsBD->bufferByteCount);
    //解除内存映射
    vk::vkUnmapMemory(device,sqsBD->memUniformBuf);
}


void MyVulkanManager::flushTexToDesSetForDraw()
{
    for (int i = 0; i<TextureManager::texNamesForCubeMap.size(); i++)
    {
        sqsSTL->writes[0].dstSet = sqsSTL->descSet[i];
        sqsSTL->writes[1].dstSet = sqsSTL->descSet[i];
        sqsSTL->writes[1].pImageInfo = &(TextureManager::texImageInfoList[TextureManager::texNamesForCubeMap[i]]);
        vk::vkUpdateDescriptorSets(device, 2, sqsSTL->writes, 0, NULL);
    }
    for (int i = 0; i<TextureManager::texNamesForSkyBox.size(); i++)
    {
        sqsTex->writes[0].dstSet = sqsTex->descSet[i];
        sqsTex->writes[1].dstSet = sqsTex->descSet[i];
        sqsTex->writes[1].pImageInfo = &(TextureManager::texImageInfoList[TextureManager::texNamesForSkyBox[i]]);
        vk::vkUpdateDescriptorSets(device, 2, sqsTex->writes, 0, NULL);
    }
    for (int i = 0; i<TextureManager::texNamesForSkyBox.size(); i++)
    {
        sqsLoad->writes[0].dstSet = sqsLoad->descSet[i];
        sqsLoad->writes[1].dstSet = sqsLoad->descSet[i];
        sqsLoad->writes[1].pImageInfo = &(TextureManager::texImageInfoList[TextureManager::texNamesForSkyBox[i]]);
        vk::vkUpdateDescriptorSets(device, 2, sqsLoad->writes, 0, NULL);
    }

}

//将纹理与描述集关联
void MyVulkanManager::flushTexToDesSetForBD()
{
    //计算着色器管线的
    sqsBD->writes[0].dstSet = sqsBD->descSet[0];
    sqsBD->writes[1].dstSet = sqsBD->descSet[0];
    sqsBD->writes[2].dstSet = sqsBD->descSet[0];
    sqsBD->writes[0].pBufferInfo=&(waterForDraw->vertexDataBufferInfo);
    sqsBD->writes[1].pBufferInfo=&(waterForDraw->vertexDataBufferInfoCompute);
    vk::vkUpdateDescriptorSets(device, 3, sqsBD->writes, 0, NULL);
}

//将纹理与描述集关联
void MyVulkanManager::flushTexToDesSetForNormal()
{
    //计算着色器管线的
    sqsNormal->writes[0].dstSet = sqsNormal->descSet[0];
    sqsNormal->writes[0].pBufferInfo=&(waterForDraw->vertexDataBufferInfoCompute);
    vk::vkUpdateDescriptorSets(device, 1, sqsNormal->writes, 0, NULL);
}

void MyVulkanManager::drawTask()
{
    CameraUtil::flushCameraToMatrix();

    //获取swapChain中的当前帧索引
    VkResult result = vk::vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAcquiredSemaphore, VK_NULL_HANDLE,&currentBuffer);
    //为RenderPass设置当前的帧缓冲
    rp_begin.framebuffer = framebuffers[currentBuffer];

    //清除一级命令缓冲
    vk::vkResetCommandBuffer(cmdBuffer, 0);
    //启动一级命令缓冲，开始接收命令
    result = vk::vkBeginCommandBuffer(cmdBuffer, &cmd_buf_info);

    //将当前帧的uniform数据送入uniformBuffer
    MyVulkanManager::flushUniformBufferForDraw();
    //将纹理与描述集关联
    MyVulkanManager::flushTexToDesSetForDraw();

    //启动渲染通道
    vk::vkCmdBeginRenderPass(cmdBuffer, &rp_begin, VK_SUBPASS_CONTENTS_INLINE);


    MatrixState3D::pushMatrix();
    MatrixState3D::translate(0, 0, SKY_BOX_HALF_EDGE);
    MatrixState3D::rotate(180, 0, 1, 0);
    texForDraw->drawSelf(cmdBuffer, sqsTex->pipelineLayout, sqsTex->pipeline,
                         &(sqsTex->descSet[TextureManager::getVkDescriptorSetIndexForSkyBox("texture/skycubemap_back.bntex")]));
    MatrixState3D::popMatrix();

    MatrixState3D::pushMatrix();
    MatrixState3D::translate(0, 0, -SKY_BOX_HALF_EDGE);
    texForDraw->drawSelf(cmdBuffer, sqsTex->pipelineLayout, sqsTex->pipeline,
                         &(sqsTex->descSet[TextureManager::getVkDescriptorSetIndexForSkyBox("texture/skycubemap_front.bntex")]));
    MatrixState3D::popMatrix();

    MatrixState3D::pushMatrix();
    MatrixState3D::translate(SKY_BOX_HALF_EDGE, 0, 0);
    MatrixState3D::rotate(90, 0, 1, 0);
    texForDraw->drawSelf(cmdBuffer, sqsTex->pipelineLayout, sqsTex->pipeline,
                         &(sqsTex->descSet[TextureManager::getVkDescriptorSetIndexForSkyBox("texture/skycubemap_left.bntex")]));
    MatrixState3D::popMatrix();

    MatrixState3D::pushMatrix();
    MatrixState3D::translate(-SKY_BOX_HALF_EDGE, 0, 0);
    MatrixState3D::rotate(-90, 0, 1, 0);
    texForDraw->drawSelf(cmdBuffer, sqsTex->pipelineLayout, sqsTex->pipeline,
                         &(sqsTex->descSet[TextureManager::getVkDescriptorSetIndexForSkyBox("texture/skycubemap_right.bntex")]));
    MatrixState3D::popMatrix();

    MatrixState3D::pushMatrix();
    MatrixState3D::translate(0, SKY_BOX_HALF_EDGE, 0);
    MatrixState3D::rotate(90, -1, 0, 0);
    texForDraw->drawSelf(cmdBuffer, sqsTex->pipelineLayout, sqsTex->pipeline,
                         &(sqsTex->descSet[TextureManager::getVkDescriptorSetIndexForSkyBox("texture/skycubemap_up.bntex")]));
    MatrixState3D::popMatrix();

    MatrixState3D::pushMatrix();
    MatrixState3D::translate(0, -SKY_BOX_HALF_EDGE, 0);
    MatrixState3D::rotate(90, 1, 0, 0);
    texForDraw->drawSelf(cmdBuffer, sqsTex->pipelineLayout, sqsTex->pipeline,
                         &(sqsTex->descSet[TextureManager::getVkDescriptorSetIndexForSkyBox("texture/skycubemap_down.bntex")]));
    MatrixState3D::popMatrix();

    MatrixState3D::pushMatrix();
    MatrixState3D::translate(0, -SKY_BOX_HALF_EDGE+200, 0);
    waterForDraw->drawSelf(cmdBuffer,sqsSTL->pipelineLayout,sqsSTL->pipeline,
                         &(sqsSTL->descSet[TextureManager::getVkDescriptorSetIndexForCubemap("texture/cube.bntexcube")]));
    MatrixState3D::popMatrix();

    MatrixState3D::pushMatrix();
    MatrixState3D::translate(0, -SKY_BOX_HALF_EDGE, 0);
    MatrixState3D::scale(100, 100, 100);
    objPool->drawSelf(cmdBuffer, sqsLoad->pipelineLayout, sqsLoad->pipeline,
                      &(sqsLoad->descSet[TextureManager::getVkDescriptorSetIndexForSkyBox("texture/poolnew.bntex")]));
    MatrixState3D::popMatrix();

    //结束渲染通道
    vk::vkCmdEndRenderPass(cmdBuffer);
    //结束一级命令缓冲
    result = vk::vkEndCommandBuffer(cmdBuffer);

    //命令缓冲提交信息更新
    submit_info[0].waitSemaphoreCount = 1;
    submit_info[0].pWaitSemaphores =  &imageAcquiredSemaphore;
    //提交给队列执行
    result = vk::vkQueueSubmit(queueGraphics, 1, &submit_info[0], taskFinishFence);

    //等待渲染完毕
    do
    {
        result = vk::vkWaitForFences(device, 1, &taskFinishFence, VK_TRUE, FENCE_TIMEOUT);
    }
    while (result == VK_TIMEOUT);
    //重置Fence
    vk::vkResetFences(device,1,&taskFinishFence);

    //为呈现做准备
    present.pImageIndices = &currentBuffer;
    //执行呈现
    result = vk::vkQueuePresentKHR(queueGraphics, &present);
}

void MyVulkanManager::calTaskBD()
{
    //清除一级命令缓冲
    vk::vkResetCommandBuffer(cmdBuffer, 0);
    //启动一级命令缓冲，开始接收命令
    VkResult result = vk::vkBeginCommandBuffer(cmdBuffer, &cmd_buf_info);
    //将当前帧的uniform数据送入uniformBuffer
    MyVulkanManager::flushUniformBufferForBD();
    //将纹理与描述集关联
    MyVulkanManager::flushTexToDesSetForBD();

    //计算水面波动
    waterForDraw->calSelfBD(cmdBuffer, sqsBD->pipelineLayout,
                            sqsBD->pipeline, &(sqsBD->descSet[0]));

    //结束一级命令缓冲
    result = vk::vkEndCommandBuffer(cmdBuffer);
    //提交给队列执行
    result = vk::vkQueueSubmit(queueGraphics, 1, &submit_info[1], taskFinishFence);

    //等待渲染完毕
    do
    {
        result = vk::vkWaitForFences(device, 1, &taskFinishFence, VK_TRUE, FENCE_TIMEOUT);
    }
    while (result == VK_TIMEOUT);
    //重置Fence
    vk::vkResetFences(device,1,&taskFinishFence);
}

void MyVulkanManager::calTaskNormal()
{
    //清除一级命令缓冲
    vk::vkResetCommandBuffer(cmdBuffer, 0);
    //启动一级命令缓冲，开始接收命令
    VkResult result = vk::vkBeginCommandBuffer(cmdBuffer, &cmd_buf_info);
    //将纹理与描述集关联
    MyVulkanManager::flushTexToDesSetForNormal();

    //计算波动后法向量
    waterForDraw->calSelfNormal(cmdBuffer, sqsNormal->pipelineLayout,
                                sqsNormal->pipeline, &(sqsNormal->descSet[0]));

    //结束一级命令缓冲
    result = vk::vkEndCommandBuffer(cmdBuffer);
    //提交给队列执行
    result = vk::vkQueueSubmit(queueGraphics, 1, &submit_info[1], taskFinishFence);

    //等待渲染完毕
    do
    {
        result = vk::vkWaitForFences(device, 1, &taskFinishFence, VK_TRUE, FENCE_TIMEOUT);
    }
    while (result == VK_TIMEOUT);
    //重置Fence
    vk::vkResetFences(device,1,&taskFinishFence);
}

void MyVulkanManager::drawObject()
{
    FPSUtil::init();
    //此循环每循环一次绘制一帧画面
    while(MyVulkanManager::loopDrawFlag)
    {
        FPSUtil::calFPS();
        FPSUtil::before();

        calTaskBD();
        calTaskNormal();
        drawTask();

        //限制FPS不超过指定的值
        FPSUtil::after(60);
    }
}

void MyVulkanManager::doVulkan()
{
    ThreadTask* tt=new ThreadTask();
    thread t1(&ThreadTask::doTask,tt);
    t1.detach();
   // tt->doTask();
}

void MyVulkanManager::init_texture()
{
    TextureManager::initTextures(device,gpus[0],memoryroperties,cmdBuffer,queueGraphics);
}

void MyVulkanManager::destroy_textures()
{
    TextureManager::deatroyTextures(device);
}

void MyVulkanManager::initPipeline()
{
    sqsSTL=new ShaderQueueSuit_SingleTexLight(&device,renderPass,memoryroperties);
    sqsBD=new ShaderQueueSuit_ComputeBD(&device,memoryroperties);
    sqsNormal=new ShaderQueueSuit_ComputeNormal(&device,memoryroperties);
    sqsTex=new ShaderQueueSuit_CommonTex(&device, renderPass, memoryroperties);
    sqsLoad=new ShaderQueueSuit_CommonTexLoad(&device, renderPass, memoryroperties);
}

void MyVulkanManager::destroyPipeline()
{
    delete sqsSTL;
    delete sqsBD;
    delete sqsNormal;
    delete sqsTex;
    delete sqsLoad;
}