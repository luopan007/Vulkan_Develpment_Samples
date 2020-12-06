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
#include "LightManager.h"
#include "ShaderQueueSuit_CommonTexLight.h"
#include "ShaderQueueSuit_Dashboard2D.h"
#define OUT_TEX_COUNT 3
#define FENCE_TIMEOUT 100000000
class MyVulkanManager
{
  public:
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
    static VkRenderPass renderPassScreen;
    static VkClearValue clear_values_screen[2];
    static VkRenderPassBeginInfo rp_begin_screen;
    static VkFence taskFinishFence;
    static VkPresentInfoKHR present;
    static VkFramebuffer* framebuffers;
    static ShaderQueueSuit_CommonTexLight* sqsCTL;
    static ShaderQueueSuit_Dashboard2D* sqsD2D;
    static ObjObject* ptObjForDraw;
    static ObjObject* carBObjForDraw;
    static ObjObject* carWObjForDraw;
    static ObjObject* skyboxObjForDraw;
    static ObjObject* dxObjForDraw;
    static Dashboard2DObject* d2dA;
    static void init_vulkan_instance();
    static void enumerate_vulkan_phy_devices();
    static void create_vulkan_devices();
    static void create_vulkan_CommandBuffer();
    static void create_vulkan_swapChain();
    static void create_vulkan_DepthBuffer();
    static void create_render_pass_screen();
    static void init_queue();
    static void create_frame_buffer_screen();
    static void createDrawableObject();
    static void flushTexToDesSetForToTex();
    static void flushUniformBufferForToScreen();
    static void flushTexToDesSetForToScreen();
    static void drawSceneToTex();
    static void drawSceneToScreen();
    static void drawObject();
    static void doVulkan();
    static void init_texture();
    static void initPipeline();
    static void createFence();
    static void initPresentInfo();
    static void initMatrixAndLight();
    static void destroyFence();
    static void destroyPipeline();
    static void destroyDrawableObject();
    static void destroyTexture();
    static void destroy_frame_buffer();
    static void destroy_render_pass_screen();
    static void destroy_vulkan_DepthBuffer();
    static void destroy_vulkan_swapChain();
    static void destroy_vulkan_CommandBuffer();
    static void destroy_vulkan_devices();
    static void destroy_vulkan_instance();
    static VkImage colorImage[3];
    static VkDeviceMemory memColor[3];
    static VkImageView colorImageView[3];
    static VkDescriptorImageInfo colorImageInfo[3];
    static VkRenderPass renderPassSelf;
    static VkClearValue clear_values_self[4];
    static VkRenderPassBeginInfo rp_begin_self;
    static VkFramebuffer selfTexFramebuffer;
    static void create_vulkan_SelfColorBufferSpec(VkFormat colorFormat, int index);
    static void create_vulkan_SelfColorBuffer();
    static void destroy_vulkan_SelfColorBuffer();
    static void create_render_pass_self();
    static void destroy_render_pass_self();
    static void create_frame_buffer_self();
    static int ifSSAO ;
};
#endif 
