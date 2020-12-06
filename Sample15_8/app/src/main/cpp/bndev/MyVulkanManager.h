#ifndef VULKANEXBASE_MYVULKANMANAGER_H
#define VULKANEXBASE_MYVULKANMANAGER_H
#include <android_native_app_glue.h>
#include <vector>
#include <vulkan/vulkan.h>
#include "../vksysutil/vulkan_wrapper.h"
#include <vector>
#include <CommonObject.h>
#include <Dashboard2DObject.h>
#include "mylog.h"
#include "ShaderQueueSuit_Common.h"
#include "ShaderQueueSuit_Dashboard2D.h"
#include "ShaderQueueSuit_SSZ.h"
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
    static ShaderQueueSuit_SSZ* sqsCLSSZ;
    static ShaderQueueSuit_Dashboard2D* sqsD2D;
    static CommonObject* objObject;
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
    static VkFormat colorFormat; 
    static VkFormatProperties colorFormatProps; 
    static VkImage colorImage[2]; 
    static VkDeviceMemory memColor[2]; 
    static VkImageView colorImageView[2]; 
    static VkDescriptorImageInfo colorImageInfo[2]; 
    static VkFramebuffer selfTexFramebuffer[2]; 
    static void drawSceneToTexSpec(VkFramebuffer framebuffer, VkPipelineLayout& pipelineLayout, VkPipeline& pipeline, VkDescriptorSet* desSetPointer);
    static void drawSceneToTex();
    static void drawSceneToScreen(); 
    static void flushUniformBufferForToTex();
    static void flushTexToDesSetForToTex();
    static void flushTexToDesSetForToScreen();
    static void create_vulkan_SelfColorBufferSpec(int index);
    static void create_vulkan_SelfColorBuffer();
    static void destroy_vulkan_SelfColorBuffer(); 
};
#endif 
