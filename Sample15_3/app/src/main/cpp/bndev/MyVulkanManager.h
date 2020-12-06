#ifndef VULKANEXBASE_MYVULKANMANAGER_H
#define VULKANEXBASE_MYVULKANMANAGER_H
#include <android_native_app_glue.h>
#include <vector>
#include <vulkan/vulkan.h>
#include "../vksysutil/vulkan_wrapper.h"
#include <vector>
#include "mylog.h"
#include "../util/ObjObject.h"
#include "TexDrawableObject.h"
#include "ShaderQueueSuit_CommonTexLight_Depth.h"
#include "ShaderQueueSuit_TexRect.h"
#include "ShaderQueueSuit_CommonTexLight.h"
#include "ShaderQueueSuit_XRay.h"
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
    static VkPhysicalDeviceMemoryProperties memoryroperties;
    static VkFormat depthFormat;
    static VkFormatProperties depthFormatProps;
    static VkImage depthImage;
    static VkDeviceMemory memDepth;
    static VkImageView depthImageView;
    static VkDescriptorImageInfo depthImageInfo;
    static VkFormat colorFormat[3];
    static VkFormatProperties colorFormatProps[3];          
    static VkImage colorImage[3];
    static VkDeviceMemory memColor[3];
    static VkImageView colorImageView[3];
    static VkDescriptorImageInfo colorImageInfo[3];
    static VkRenderPass renderPassSelf;
    static VkClearValue clear_values_self[3];
    static VkRenderPassBeginInfo rp_begin_self;
    static VkSemaphore imageAcquiredSemaphore;
    static uint32_t currentBuffer;
    static VkRenderPass renderPassScreen;
    static VkClearValue clear_values_screen[2];
    static VkRenderPassBeginInfo rp_begin_screen;
    static VkFence taskFinishFence;
    static VkPresentInfoKHR present;
    static VkFramebuffer* framebuffers;
    static VkFramebuffer selfTexFramebuffer;
    static VkFramebuffer selfTexFramebufferPeople;
    static ShaderQueueSuit_CommonTexLight_Depth* sqsCTLD;
    static ShaderQueueSuit_CommonTexLight* sqsCTL;
    static ShaderQueueSuit_XRay* sqsXRay;
    static ShaderQueueSuit_TexRect* sqsD2D;
    static float objScale;
    static float peopleScale;
    static float wallDis;
    static float cx;
    static float cy;
    static float cz;
    static float cxPrevious;
    static float cyPrevious;
    static float czPrevious;
    static ObjObject* floor;
    static ObjObject* wall;
    static ObjObject* people;
    static TexDrawableObject* d2dA;
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
    static void create_vulkan_SelfColorBufferSpec(VkFormat colorFormat, int index);
    static void create_vulkan_SelfColorBuffer();
    static void destroy_vulkan_SelfColorBuffer();
    static void create_render_pass_screen();
    static void destroy_render_pass_screen();
    static void create_render_pass_self();
    static void destroy_render_pass_self();
    static void init_queue();
    static void create_frame_buffer_screen();
    static void create_frame_buffer_self();
    static void destroy_frame_buffer();
    static void createDrawableObject();
    static void destroyDrawableObject();
    static void drawSceneToTex();
    static void drawPeopleToTex();
    static void drawSceneToScreen();
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
    static void flushUniformBufferForToTex();
    static void flushTexToDesSetForToTex();
    static void flushUniformBufferForToScreen();
    static void flushTexToDesSetForToScreen();
};
#endif 
