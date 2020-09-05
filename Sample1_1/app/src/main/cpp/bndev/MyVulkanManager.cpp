#include <vulkan/vulkan.h>
#include "MatrixState3D.h"
#include "MyVulkanManager.h"
#include "../util/FileUtil.h"
#include "../util/HelpFunction.h"
#include <thread>
#include <iostream>
#include <assert.h>
#include <chrono>
#include "ThreadTask.h"
#include "../util/FPSUtil.h"
#include "TriangleData.h"
#include <sys/time.h>

//静态成员实现
android_app *MyVulkanManager::Android_application;
bool MyVulkanManager::loopDrawFlag = true;
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
VkFramebuffer *MyVulkanManager::framebuffers;
ShaderQueueSuit_Common *MyVulkanManager::sqsCL;
DrawableObjectCommonLight *MyVulkanManager::triForDraw;
float MyVulkanManager::xAngle = 0;

//创建Vulkan实例的方法
void MyVulkanManager::init_vulkan_instance() {
    AAssetManager *aam = MyVulkanManager::Android_application->activity->assetManager;//获取资源管理器指针
    FileUtil::setAAssetManager(aam);//将资源管理器传给文件IO工具类
    if (!vk::loadVulkan()) {//加载Vulkan动态库
        LOGI("加载Vulkan图形应用程序接口失败!");
        return;
    }
    instanceExtensionNames.push_back(VK_KHR_SURFACE_EXTENSION_NAME);//初始化所需实例扩展名称列表
    instanceExtensionNames.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);

    VkApplicationInfo app_info = {};//构建应用信息结构体实例
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;//结构体的类型
    app_info.pNext = NULL;//自定义数据的指针
    app_info.pApplicationName = "HelloVulkan";//应用的名称
    app_info.applicationVersion = 1;//应用的版本号
    app_info.pEngineName = "HelloVulkan";//应用的引擎名称
    app_info.engineVersion = 1;//应用的引擎版本号
    app_info.apiVersion = VK_API_VERSION_1_0;//使用的Vulkan图形应用程序API版本

    VkInstanceCreateInfo inst_info = {};//构建实例创建信息结构体实例
    inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;//结构体的类型
    inst_info.pNext = NULL;//自定义数据的指针
    inst_info.flags = 0;//供将来使用的标志
    inst_info.pApplicationInfo = &app_info;//绑定应用信息结构体
    inst_info.enabledExtensionCount = instanceExtensionNames.size();//扩展的数量
    inst_info.ppEnabledExtensionNames = instanceExtensionNames.data();//扩展名称列表数据
    inst_info.enabledLayerCount = 0;//启动的层数量
    inst_info.ppEnabledLayerNames = NULL;//启动的层名称列表
    VkResult result;//存储运行结果的辅助变量
    //创建Vulkan实例
    result = vk::vkCreateInstance(&inst_info, NULL, &instance);
    if (result == VK_SUCCESS) {//检查实例是否创建成功
        LOGE("Vulkan实例创建成功!");
    } else {
        LOGE("Vulkan实例创建失败!");
    }
}

//销毁Vulkan实例的方法
void MyVulkanManager::destroy_vulkan_instance() {
    vk::vkDestroyInstance(instance, NULL);
    LOGE("Vulkan实例销毁完毕!");
}

//获取物理设备列表
void MyVulkanManager::enumerate_vulkan_phy_devices() {
    gpuCount = 0;//存储物理设备数量的变量
    VkResult result = vk::vkEnumeratePhysicalDevices(instance, &gpuCount, NULL);//获取物理设备数量
    assert(result == VK_SUCCESS);
    LOGE("[Vulkan硬件设备数量为%d个]", gpuCount);
    gpus.resize(gpuCount);//设置物理设备列表尺寸
    result = vk::vkEnumeratePhysicalDevices(instance, &gpuCount, gpus.data());//填充物理设备列表
    assert(result == VK_SUCCESS);
    vk::vkGetPhysicalDeviceMemoryProperties(gpus[0], &memoryroperties);//获取第一物理设备的内存属性
}

