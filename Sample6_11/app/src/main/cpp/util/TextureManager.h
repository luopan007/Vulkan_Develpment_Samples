#ifndef VULKANEXBASE_TEXTUREMANAGER_H
#define VULKANEXBASE_TEXTUREMANAGER_H
#include <vector>
#include <map>
#include <string>
#include <vulkan/vulkan.h>
#include "../vksysutil/vulkan_wrapper.h"
#include "TexDataObject.h"
#define SAMPLER_COUNT 2
class TextureManager
{
public:
    static std::vector<std::string> texNames;
    static std::vector<VkSampler> samplerList;
    static std::map<std::string, VkImage> textureImageList;
    static std::map<std::string, VkDeviceMemory> textureMemoryList;
    static std::map<std::string, VkImageView> viewTextureList;
    static std::map<std::string, VkDescriptorImageInfo> texImageInfoList;
    static void initTextures(VkDevice& device, VkPhysicalDevice& gpu, VkPhysicalDeviceMemoryProperties& memoryroperties, VkCommandBuffer& cmdBuffer, VkQueue& queueGraphics);
    static void deatroyTextures(VkDevice& device);
    static int getVkDescriptorSetIndex(std::string texName);
private:
    static void initSampler(VkDevice& device, VkPhysicalDevice& gpu);
    static void init_SPEC_Textures_ForMipMap(std::string texName, VkDevice& device, VkPhysicalDevice& gpu, VkPhysicalDeviceMemoryProperties& memoryroperties, VkCommandBuffer& cmdBuffer, VkQueue& queueGraphics, VkFormat format, TexDataObject* ctdo, int levels,int samplerIndex);
};
#endif 
