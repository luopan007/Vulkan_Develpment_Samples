#ifndef VULKANEXBASE_MYVULKANMANAGER_H
#define VULKANEXBASE_MYVULKANMANAGER_H
#include <android_native_app_glue.h>
#include <vector>
#include <vulkan/vulkan.h>
#include "../vksysutil/vulkan_wrapper.h"
#include <vector>
#include <ObjObject.h>
#include <Dashboard2DObject.h>
#include "mylog.h"
#include "ShaderQueueSuit_Common.h"
#include "ShaderQueueSuit_Dashboard2D.h"
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
    static VkFormat depthFormat;
    static VkFormatProperties depthFormatProps;	
    static VkImage depthImage;
    static VkPhysicalDeviceMemoryProperties memoryroperties;
    static VkDeviceMemory memDepth;	
    static VkImageView depthImageView;
    static VkSemaphore imageAcquiredSemaphore;
    static uint32_t currentBuffer;
    static VkRenderPass renderPass;
    static VkClearValue clear_values[2];
    static VkRenderPassBeginInfo rp_begin;
    static VkFence taskFinishFence;
    static VkPresentInfoKHR present;
    static VkFramebuffer* framebuffers;
    static ShaderQueueSuit_Common* sqsCL;
    static ShaderQueueSuit_Dashboard2D* sqsD2D;
    static ObjObject* objObject;
    static Dashboard2DObject* d2dA;
    static void init_vulkan_instance();
    static void enumerate_vulkan_phy_devices();
    static void create_vulkan_devices();
    static void create_vulkan_CommandBuffer();
    static void create_vulkan_swapChain();
    static void create_vulkan_DepthBuffer();
    static void create_render_pass();
    static void init_queue();
    static void create_frame_buffer();
    static void createDrawableObject();
    static void drawObject();
    static void doVulkan();
    static void init_texture();
    static void initPipeline();
    static void createFence();
    static void initPresentInfo();
    static void initMatrix();
    static void destroyFence();
    static void destroyPipeline();
    static void destroyDrawableObject();
    static void destroyTexture();
    static void destroy_frame_buffer();
    static void destroy_render_pass();
    static void destroy_vulkan_DepthBuffer();
    static void destroy_vulkan_swapChain();
    static void destroy_vulkan_CommandBuffer();
    static void destroy_vulkan_devices();
    static void destroy_vulkan_instance();

    static VkFormat colorFormat; //作为颜色附件图像的格式
    static VkFormatProperties colorFormatProps; //物理设备支持的颜色格式属性
    static VkImage colorImage; //颜色附件图像
    static VkDeviceMemory memColor; //颜色附件图像对应的内存
    static VkImageView colorImageView; //颜色附件图像视图
    static VkDescriptorImageInfo colorImageInfo; //颜色附件图像信息
    static VkFramebuffer selfTexFramebuffer; //服务于绘制到纹理的专用帧缓冲
    static void drawSceneToTex(); //将场景绘制到纹理的方法
    static void drawSceneToScreen(); //将纹理绘制到屏幕的方法
    static void flushUniformBufferForToTex();//将场景绘制到纹理时使用的将一致变量数据送入缓冲的方法
    static void flushTexToDesSetForToTex();//将场景绘制到纹理时使用的将纹理等数据与描述集关联的方法
    static void flushUniformBufferForToScreen(); //将纹理绘制到屏幕时使用的将一致变量数据送入缓冲的方法
    static void flushTexToDesSetForToScreen();//将纹理绘制到屏幕时使用的将纹理等数据与描述集关联的方法
    static void create_vulkan_SelfColorBuffer(); //创建绘制到纹理用颜色缓冲的方法
    static void destroy_vulkan_SelfColorBuffer(); //销毁绘制到纹理用颜色缓冲相关的方法
};
#endif 