//创建逻辑设备的方法
void MyVulkanManager::create_vulkan_devices() {
    vk::vkGetPhysicalDeviceQueueFamilyProperties(gpus[0], &queueFamilyCount, NULL);//获取物理设备0中队列家族的数量
    LOGE("[Vulkan硬件设备0支持的队列家族数量为%d]", queueFamilyCount);
    queueFamilyprops.resize(queueFamilyCount);//随队列家族数量改变vector长度
    vk::vkGetPhysicalDeviceQueueFamilyProperties(gpus[0], &queueFamilyCount,
                                                 queueFamilyprops.data());//填充物理设备0队列家族属性列表
    LOGE("[成功获取Vulkan硬件设备0支持的队列家族属性列表]");

    VkDeviceQueueCreateInfo queueInfo = {};//构建设备队列创建信息结构体实例
    bool found = false;//辅助标志
    for (unsigned int i = 0; i < queueFamilyCount; i++) {//遍历所有队列家族
        if (queueFamilyprops[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {//若当前队列家族支持图形工作
            queueInfo.queueFamilyIndex = i;//绑定此队列家族索引
            queueGraphicsFamilyIndex = i;//记录支持图形工作的队列家族索引
            LOGE("[支持GRAPHICS工作的一个队列家族的索引为%d]", i);
            LOGE("[此家族中的实际队列数量是%d]", queueFamilyprops[i].queueCount);
            found = true;
            break;
        }
    }

    float queue_priorities[1] = {0.0};//创建队列优先级数组
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;//给出结构体类型
    queueInfo.pNext = NULL;//自定义数据的指针
    queueInfo.queueCount = 1;//指定队列数量
    queueInfo.pQueuePriorities = queue_priorities;//给出每个队列的优先级
    queueInfo.queueFamilyIndex = queueGraphicsFamilyIndex;//绑定队列家族索引
    deviceExtensionNames.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);//设置所需扩展

    VkDeviceCreateInfo deviceInfo = {};//构建逻辑设备创建信息结构体实例
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;//给出结构体类型
    deviceInfo.pNext = NULL;//自定义数据的指针
    deviceInfo.queueCreateInfoCount = 1;//指定设备队列创建信息结构体数量
    deviceInfo.pQueueCreateInfos = &queueInfo;//给定设备队列创建信息结构体列表
    deviceInfo.enabledExtensionCount = deviceExtensionNames.size();//所需扩展数量
    deviceInfo.ppEnabledExtensionNames = deviceExtensionNames.data();//所需扩展列表
    deviceInfo.enabledLayerCount = 0;//需启动Layer的数量
    deviceInfo.ppEnabledLayerNames = NULL;//需启动Layer的名称列表
    deviceInfo.pEnabledFeatures = NULL;//启用的设备特性
    VkResult result = vk::vkCreateDevice(gpus[0], &deviceInfo, NULL, &device);//创建逻辑设备
    assert(result == VK_SUCCESS);//检查逻辑设备是否创建成功
}

//销毁逻辑设备的方法
void MyVulkanManager::destroy_vulkan_devices() {
    vk::vkDestroyDevice(device, NULL);
    LOGE("逻辑设备销毁完毕！");
}

//创建命令缓冲的方法
void MyVulkanManager::create_vulkan_CommandBuffer() {

    VkCommandPoolCreateInfo cmd_pool_info = {};//构建命令池创建信息结构体实例
    cmd_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;    //给定结构体类型
    cmd_pool_info.pNext = NULL;//自定义数据的指针
    cmd_pool_info.queueFamilyIndex = queueGraphicsFamilyIndex;//绑定所需队列家族索引
    cmd_pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;    //执行控制标志
    VkResult result = vk::vkCreateCommandPool(device, &cmd_pool_info, NULL, &cmdPool);//创建命令池
    assert(result == VK_SUCCESS);//检查命令池创建是否成功

    VkCommandBufferAllocateInfo cmdBAI = {};//构建命令缓冲分配信息结构体实例
    cmdBAI.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;//给定结构体类型
    cmdBAI.pNext = NULL;//自定义数据的指针
    cmdBAI.commandPool = cmdPool;//指定命令池
    cmdBAI.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;//分配的命令缓冲级别
    cmdBAI.commandBufferCount = 1;//分配的命令缓冲数量
    result = vk::vkAllocateCommandBuffers(device, &cmdBAI, &cmdBuffer);//分配命令缓冲

    assert(result == VK_SUCCESS);//检查分配是否成功
    cmd_buf_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;//给定结构体类型
    cmd_buf_info.pNext = NULL;//自定义数据的指针
    cmd_buf_info.flags = 0;//描述使用标志
    cmd_buf_info.pInheritanceInfo = NULL;//命令缓冲继承信息
    cmd_bufs[0] = cmdBuffer;//要提交到队列执行的命令缓冲数组

    VkPipelineStageFlags *pipe_stage_flags = new VkPipelineStageFlags();//目标管线阶段
    *pipe_stage_flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    submit_info[0].pNext = NULL;//自定义数据的指针
    submit_info[0].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;//给定结构体类型
    submit_info[0].pWaitDstStageMask = pipe_stage_flags;//给定目标管线阶段
    submit_info[0].commandBufferCount = 1;//命令缓冲数量
    submit_info[0].pCommandBuffers = cmd_bufs;//提交的命令缓冲数组
    submit_info[0].signalSemaphoreCount = 0;//信号量数量
    submit_info[0].pSignalSemaphores = NULL;//信号量数组
}


//销毁命令缓冲的方法
void MyVulkanManager::destroy_vulkan_CommandBuffer() {
    VkCommandBuffer cmdBufferArray[1] = {cmdBuffer};//创建要释放的命令缓冲数组
    vk::vkFreeCommandBuffers(//释放命令缓冲
            device, //所属逻辑设备
            cmdPool,//所属命令池
            1,//要销毁的命令缓冲数量
            cmdBufferArray//要销毁的命令缓冲数组
    );
    vk::vkDestroyCommandPool(device, cmdPool, NULL);//销毁命令池
}

//创建绘制用交换链的方法
void MyVulkanManager::create_vulkan_swapChain() {
    screenWidth = ANativeWindow_getWidth(Android_application->window);//获取屏幕宽度
    screenHeight = ANativeWindow_getHeight(Android_application->window);//获取屏幕高度
    LOGE("窗体宽度%d 窗体高度%d", screenWidth, screenHeight);
    VkAndroidSurfaceCreateInfoKHR createInfo;//构建KHR表面创建信息结构体实例
    createInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;//给定结构体类型
    createInfo.pNext = nullptr;    //自定义数据的指针
    createInfo.flags = 0;//供未来使用的标志
    createInfo.window = Android_application->window;//给定窗体
    PFN_vkCreateAndroidSurfaceKHR fpCreateAndroidSurfaceKHR =//动态加载创建KHR表面的方法
            (PFN_vkCreateAndroidSurfaceKHR) vk::vkGetInstanceProcAddr(instance,
                                                                      "vkCreateAndroidSurfaceKHR");//加载Android平台所需方法
    if (fpCreateAndroidSurfaceKHR == NULL) {    //判断方法是否加载成功
        LOGE("找不到vkCreateAndroidSurfaceKHR扩展函数！");
    }
    VkResult result = fpCreateAndroidSurfaceKHR(instance, &createInfo, nullptr,
                                                &surface);//创建Android平台用KHR表面
    assert(result == VK_SUCCESS);//检查是否创建成功
    VkBool32 *pSupportsPresent = (VkBool32 *) malloc(queueFamilyCount * sizeof(VkBool32));
    for (uint32_t i = 0; i < queueFamilyCount; i++) {//遍历设备对应的队列家族列表
        vk::vkGetPhysicalDeviceSurfaceSupportKHR(gpus[0], i, surface, &pSupportsPresent[i]);
        LOGE("队列家族索引=%d %s显示", i, (pSupportsPresent[i] == 1 ? "支持" : "不支持"));
    }
    queueGraphicsFamilyIndex = UINT32_MAX;//支持图形工作的队列家族索引
    queuePresentFamilyIndex = UINT32_MAX;//支持显示(呈现)工作的队列家族索引
    for (uint32_t i = 0; i < queueFamilyCount; ++i)//遍历设备对应的队列家族列表
    {
        if ((queueFamilyprops[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)//若此队列家族支持图形工作
        {
            //记录支持图形工作的队列家族索引
            if (queueGraphicsFamilyIndex == UINT32_MAX) queueGraphicsFamilyIndex = i;
            if (pSupportsPresent[i] == VK_TRUE)//如果当前队列家族支持显示工作
            {
                queueGraphicsFamilyIndex = i;//记录此队列家族索引为支持图形工作的
                queuePresentFamilyIndex = i;//记录此队列家族索引为支持显示工作的
                LOGE("队列家族索引=%d 同时支持Graphis（图形）和Present（呈现）工作", i);
                break;
            }
        }
    }
    if (queuePresentFamilyIndex == UINT32_MAX)//若没有找到同时支持两项工作的队列家族
    {
        for (size_t i = 0; i < queueFamilyCount; ++i)//遍历设备对应的队列家族列表
        {
            if (pSupportsPresent[i] == VK_TRUE)//判断是否支持显示工作
            {
                queuePresentFamilyIndex = i;//记录此队列家族索引为支持显示工作的
                break;
            }
        }
    }
    free(pSupportsPresent);//释放存储是否支持呈现工作的布尔值列表

    //没有找到支持图形或显示工作的队列家族
    if (queueGraphicsFamilyIndex == UINT32_MAX || queuePresentFamilyIndex == UINT32_MAX) {
        LOGE("没有找到支持Graphis（图形）或Present（显示）工作的队列家族");
        assert(false);//若没有支持图形或显示操作的队列家族则程序终止
    }

    uint32_t formatCount;//支持的格式数量
    result = vk::vkGetPhysicalDeviceSurfaceFormatsKHR(gpus[0], surface, &formatCount,
                                                      NULL);//获取支持的格式数量
    LOGE("支持的格式数量为 %d", formatCount);
    VkSurfaceFormatKHR *surfFormats = (VkSurfaceFormatKHR *) malloc(
            formatCount * sizeof(VkSurfaceFormatKHR));//分配对应数量的空间
    formats.resize(formatCount);//调整对应Vector尺寸
    result = vk::vkGetPhysicalDeviceSurfaceFormatsKHR(gpus[0], surface, &formatCount,
                                                      surfFormats);//获取支持的格式信息
    for (int i = 0; i < formatCount; i++) {//记录支持的格式信息
        formats[i] = surfFormats[i].format;
        LOGE("[%d]支持的格式为%d", i, formats[i]);
    }
    if (formatCount == 1 && surfFormats[0].format == VK_FORMAT_UNDEFINED) {//特殊情况处理
        formats[0] = VK_FORMAT_B8G8R8A8_UNORM;
    }
    free(surfFormats);
    //获取KHR表面的能力
    result = vk::vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpus[0], surface, &surfCapabilities);
    assert(result == VK_SUCCESS);

    //获取支持的显示模式数量
    result = vk::vkGetPhysicalDeviceSurfacePresentModesKHR(gpus[0], surface, &presentModeCount,
                                                           NULL);
    assert(result == VK_SUCCESS);
    LOGE("显示模式数量为%d", presentModeCount);

    //调整对应Vector尺寸
    presentModes.resize(presentModeCount);
    //获取支持的显示模式列表
    result = vk::vkGetPhysicalDeviceSurfacePresentModesKHR(gpus[0], surface, &presentModeCount,
                                                           presentModes.data());
    for (int i = 0; i < presentModeCount; i++)//遍历打印所有显示模式的信息
    {
        LOGE("显示模式[%d]编号为%d", i, presentModes[i]);
    }
    VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;//确定交换链显示模式
    for (size_t i = 0; i < presentModeCount; i++)//遍历显示模式列表
    {
        //如果也支持VK_PRESENT_MODE_MAILBOX_KHR模式，由于其效率高，便选用
        if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)    //若支持MAILBOX模式
        {
            swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
            break;
        }
        if ((swapchainPresentMode != VK_PRESENT_MODE_MAILBOX_KHR) &&
            (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR))//若支持IMMEDIATE模式
        {
            //如果没能用上VK_PRESENT_MODE_MAILBOX_KHR模式，但有VK_PRESENT_MODE_IMMEDIATE_KHR模式
            //也比VK_PRESENT_MODE_FIFO_KHR模式强，故选用
            swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
        }
    }

    //若表面没有确定尺寸
    if (surfCapabilities.currentExtent.width == 0xFFFFFFFF) {
        swapchainExtent.width = screenWidth;//设置宽度为窗体宽度
        swapchainExtent.height = screenHeight;//设置高度为窗体高度
        //限制宽度在范围内
        if (swapchainExtent.width < surfCapabilities.minImageExtent.width) {
            swapchainExtent.width = surfCapabilities.minImageExtent.width;
        } else if (swapchainExtent.width > surfCapabilities.maxImageExtent.width) {
            swapchainExtent.width = surfCapabilities.maxImageExtent.width;
        }
        //限制高度在范围内
        if (swapchainExtent.height < surfCapabilities.minImageExtent.height) {
            swapchainExtent.height = surfCapabilities.minImageExtent.height;
        } else if (swapchainExtent.height > surfCapabilities.maxImageExtent.height) {
            swapchainExtent.height = surfCapabilities.maxImageExtent.height;
        }
        LOGE("使用自己设置的 宽度 %d 高度 %d", swapchainExtent.width, swapchainExtent.height);
    } else {
        //若表面有确定尺寸
        swapchainExtent = surfCapabilities.currentExtent;
        LOGE("使用获取的surface能力中的 宽度 %d 高度 %d", swapchainExtent.width, swapchainExtent.height);
    }

    screenWidth = swapchainExtent.width;//记录实际采用的宽度
    screenHeight = swapchainExtent.height;//记录实际采用的高度

    uint32_t desiredMinNumberOfSwapChainImages = surfCapabilities.minImageCount + 1;//期望交换链中的最少图像数量
    //将图像数量限制到范围内
    if ((surfCapabilities.maxImageCount > 0) &&
        (desiredMinNumberOfSwapChainImages > surfCapabilities.maxImageCount)) {
        desiredMinNumberOfSwapChainImages = surfCapabilities.maxImageCount;
    }

    VkSurfaceTransformFlagBitsKHR preTransform;//KHR表面变换标志
    if (surfCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)//若支持所需的变换
    {
        preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    } else//若不支持所需的变换
    {
        preTransform = surfCapabilities.currentTransform;
    }
    VkSwapchainCreateInfoKHR swapchain_ci = {};//构建交换链创建信息结构体实例
    swapchain_ci.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;//结构体类型
    swapchain_ci.pNext = NULL;//自定义数据的指针
    swapchain_ci.surface = surface;//指定KHR表面
    swapchain_ci.minImageCount = desiredMinNumberOfSwapChainImages;//最少图像数量
    swapchain_ci.imageFormat = formats[0];//图像格式
    swapchain_ci.imageExtent.width = swapchainExtent.width;//交换链图像宽度
    swapchain_ci.imageExtent.height = swapchainExtent.height;//交换链图像高度
    swapchain_ci.preTransform = preTransform;//指定变换标志
    swapchain_ci.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;//混合Alpha值
    swapchain_ci.imageArrayLayers = 1;//图像数组层数
    swapchain_ci.presentMode = swapchainPresentMode;//交换链的显示模式
    swapchain_ci.oldSwapchain = VK_NULL_HANDLE;//前导交换链
    swapchain_ci.clipped = true;//开启剪裁
    swapchain_ci.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;//色彩空间
    swapchain_ci.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;//图像用途
    swapchain_ci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;    //图像共享模式
    swapchain_ci.queueFamilyIndexCount = 0;//队列家族数量
    swapchain_ci.pQueueFamilyIndices = NULL;//队列家族索引列表

    if (queueGraphicsFamilyIndex != queuePresentFamilyIndex)//若支持图形和显示工作的队列家族不相同
    {
        swapchain_ci.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchain_ci.queueFamilyIndexCount = 2;//交换链所需的队列家族索引数量为2
        uint32_t queueFamilyIndices[2] = {queueGraphicsFamilyIndex, queuePresentFamilyIndex};
        swapchain_ci.pQueueFamilyIndices = queueFamilyIndices;//交换链所需的队列家族索引列表
    }

    result = vk::vkCreateSwapchainKHR(device, &swapchain_ci, NULL, &swapChain);//创建交换链
    assert(result == VK_SUCCESS);//检查交换链是否创建成功

    //获取交换链中的图像数量
    result = vk::vkGetSwapchainImagesKHR(device, swapChain, &swapchainImageCount, NULL);
    assert(result == VK_SUCCESS);//检查是否获取成功
    LOGE("[SwapChain中的Image数量为%d]", swapchainImageCount);
    swapchainImages.resize(swapchainImageCount);//调整图像列表尺寸
    //获取交换链中的图像列表
    result = vk::vkGetSwapchainImagesKHR(device, swapChain, &swapchainImageCount,
                                         swapchainImages.data());
    assert(result == VK_SUCCESS);
    //调整尺寸
    swapchainImageViews.resize(swapchainImageCount);//调整图像视图列表尺寸
    for (uint32_t i = 0; i < swapchainImageCount; i++)//为交换链中的各幅图像创建图像视图
    {
        VkImageViewCreateInfo color_image_view = {};//构建图像视图创建信息结构体实例
        color_image_view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;//设置结构体类型
        color_image_view.pNext = NULL;//自定义数据的指针
        color_image_view.flags = 0;//供将来使用的标志
        color_image_view.image = swapchainImages[i];//对应交换链图像
        color_image_view.viewType = VK_IMAGE_VIEW_TYPE_2D;//图像视图的类型
        color_image_view.format = formats[0];//图像视图格式
        color_image_view.components.r = VK_COMPONENT_SWIZZLE_R;//设置R通道调和
        color_image_view.components.g = VK_COMPONENT_SWIZZLE_G;//设置G通道调和
        color_image_view.components.b = VK_COMPONENT_SWIZZLE_B;//设置B通道调和
        color_image_view.components.a = VK_COMPONENT_SWIZZLE_A;//设置A通道调和
        color_image_view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;//图像视图使用方面
        color_image_view.subresourceRange.baseMipLevel = 0;//基础Mipmap级别
        color_image_view.subresourceRange.levelCount = 1;//Mipmap级别的数量
        color_image_view.subresourceRange.baseArrayLayer = 0;//基础数组层
        color_image_view.subresourceRange.layerCount = 1;//数组层的数量
        result = vk::vkCreateImageView(device, &color_image_view, NULL,
                                       &swapchainImageViews[i]);//创建图像视图
        assert(result == VK_SUCCESS);//检查是否创建成功
    }
}

void MyVulkanManager::destroy_vulkan_swapChain()//销毁交换链相关的方法
{
    for (uint32_t i = 0; i < swapchainImageCount; i++) {
        vk::vkDestroyImageView(device, swapchainImageViews[i], NULL);
        LOGE("[销毁SwapChain ImageView %d 成功]", i);
    }
    vk::vkDestroySwapchainKHR(device, swapChain, NULL);
    LOGE("销毁SwapChain成功!");
}

//创建深度缓冲相关
void MyVulkanManager::create_vulkan_DepthBuffer() {
    depthFormat = VK_FORMAT_D16_UNORM;//指定深度图像的格式
    //创建深度图像创建信息结构体
    VkImageCreateInfo image_info = {};//构建深度图像创建信息结构体实例
    vk::vkGetPhysicalDeviceFormatProperties(gpus[0], depthFormat,
                                            &depthFormatProps);//获取物理设备支持的指定格式的属性
    //确定平铺方式
    if (depthFormatProps.linearTilingFeatures &
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)//是否支持线性瓦片组织方式
    {
        image_info.tiling = VK_IMAGE_TILING_LINEAR;//采用线性瓦片组织方式
        LOGE("tiling为VK_IMAGE_TILING_LINEAR！");
    } else if (depthFormatProps.optimalTilingFeatures &
               VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)//是否支持最优瓦片组织方式
    {
        image_info.tiling = VK_IMAGE_TILING_OPTIMAL;//采用最优瓦片组织方式
        LOGE("tiling为VK_IMAGE_TILING_OPTIMAL！");
    } else {
        LOGE("不支持VK_FORMAT_D16_UNORM！");//打印不支持指定格式的提示信息
    }
    image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;    //指定结构体类型
    image_info.pNext = NULL;//自定义数据的指针
    image_info.imageType = VK_IMAGE_TYPE_2D;//图像类型
    image_info.format = depthFormat;//图像格式
    image_info.extent.width = screenWidth;//图像宽度
    image_info.extent.height = screenHeight;//图像高度
    image_info.extent.depth = 1;//图像深度
    image_info.mipLevels = 1;//图像mipmap级数
    image_info.arrayLayers = 1;//图像数组层数量
    image_info.samples = VK_SAMPLE_COUNT_1_BIT;//采样模式
    image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;//初始布局
    image_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;//图像用途
    image_info.queueFamilyIndexCount = 0;//队列家族数量
    image_info.pQueueFamilyIndices = NULL;//队列家族索引列表
    image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;//共享模式
    image_info.flags = 0;//标志

    VkMemoryAllocateInfo mem_alloc = {};//构建内存分配信息结构体实例
    mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;//结构体类型
    mem_alloc.pNext = NULL;//自定义数据的指针
    mem_alloc.allocationSize = 0;//分配的内存字节数
    mem_alloc.memoryTypeIndex = 0;//内存的类型索引

    VkImageViewCreateInfo view_info = {};//构建深度图像视图创建信息结构体实例
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;//设置结构体类型
    view_info.pNext = NULL;//自定义数据的指针
    view_info.image = VK_NULL_HANDLE;//对应的图像
    view_info.format = depthFormat;//图像视图的格式
    view_info.components.r = VK_COMPONENT_SWIZZLE_R;//设置R通道调和
    view_info.components.g = VK_COMPONENT_SWIZZLE_G;//设置G通道调和
    view_info.components.b = VK_COMPONENT_SWIZZLE_B;//设置B通道调和
    view_info.components.a = VK_COMPONENT_SWIZZLE_A;//设置A通道调和
    view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;//图像视图使用方面
    view_info.subresourceRange.baseMipLevel = 0;//基础Mipmap级别
    view_info.subresourceRange.levelCount = 1;//Mipmap级别的数量
    view_info.subresourceRange.baseArrayLayer = 0;//基础数组层
    view_info.subresourceRange.layerCount = 1;//数组层的数量
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;//图像视图的类型
    view_info.flags = 0;//标志

    VkResult result = vk::vkCreateImage(device, &image_info, NULL, &depthImage);//创建深度图像
    assert(result == VK_SUCCESS);

    VkMemoryRequirements mem_reqs;//获取图像内存需求
    vk::vkGetImageMemoryRequirements(device, depthImage, &mem_reqs);
    mem_alloc.allocationSize = mem_reqs.size;//获取所需内存字节数
    VkFlags requirements_mask = 0;//需要的内存类型掩码
    bool flag = memoryTypeFromProperties(memoryroperties, mem_reqs.memoryTypeBits,
                                         requirements_mask, &mem_alloc.memoryTypeIndex);//获取所需内存类型索引
    assert(flag);//检查获取是否成功
    LOGE("确定内存类型成功 类型索引为%d", mem_alloc.memoryTypeIndex);
    result = vk::vkAllocateMemory(device, &mem_alloc, NULL, &memDepth);    //分配内存
    assert(result == VK_SUCCESS);
    result = vk::vkBindImageMemory(device, depthImage, memDepth, 0);//绑定图像和内存
    assert(result == VK_SUCCESS);
    view_info.image = depthImage;//指定图像视图对应图像
    result = vk::vkCreateImageView(device, &view_info, NULL, &depthImageView);//创建深度图像视图
    assert(result == VK_SUCCESS);

}

//销毁深度缓冲相关
void MyVulkanManager::destroy_vulkan_DepthBuffer() {
    vk::vkDestroyImageView(device, depthImageView, NULL);
    vk::vkDestroyImage(device, depthImage, NULL);
    vk::vkFreeMemory(device, memDepth, NULL);
    LOGE("销毁深度缓冲相关成功!");
}

//创建渲染通道
void MyVulkanManager::create_render_pass() {
    VkSemaphoreCreateInfo imageAcquiredSemaphoreCreateInfo;        //构建信号量创建信息结构体实例
    imageAcquiredSemaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;//结构体类型
    imageAcquiredSemaphoreCreateInfo.pNext = NULL;//自定义数据的指针
    imageAcquiredSemaphoreCreateInfo.flags = 0;//供将来使用的标志
    VkResult result = vk::vkCreateSemaphore(device, &imageAcquiredSemaphoreCreateInfo, NULL,
                                            &imageAcquiredSemaphore);//创建信号量
    assert(result == VK_SUCCESS);//检测信号量是否创建成功

    VkAttachmentDescription attachments[2];//附件描述信息数组
    attachments[0].format = formats[0];//设置颜色附件的格式
    attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;//设置采样模式
    attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;//加载时对附件的操作
    attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;//存储时对附件的操作
    attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;//模板加载时对附件的操作
    attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;//模板存储时对附件的操作
    attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;//初始的布局
    attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;//结束时的最终布局
    attachments[0].flags = 0;//设置位掩码
    attachments[1].format = depthFormat;//设置深度附件的格式
    attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;//设置采样模式
    attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;//加载时对附件的操作
    attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;//存储时对附件的操作
    attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;//模板加载时对附件的操作
    attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;//模板存储时对附件的操作
    attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;    //初始的布局
    attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;//结束时的布局
    attachments[1].flags = 0;//设置位掩码

    VkAttachmentReference color_reference = {};//颜色附件引用
    color_reference.attachment = 0;//对应附件描述信息数组下标
    color_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;//设置附件布局

    VkAttachmentReference depth_reference = {};//深度附件引用
    depth_reference.attachment = 1;//对应附件描述信息数组下标
    depth_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;//设置附件布局

    VkSubpassDescription subpass = {};//构建渲染子通道描述结构体实例
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;//设置管线绑定点
    subpass.flags = 0;//设置掩码
    subpass.inputAttachmentCount = 0;//输入附件数量
    subpass.pInputAttachments = NULL;//输入附件列表
    subpass.colorAttachmentCount = 1;//颜色附件数量
    subpass.pColorAttachments = &color_reference;//颜色附件列表
    subpass.pResolveAttachments = NULL;//Resolve附件
    subpass.pDepthStencilAttachment = &depth_reference;//深度模板附件
    subpass.preserveAttachmentCount = 0;//preserve附件数量
    subpass.pPreserveAttachments = NULL;//preserve附件列表

    VkRenderPassCreateInfo rp_info = {};//构建渲染通道创建信息结构体实例
    rp_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;//结构体类型
    rp_info.pNext = NULL;//自定义数据的指针
    rp_info.attachmentCount = 2;//附件的数量
    rp_info.pAttachments = attachments;//附件列表
    rp_info.subpassCount = 1;//渲染子通道数量
    rp_info.pSubpasses = &subpass;//渲染子通道列表
    rp_info.dependencyCount = 0;//子通道依赖数量
    rp_info.pDependencies = NULL;//子通道依赖列表

    result = vk::vkCreateRenderPass(device, &rp_info, NULL, &renderPass);//创建渲染通道
    assert(result == VK_SUCCESS);

    clear_values[0].color.float32[0] = 0.2f;//帧缓冲清除用R分量值
    clear_values[0].color.float32[1] = 0.2f;//帧缓冲清除用G分量值
    clear_values[0].color.float32[2] = 0.2f;//帧缓冲清除用B分量值
    clear_values[0].color.float32[3] = 0.2f;//帧缓冲清除用A分量值
    clear_values[1].depthStencil.depth = 1.0f;//帧缓冲清除用深度值
    clear_values[1].depthStencil.stencil = 0;//帧缓冲清除用模板值

    rp_begin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;//渲染通道启动信息结构体类型
    rp_begin.pNext = NULL;//自定义数据的指针
    rp_begin.renderPass = renderPass;//指定要启动的渲染通道
    rp_begin.renderArea.offset.x = 0;//渲染区域起始X坐标
    rp_begin.renderArea.offset.y = 0;//渲染区域起始Y坐标
    rp_begin.renderArea.extent.width = screenWidth;//渲染区域宽度
    rp_begin.renderArea.extent.height = screenHeight;//渲染区域高度
    rp_begin.clearValueCount = 2;//帧缓冲清除值数量
    rp_begin.pClearValues = clear_values;//帧缓冲清除值数组

}

void MyVulkanManager::destroy_render_pass()//销毁渲染通道相关
{
    vk::vkDestroyRenderPass(device, renderPass, NULL);
    vk::vkDestroySemaphore(device, imageAcquiredSemaphore, NULL);
}

//获取设备中支持图形工作的队列
void MyVulkanManager::init_queue() {
    //获取指定家族中索引为0的队列
    vk::vkGetDeviceQueue(device, queueGraphicsFamilyIndex, 0, &queueGraphics);
}

//创建帧缓冲
void MyVulkanManager::create_frame_buffer() {
    VkImageView attachments[2];//附件图像视图数组
    attachments[1] = depthImageView;//给定深度图像视图
    VkFramebufferCreateInfo fb_info = {};//构建帧缓冲创建信息结构体实例
    fb_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;//结构体类型
    fb_info.pNext = NULL;//自定义数据的指针
    fb_info.renderPass = renderPass;//指定渲染通道
    fb_info.attachmentCount = 2;//附件数量
    fb_info.pAttachments = attachments;//附件图像视图数组
    fb_info.width = screenWidth;//宽度
    fb_info.height = screenHeight;//高度
    fb_info.layers = 1;//层数
    uint32_t i;//循环控制变量
    framebuffers = (VkFramebuffer *) malloc(
            swapchainImageCount * sizeof(VkFramebuffer));//为帧缓冲序列动态分配内存
    assert(framebuffers);//检查内存分配是否成功
    for (i = 0; i < swapchainImageCount; i++) //遍历交换链中的各个图像
    {
        attachments[0] = swapchainImageViews[i];//给定颜色附件对应图像视图
        VkResult result = vk::vkCreateFramebuffer(device, &fb_info, NULL, &framebuffers[i]);//创建帧缓冲
        assert(result == VK_SUCCESS);//检查是否创建成功
        LOGE("[创建帧缓冲%d成功！]", i);
    }
}

//销毁帧缓冲
void MyVulkanManager::destroy_frame_buffer() {
    //循环销毁交换链中各个图像对应的帧缓冲
    for (int i = 0; i < swapchainImageCount; i++) {
        vk::vkDestroyFramebuffer(device, framebuffers[i], NULL);
    }
    free(framebuffers);
    LOGE("销毁帧缓冲成功！");
}

//创建绘制用物体
void MyVulkanManager::createDrawableObject() {
    TriangleData::genVertexData();//生成三色三角形顶点数据和颜色数据
    triForDraw = new DrawableObjectCommonLight(TriangleData::vdata, TriangleData::dataByteCount,
                                               TriangleData::vCount, device,
                                               memoryroperties);//创建绘制用三色三角形对象
}

//销毁绘制用物体
void MyVulkanManager::destroyDrawableObject() {
    delete triForDraw;
}

void MyVulkanManager::createFence() {//创建用于等待指定任务执行完毕的栅栏
    VkFenceCreateInfo fenceInfo;//栅栏创建信息结构体实例
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;//结构体类型
    fenceInfo.pNext = NULL;//自定义数据的指针
    fenceInfo.flags = 0;//供将来使用的标志位
    vk::vkCreateFence(device, &fenceInfo, NULL, &taskFinishFence);//创建栅栏
}


void MyVulkanManager::destroyFence() {
    vk::vkDestroyFence(device, taskFinishFence, NULL);
}

void MyVulkanManager::initPresentInfo() {
    present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;    //结构体类型
    present.pNext = NULL;//自定义数据的指针
    present.swapchainCount = 1;//交换链的数量
    present.pSwapchains = &swapChain;//交换链列表
    present.waitSemaphoreCount = 0;//等待的信号量数量
    present.pWaitSemaphores = NULL;//等待的信号量列表
    present.pResults = NULL;//呈现操作结果标志列表
}

void MyVulkanManager::initMatrix() {
    MatrixState3D::setCamera(0, 0, 200, 0, 0, 0, 0, 1, 0);//初始化摄像机
    MatrixState3D::setInitStack();//初始化基本变换矩阵
    float ratio = (float) screenWidth / (float) screenHeight;//求屏幕长宽比
    MatrixState3D::setProjectFrustum(-ratio, ratio, -1, 1, 1.5f, 1000);//设置投影参数
}

void MyVulkanManager::flushUniformBuffer()//将当前帧相关数据送入一致变量缓冲
{
    xAngle = xAngle + 1.0f;//改变三色三角形旋转角
    if (xAngle >= 360) { xAngle = 0; }//限制三色三角形旋转角范围
    MatrixState3D::pushMatrix();//保护现场
    MatrixState3D::rotate(xAngle, 1, 0, 0);//旋转变换
    float *vertexUniformData = MatrixState3D::getFinalMatrix();//获取最终变换矩阵
    MatrixState3D::popMatrix();//恢复现场
    uint8_t *pData;//CPU访问时的辅助指针
    VkResult result = vk::vkMapMemory(device, sqsCL->memUniformBuf, 0, sqsCL->bufferByteCount, 0,
                                      (void **) &pData);//将设备内存映射为CPU可访问
    assert(result == VK_SUCCESS);
    memcpy(pData, vertexUniformData, sqsCL->bufferByteCount);//将最终矩阵数据拷贝进显存
    vk::vkUnmapMemory(device, sqsCL->memUniformBuf);    //解除内存映射

}

void MyVulkanManager::flushTexToDesSet()//更新绘制用描述集的方法
{
    sqsCL->writes[0].dstSet = sqsCL->descSet[0];//更新描述集对应的写入属性
    vk::vkUpdateDescriptorSets(device, 1, sqsCL->writes, 0, NULL);//更新描述集
}

void MyVulkanManager::drawObject() {
    FPSUtil::init();//初始化FPS计算
    while (MyVulkanManager::loopDrawFlag) {//每循环一次绘制一帧画面
        FPSUtil::calFPS();//计算FPS
        FPSUtil::before();//一帧开始

        VkResult result = vk::vkAcquireNextImageKHR(device, swapChain, UINT64_MAX,
                                                    imageAcquiredSemaphore, VK_NULL_HANDLE,
                                                    &currentBuffer);//获取交换链中的当前帧索引
        rp_begin.framebuffer = framebuffers[currentBuffer];    //为渲染通道设置当前帧缓冲
        vk::vkResetCommandBuffer(cmdBuffer, 0);//恢复命令缓冲到初始状态
        result = vk::vkBeginCommandBuffer(cmdBuffer, &cmd_buf_info);//启动命令缓冲

        MyVulkanManager::flushUniformBuffer();//将当前帧相关数据送入一致变量缓冲
        MyVulkanManager::flushTexToDesSet();//更新绘制用描述集

        vk::vkCmdBeginRenderPass(cmdBuffer, &rp_begin, VK_SUBPASS_CONTENTS_INLINE);//启动渲染通道
        triForDraw->drawSelf(cmdBuffer, sqsCL->pipelineLayout, sqsCL->pipeline,
                             &(sqsCL->descSet[0]));    //绘制三色三角形
        vk::vkCmdEndRenderPass(cmdBuffer);//结束渲染通道
        result = vk::vkEndCommandBuffer(cmdBuffer);//结束命令缓冲

        submit_info[0].waitSemaphoreCount = 1;//等待的信号量数量
        submit_info[0].pWaitSemaphores = &imageAcquiredSemaphore;//等待的信号量列表

        result = vk::vkQueueSubmit(queueGraphics, 1, submit_info, taskFinishFence);//提交命令缓冲
        do {    //等待渲染完毕
            result = vk::vkWaitForFences(device, 1, &taskFinishFence, VK_TRUE, FENCE_TIMEOUT);
        } while (result == VK_TIMEOUT);
        vk::vkResetFences(device, 1, &taskFinishFence);//重置栅栏
        present.pImageIndices = &currentBuffer;//指定此次呈现的交换链图像索引
        result = vk::vkQueuePresentKHR(queueGraphics, &present);//执行呈现
        FPSUtil::after(60);//限制FPS不超过指定的值
    }
}

//执行绘制
void MyVulkanManager::doVulkan() {
    ThreadTask *tt = new ThreadTask();
    thread t1(&ThreadTask::doTask, tt);
    t1.detach();
}

void MyVulkanManager::initPipeline()//初始化管线的方法
{
    sqsCL = new ShaderQueueSuit_Common(&device, renderPass, memoryroperties);//创建封装了渲染管线相关的对象
}

void MyVulkanManager::destroyPipeline()//销毁管线
{
    delete sqsCL;
}