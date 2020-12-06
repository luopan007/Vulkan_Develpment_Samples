#include "ThreadTask.h"
#include "MyVulkanManager.h"
#include "ShaderQueueSuit_CommonTexLight.h"
void ThreadTask::doTask()
{
    MyVulkanManager::init_vulkan_instance();
    MyVulkanManager::enumerate_vulkan_phy_devices();
    MyVulkanManager::create_vulkan_devices();
    MyVulkanManager::create_vulkan_CommandBuffer();
    MyVulkanManager::init_queue();
    MyVulkanManager::create_vulkan_swapChain();
    MyVulkanManager::create_vulkan_DepthBuffer();
    MyVulkanManager::create_vulkan_SelfColorBuffer();
    MyVulkanManager::create_render_pass_screen();
    MyVulkanManager::create_render_pass_self();
    MyVulkanManager::create_frame_buffer_screen();
    MyVulkanManager::create_frame_buffer_self();
    MyVulkanManager::init_texture();
    MyVulkanManager::createDrawableObject();
    MyVulkanManager::initPipeline();
    MyVulkanManager::createFence();
    MyVulkanManager::initPresentInfo();
    MyVulkanManager::initMatrixAndLight();
    MyVulkanManager::drawObject();
    MyVulkanManager::destroyPipeline();
    MyVulkanManager::destroyDrawableObject();
    MyVulkanManager::destroy_textures();
    MyVulkanManager::destroy_vulkan_SelfColorBuffer();
    MyVulkanManager::destroy_frame_buffer();
    MyVulkanManager::destroy_render_pass_self();
    MyVulkanManager::destroy_render_pass_screen();
    MyVulkanManager::destroy_vulkan_DepthBuffer();
    MyVulkanManager::destroy_vulkan_swapChain();
    MyVulkanManager::destroy_vulkan_CommandBuffer();
    MyVulkanManager::destroy_vulkan_devices();
    MyVulkanManager::destroy_vulkan_instance();
}
ThreadTask::ThreadTask()
{
}
ThreadTask:: ~ThreadTask()
{
}