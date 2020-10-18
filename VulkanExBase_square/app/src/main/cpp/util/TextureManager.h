#ifndef VULKANEXBASE_TEXTUREMANAGER_H
#define VULKANEXBASE_TEXTUREMANAGER_H

#include <vector>
#include <map>
#include <string>
#include <vulkan/vulkan.h>
#include "../vksysutil/vulkan_wrapper.h"
#include "TexDataObject.h"

#define SAMPLER_COUNT 1

class TextureManager
{
private:
    //总纹理文件名称列表
    static std::vector<std::string> texNames;
public:
    //为ShaderQueueSuit_CommonTexLight准备的纹理名称列表
    static std::vector<std::string> texNamesForCommonTexLight;
    //为ShaderQueueSuit_Dashboard2D准备的纹理名称列表
    static std::vector<std::string> texNamesForDashboard2D;
    //采样器列表
    static std::vector<VkSampler> samplerList;
    //纹理Image列表
    static std::map<std::string,VkImage> textureImageList;
    //纹理内存列表
    static std::map<std::string,VkDeviceMemory> textureMemoryList;

    //纹理ImageView列表
    static std::map<std::string,VkImageView> viewTextureList;
    //纹理描述信息列表
    static std::map<std::string,VkDescriptorImageInfo> texImageInfoList;

    //加载纹理初始化的方法
    static void initTextures(VkDevice& device,VkPhysicalDevice& gpu,VkPhysicalDeviceMemoryProperties& memoryroperties,VkCommandBuffer& cmdBuffer,VkQueue& queueGraphics);
    //销毁纹理的方法
    static void deatroyTextures(VkDevice& device);
    //通过纹理的名称获取对应VkDescriptorSet在列表中的索引
    static int getVkDescriptorSetIndexForCommonTexLight(std::string texName);
    static int getVkDescriptorSetIndexForDashboard2D(std::string texName);

private:
    //初始化采样器的方法
    static void initSampler(VkDevice& device,VkPhysicalDevice& gpu);
    //初始化PKM封装格式的RGBA8_ETC2_EAC压缩纹理的方法
    static void init_RGBA8_ETC2_EAC_Textures(std::string texName,VkDevice& device,VkPhysicalDevice& gpu,VkPhysicalDeviceMemoryProperties& memoryroperties,VkCommandBuffer& cmdBuffer,VkQueue& queueGraphics, VkFormat format, TexDataObject* ctdo);
};


#endif //VULKANEXBASE_TEXTUREMANAGER_H
