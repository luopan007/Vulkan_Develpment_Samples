#include "TextureManager.h"
#include "../bndev/mylog.h"
#include "FileUtil.h"
#include "HelpFunction.h"
#include <math.h>
#include <assert.h>
void setImageLayout(VkCommandBuffer cmd, VkImage image,
                    VkImageAspectFlags aspectMask,
                    VkImageLayout old_image_layout,
                    VkImageLayout new_image_layout)
{
    VkImageMemoryBarrier image_memory_barrier = {};
    image_memory_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    image_memory_barrier.pNext = NULL;
    image_memory_barrier.srcAccessMask = 0;
    image_memory_barrier.dstAccessMask = 0;
    image_memory_barrier.oldLayout = old_image_layout;
    image_memory_barrier.newLayout = new_image_layout;
    image_memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    image_memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    image_memory_barrier.image = image;
    image_memory_barrier.subresourceRange.aspectMask = aspectMask;
    image_memory_barrier.subresourceRange.baseMipLevel = 0;
    image_memory_barrier.subresourceRange.levelCount = 1;
    image_memory_barrier.subresourceRange.baseArrayLayer = 0;
    image_memory_barrier.subresourceRange.layerCount = 1;
    if (old_image_layout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
        image_memory_barrier.srcAccessMask =
                VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    }
    if (new_image_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    }
    if (new_image_layout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) {
        image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    }
    if (old_image_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        image_memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    }
    if (old_image_layout == VK_IMAGE_LAYOUT_PREINITIALIZED) {
        image_memory_barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
    }
    if (new_image_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        image_memory_barrier.srcAccessMask =
                VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
        image_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    }
    if (new_image_layout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
        image_memory_barrier.dstAccessMask =
                VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    }
    if (new_image_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        image_memory_barrier.dstAccessMask =
                VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    }
    VkPipelineStageFlags src_stages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    VkPipelineStageFlags dest_stages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    vk::vkCmdPipelineBarrier(cmd, src_stages, dest_stages, 0, 0, NULL, 0, NULL,1, &image_memory_barrier);
}
std::vector<std::string> TextureManager::texNames={"texture/mipmap.bntex"};
std::vector<VkSampler> TextureManager::samplerList;
std::map<std::string,VkImage> TextureManager::textureImageList;
std::map<std::string,VkDeviceMemory> TextureManager::textureMemoryList;
std::map<std::string,VkImageView> TextureManager::viewTextureList;
std::map<std::string,VkDescriptorImageInfo> TextureManager::texImageInfoList;
void TextureManager::initSampler(VkDevice& device, VkPhysicalDevice& gpu)
{
    VkSamplerCreateInfo samplerCreateInfo = {};
    samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerCreateInfo.magFilter = VK_FILTER_NEAREST;
    samplerCreateInfo.minFilter = VK_FILTER_NEAREST;
    samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerCreateInfo.mipLodBias = 0.0;
    samplerCreateInfo.anisotropyEnable = VK_FALSE;
    samplerCreateInfo.maxAnisotropy = 1;
    samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
    samplerCreateInfo.minLod = 0.0;
    samplerCreateInfo.maxLod = 9.0;
    samplerCreateInfo.compareEnable = VK_FALSE;
    samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
    for (int i = 0; i<SAMPLER_COUNT; i++)
    {
        VkSampler samplerTexture;
        VkResult result = vk::vkCreateSampler(device, &samplerCreateInfo, NULL, &samplerTexture);
        assert(result == VK_SUCCESS);
        samplerList.push_back(samplerTexture);
    }
}
void TextureManager::init_SPEC_Textures_ForMipMap(std::string texName, VkDevice& device, VkPhysicalDevice& gpu, VkPhysicalDeviceMemoryProperties& memoryroperties, VkCommandBuffer& cmdBuffer, VkQueue& queueGraphics, VkFormat format, TexDataObject* ctdo, int levels)
{
    VkImageCreateInfo image_create_info = {};
    image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_create_info.pNext = NULL;
    image_create_info.imageType = VK_IMAGE_TYPE_2D;
    image_create_info.format = format;
    image_create_info.extent.width = ctdo->width;
    image_create_info.extent.height = ctdo->height;
    image_create_info.extent.depth = 1;
    image_create_info.mipLevels = levels;
    image_create_info.arrayLayers = 1;
    image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
    image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
    image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_create_info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    image_create_info.queueFamilyIndexCount = 0;
    image_create_info.pQueueFamilyIndices = NULL;
    image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    image_create_info.flags = 0;
    VkImage textureImage;
    VkResult result = vk::vkCreateImage(device, &image_create_info, NULL, &textureImage);
    assert(result == VK_SUCCESS);
    textureImageList[texName] = textureImage;
    VkMemoryAllocateInfo mem_alloc = {};
    mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    mem_alloc.pNext = NULL;
    mem_alloc.allocationSize = 0;
    mem_alloc.memoryTypeIndex = 0;
    VkMemoryRequirements mem_reqs;
    vk::vkGetImageMemoryRequirements(device, textureImage, &mem_reqs);
    mem_alloc.allocationSize = mem_reqs.size;
    printf("IMG mem_reqs.size %d", (int)(mem_reqs.size));
    bool flag = memoryTypeFromProperties(memoryroperties, mem_reqs.memoryTypeBits, 0, &mem_alloc.memoryTypeIndex);
    assert(flag);
    VkDeviceMemory textureMemory;
    result = vk::vkAllocateMemory(device, &mem_alloc, NULL, &(textureMemory));
    assert(result == VK_SUCCESS);
    textureMemoryList[texName] = textureMemory;
    result = vk::vkBindImageMemory(device, textureImage, textureMemory, 0);
    assert(result == VK_SUCCESS);
    /*创建缓冲，将纹理数据首先搞进缓冲，然后传输进纹理*/
    VkBuffer stagingBuffer;
    VkBufferCreateInfo bufferCreateInfo = {};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.pNext = NULL;
    bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    bufferCreateInfo.size = ctdo->dataByteCount;
    bufferCreateInfo.queueFamilyIndexCount = 0;
    bufferCreateInfo.pQueueFamilyIndices = NULL;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferCreateInfo.flags = 0;
    result = vk::vkCreateBuffer(device, &bufferCreateInfo, nullptr, &stagingBuffer);
    assert(result == VK_SUCCESS);
    VkMemoryRequirements memReqs = {};
    vk::vkGetBufferMemoryRequirements(device, stagingBuffer, &memReqs);
    VkMemoryAllocateInfo memAllocInfo = {};
    memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memAllocInfo.pNext = NULL;
    memAllocInfo.memoryTypeIndex = 0;
    memAllocInfo.allocationSize = memReqs.size;
    memoryTypeFromProperties(memoryroperties, memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &memAllocInfo.memoryTypeIndex);
    VkDeviceMemory stagingMemory;
    result = vk::vkAllocateMemory(device, &memAllocInfo, nullptr, &stagingMemory);
    assert(result == VK_SUCCESS);
    result = vk::vkBindBufferMemory(device, stagingBuffer, stagingMemory, 0);
    assert(result == VK_SUCCESS);
    uint8_t *pData;
    vk::vkMapMemory(device, stagingMemory, 0, memReqs.size, 0, (void**)(&pData));
    memcpy(pData, ctdo->data, memReqs.size);
    vk::vkUnmapMemory(device, stagingMemory);
    VkBufferImageCopy bufferCopyRegion = {};
    bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    bufferCopyRegion.imageSubresource.mipLevel = 0;
    bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
    bufferCopyRegion.imageSubresource.layerCount = 1;
    bufferCopyRegion.imageExtent.width = ctdo->width;
    bufferCopyRegion.imageExtent.height = ctdo->height;
    bufferCopyRegion.imageExtent.depth = 1;
    bufferCopyRegion.bufferOffset = 0;
    VkCommandBufferBeginInfo cmd_buf_info = {};
    cmd_buf_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmd_buf_info.pNext = NULL;
    cmd_buf_info.flags = 0;
    cmd_buf_info.pInheritanceInfo = NULL;
    const VkCommandBuffer cmd_bufs[] = { cmdBuffer };
    VkSubmitInfo submit_info[1] = {};
    submit_info[0].pNext = NULL;
    submit_info[0].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info[0].waitSemaphoreCount = 0;
    submit_info[0].pWaitSemaphores = VK_NULL_HANDLE;
    submit_info[0].pWaitDstStageMask = VK_NULL_HANDLE;
    submit_info[0].commandBufferCount = 1;
    submit_info[0].pCommandBuffers = cmd_bufs;
    submit_info[0].signalSemaphoreCount = 0;
    submit_info[0].pSignalSemaphores = NULL;
    VkFenceCreateInfo fenceInfo;
    VkFence copyFence;
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.pNext = NULL;
    fenceInfo.flags = 0;
    vk::vkCreateFence(device, &fenceInfo, NULL, &copyFence);
    vk::vkResetCommandBuffer(cmdBuffer, 0);
    result = vk::vkBeginCommandBuffer(cmdBuffer, &cmd_buf_info);
    setImageLayout(cmdBuffer, textureImage, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    vk::vkCmdCopyBufferToImage(cmdBuffer, stagingBuffer, textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferCopyRegion);
    setImageLayout(cmdBuffer, textureImage, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    for (int32_t i = 1; i < levels; i++){ //遍历所有 mipmap 级数
        VkImageBlit imageBlit{}; //创建图像 blit 实例
        imageBlit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; //使用方面
        imageBlit.srcSubresource.layerCount = 1; //源资源的层数量
        imageBlit.srcSubresource.mipLevel = i - 1; //源资源的 mipmap 级别
        imageBlit.srcOffsets[1].x = int32_t(ctdo->width >> (i - 1)); //源资源的 x 偏移量
        imageBlit.srcOffsets[1].y = int32_t(ctdo->height >> (i - 1)); //源资源的 y 偏移量
        imageBlit.srcOffsets[1].z = 1; //源资源的 z 偏移量
        imageBlit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; //使用方面
        imageBlit.dstSubresource.layerCount = 1; //目标资源的层数量
        imageBlit.dstSubresource.mipLevel = i; //目标资源的 mipmap 级别
        imageBlit.dstOffsets[1].x = int32_t(ctdo->width >> i); //目标资源的 x 偏移量
        imageBlit.dstOffsets[1].y = int32_t(ctdo->height >> i); //目标资源的 y 偏移量
        imageBlit.dstOffsets[1].z = 1; //目标资源的 z 偏移量
        vk::vkCmdBlitImage( //从源资源生成目标资源（即通过上一层纹理数据生成下一层纹理数据）
                cmdBuffer, //命令缓冲
                textureImage, //源图像
                VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,//源图像瓦片组织方式
                textureImage, //目标图像
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,//目标图像瓦片组织方式
                1, //图像 blit 实例数量
                &imageBlit, //图像 blit 实例列表
                VK_FILTER_LINEAR); //要应用的纹理采样过滤器
    }
    result = vk::vkEndCommandBuffer(cmdBuffer); //结束命令缓冲（停止记录命令）
    result = vk::vkQueueSubmit(queueGraphics, 1, submit_info, copyFence); //提交给队列执行
    do
    {
        result = vk::vkWaitForFences(device, 1, &copyFence, VK_TRUE, 100000000);
    } while (result == VK_TIMEOUT);
    vk::vkDestroyBuffer(device, stagingBuffer, NULL);
    vk::vkFreeMemory(device, stagingMemory, NULL);
    vk::vkDestroyFence(device, copyFence, NULL);
    VkImageViewCreateInfo view_info = {};
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.pNext = NULL;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.format = VK_FORMAT_R8G8B8A8_UNORM;
    view_info.components.r = VK_COMPONENT_SWIZZLE_R;
    view_info.components.g = VK_COMPONENT_SWIZZLE_G;
    view_info.components.b = VK_COMPONENT_SWIZZLE_B;
    view_info.components.a = VK_COMPONENT_SWIZZLE_A;
    view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.levelCount = levels;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount = 1;
    view_info.image = textureImageList[texName];
    VkImageView viewTexture;
    result = vk::vkCreateImageView(device, &view_info, NULL, &viewTexture);
    viewTextureList[texName] = viewTexture;
    VkDescriptorImageInfo texImageInfo;
    texImageInfo.imageView = viewTexture;
    texImageInfo.sampler = samplerList[0];
    texImageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
    texImageInfoList[texName] = texImageInfo;
    delete ctdo;
}
void TextureManager::initTextures(VkDevice& device, VkPhysicalDevice& gpu, VkPhysicalDeviceMemoryProperties& memoryroperties,VkCommandBuffer& cmdBuffer, VkQueue& queueGraphics){//加载所有纹理的方法
    initSampler(device,gpu); //初始化采样器
    VkFormatProperties formatProps; //指定格式纹理的格式属性
    vk::vkGetPhysicalDeviceFormatProperties(gpu,VK_FORMAT_R8G8B8A8_UNORM, &formatProps); //获取指定格式纹理的格式属性
    assert(formatProps.optimalTilingFeatures & //检查是否支持生成 mipmap 所需的 BLIT 类型的源和目标
           VK_FORMAT_FEATURE_BLIT_SRC_BIT);
    assert(formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT);
    for (int i = 0; i<texNames.size(); i++){ //遍历纹理文件名称列表
        TexDataObject* ctdo = FileUtil::loadCommonTexData(texNames[i]);//加载纹理文件数据
        int levels = floor(log2(max(ctdo->width, ctdo->height))) + 1; //计算 mipmap 层次数
        LOGE("%s w %d h %d L %d", texNames[i].c_str(), ctdo->width, ctdo->height, levels); //打印信息
        init_SPEC_Textures_ForMipMap(texNames[i], device, gpu, memoryroperties, //加载 2D 纹理
                                     cmdBuffer, queueGraphics, VK_FORMAT_R8G8B8A8_UNORM, ctdo, levels);
    }}
void TextureManager::deatroyTextures(VkDevice& device)
{
    for(int i=0;i<SAMPLER_COUNT;i++)
    {
        vk::vkDestroySampler(device, samplerList[i], NULL);
    }
    for(int i=0;i<texNames.size();i++)
    {
        vk::vkDestroyImageView(device, viewTextureList[texNames[i]], NULL);
        vk::vkDestroyImage(device, textureImageList[texNames[i]], NULL);
        vk::vkFreeMemory(device, textureMemoryList[texNames[i]], NULL);
    }
}
int TextureManager::getVkDescriptorSetIndex(std::string texName)
{
    int result=-1;
    for(int i=0;i<texNames.size();i++)
    {
        if(texNames[i].compare(texName.c_str())==0)
        {
            result=i;
            break;
        }
    }
    assert(result!=-1);
    return result;
}