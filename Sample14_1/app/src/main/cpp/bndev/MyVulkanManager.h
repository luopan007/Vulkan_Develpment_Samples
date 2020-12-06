#ifndef VULKANEXBASE_MYVULKANMANAGER_H
#define VULKANEXBASE_MYVULKANMANAGER_H
#include <android_native_app_glue.h>
#include <vector>
#include <vulkan/vulkan.h>
#include "../vksysutil/vulkan_wrapper.h"
#include <vector>
#include "mylog.h"
#include "../util/ObjObject.h"
#include "../util/Dashboard2DObject.h"
#include "ShaderQueueSuit_CommonTexLight.h"
#include "ShaderQueueSuit_Dashboard2D.h"
#define OUT_TEX_COUNT 4//第一轮绘制时的输出数量
#define FENCE_TIMEOUT 100000000
class MyVulkanManager
{
  public:
    static android_app* Android_application;
    static bool loopDrawFlag;
    static std::vector<const char *> instanceExtensionNames;
    static VkInstance instance;
    static uint32_t gpuCount;
    static  std::vector<VkPhysicalDevice> gpus;
    static uint32_t queueFamilyCount;
    static std::vector<VkQueueFamilyProperties> queueFamilyprops;
    static uint32_t queueGraphicsFamilyIndex;
    static VkQueue queueGraphics;
    static uint32_t queuePresentFamilyIndex;
    static std::vector<const char *> deviceExtensionNames;
    static VkDevice device;
    static VkCommandPool cmdPool;
    static VkCommandBuffer cmdBuffer;
    static VkCommandBufferBeginInfo cmd_buf_info;
    static VkCommandBuffer cmd_bufs[1];
    static VkSubmitInfo submit_info[1];
    static uint32_t screenWidth;
    static uint32_t screenHeight;
    static float ratio;
    static VkSurfaceKHR surface;
    static std::vector<VkFormat> formats;
    static VkSurfaceCapabilitiesKHR surfCapabilities;
    static uint32_t presentModeCount;
    static std::vector<VkPresentModeKHR> presentModes;
    static VkExtent2D swapchainExtent;
    static VkSwapchainKHR swapChain;
    static uint32_t swapchainImageCount;
    static std::vector<VkImage> swapchainImages;
    static std::vector<VkImageView> swapchainImageViews;
    static VkPhysicalDeviceMemoryProperties memoryroperties;
    static VkFormat depthFormat;
    static VkFormatProperties depthFormatProps;
    static VkImage depthImage;
    static VkDeviceMemory memDepth;
    static VkImageView depthImageView;
    static VkFormat colorFormat;
    static VkFormatProperties colorFormatProps;
    static VkImage colorImage[4]; //颜色缓冲附件图像数组
    static VkDeviceMemory memColor[4]; //颜色缓冲附件图像内存数组
    static VkImageView colorImageView[4]; //颜色缓冲附件图像视图数组
    static VkDescriptorImageInfo colorImageInfo[4]; //颜色缓冲附件图像描述信息数组
    static VkSemaphore imageAcquiredSemaphore;
    static uint32_t currentBuffer;
    static VkRenderPass renderPassScreen;
    static VkRenderPass renderPassSelf;//渲染通道（服务于 MRT）
    static VkClearValue clear_values_screen[2];
    static VkClearValue clear_values_self[5];//渲染通道用清除帧缓冲的数据（服务于 MRT）
    static VkRenderPassBeginInfo rp_begin_screen;
    static VkRenderPassBeginInfo rp_begin_self;//渲染通道启动信息（服务于 MRT）
    static VkFence taskFinishFence;
    static VkPresentInfoKHR present;
    static VkFramebuffer* framebuffers;
    static VkFramebuffer selfTexFramebuffer;//服务于 MRT 的帧缓冲
    static ShaderQueueSuit_CommonTexLight* sqsCTL;
    static ShaderQueueSuit_Dashboard2D* sqsD2D;
    static ObjObject* CHForDraw;
    static ObjObject* carForDraw;
    static ObjObject* treeForDraw;
    static Dashboard2DObject* d2dA;
    static void init_vulkan_instance();
    static void destroy_vulkan_instance();
    static void enumerate_vulkan_phy_devices();
    static void create_vulkan_devices();
    static void destroy_vulkan_devices();
    static void create_vulkan_CommandBuffer();
    static void destroy_vulkan_CommandBuffer();
    static void create_vulkan_swapChain();
    static void destroy_vulkan_swapChain();
    static void create_vulkan_DepthBuffer();
    static void destroy_vulkan_DepthBuffer();
    static void create_vulkan_SelfColorBuffer();//创建颜色缓冲附件的方法（服务于 MRT）
    static void destroy_vulkan_SelfColorBuffer();//销毁颜色缓冲附件相关的方法（服务于 MRT）
    static void create_render_pass_screen();
    static void destroy_render_pass_screen();
    static void create_render_pass_self();//创建渲染通道的方法（服务于 MRT）
    static void destroy_render_pass_self();//销毁渲染通道的方法（服务于 MRT）
    static void init_queue();
    static void create_frame_buffer_screen();
    static void create_frame_buffer_self();
    static void destroy_frame_buffer();//销毁帧缓冲的方法
    static void createDrawableObject();
    static void destroyDrawableObject();
    static void drawSceneToTex();//将场景绘制到纹理的方法（第一轮 MRT）
    static void drawSceneToScreen();//将场景绘制到屏幕的方法（第二轮）
    static void drawObject();
    static void doVulkan();
    static void init_texture();
    static void destroy_textures();
    static void initPipeline();
    static void destroyPipeline();
    static void createFence();
    static void destroyFence();
    static void initPresentInfo();
    static void initMatrixAndLight();
    static void flushUniformBufferForToTex(); //将绘制场景到纹理使用的一致变量数据送入缓冲的方法
    static void flushTexToDesSetForToTex(); //将绘制场景到纹理时的纹理等数据与描述集关联的方法
    static void flushUniformBufferForToScreen(); //将绘制到屏幕使用的一致变量数据送入缓冲的方法
    static void flushTexToDesSetForToScreen(); //将绘制到屏幕时的纹理等数据与描述集关联的方法
};
#endif 
