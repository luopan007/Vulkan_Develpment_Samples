#include "TextureManager.h"
#include "../bndev/mylog.h"
#include "FileUtil.h"
#include "HelpFunction.h"
#include <assert.h>


void setImageLayoutCube(VkCommandBuffer cmd, VkImage image,
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
    image_memory_barrier.subresourceRange.layerCount = 6;

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

void setImageLayout2D(VkCommandBuffer cmd, VkImage image,
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

    vk::vkCmdPipelineBarrier(cmd, src_stages, dest_stages, 0, 0, NULL, 0, NULL, 1, &image_memory_barrier);
}



size_t  splitString(const string& strSrc, const string& strDelims, vector<string>& strDest)
{
    string delims = strDelims;
    string STR;
    if(delims.empty()) delims = " **";

    string::size_type pos=0;
    string::size_type LEN = strSrc.size();
    while(pos < LEN ){
        STR="";
        while( (delims.find(strSrc[pos]) != std::string::npos) && (pos < LEN) )
        {
            ++pos;
        }
        if(pos==LEN) {
            return strDest.size();
        }
        while( (delims.find(strSrc[pos]) == std::string::npos) && (pos < LEN) )
        {
            STR += strSrc[pos++];
        }
        if( ! STR.empty() )
        {
            strDest.push_back(STR);
        }
    }
    return strDest.size();
}

std::vector<std::string> TextureManager::texNames={"texture/skycubemap_left.bntex","texture/skycubemap_right.bntex","texture/skycubemap_down.bntex","texture/skycubemap_up.bntex","texture/skycubemap_back.bntex","texture/skycubemap_front.bntex","texture/flare1.bntex","texture/flare2.bntex","texture/flare3.bntex"};
std::vector<std::string> TextureManager::texNamesForSkyBox={"texture/skycubemap_left.bntex","texture/skycubemap_right.bntex","texture/skycubemap_down.bntex","texture/skycubemap_up.bntex","texture/skycubemap_back.bntex","texture/skycubemap_front.bntex"};
std::vector<std::string> TextureManager::texNamesForGuang = { "texture/flare1.bntex","texture/flare2.bntex","texture/flare3.bntex"};

std::vector<VkSampler> TextureManager::samplerList;
std::map<std::string,VkImage> TextureManager::textureImageList;
std::map<std::string,VkDeviceMemory> TextureManager::textureMemoryList;
std::map<std::string,VkImageView> TextureManager::viewTextureList;
std::map<std::string,VkDescriptorImageInfo> TextureManager::texImageInfoList;

void TextureManager::initSampler(VkDevice& device,VkPhysicalDevice& gpu)
{

    VkSamplerCreateInfo samplerCreateInfo = {};
    samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
    samplerCreateInfo.minFilter = VK_FILTER_NEAREST;
    samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
    samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCreateInfo.mipLodBias = 0.0;
    samplerCreateInfo.anisotropyEnable = VK_FALSE;
    samplerCreateInfo.maxAnisotropy = 0;
    samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
    samplerCreateInfo.minLod = 0.0;
    samplerCreateInfo.maxLod = 0.0;
    samplerCreateInfo.compareEnable = VK_FALSE;
    samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;


    for(int i=0;i<SAMPLER_COUNT;i++)
    {

        VkSampler samplerTexture;

        VkResult result = vk::vkCreateSampler(device, &samplerCreateInfo, NULL, &samplerTexture);

        samplerList.push_back(samplerTexture);
    }
}

void TextureManager::init_SPEC_2D_Textures(std::string texName, VkDevice& device, VkPhysicalDevice& gpu, VkPhysicalDeviceMemoryProperties& memoryroperties, VkCommandBuffer& cmdBuffer, VkQueue& queueGraphics, VkFormat format, TexDataObject* ctdo)
{

    VkFormatProperties formatProps;
    vk::vkGetPhysicalDeviceFormatProperties(gpu, format, &formatProps);


    bool needStaging = (!(formatProps.linearTilingFeatures &VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT)) ? true : false;
    printf("\nTextureManager %s\n", (needStaging ? "不能使用线性瓦片纹理" : "能使用线性瓦片纹理"));

    if (needStaging)
    {

        VkBuffer tempBuf;

        VkBufferCreateInfo buf_info = {};
        buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        buf_info.pNext = NULL;
        buf_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        buf_info.size = ctdo->dataByteCount;
        buf_info.queueFamilyIndexCount = 0;
        buf_info.pQueueFamilyIndices = NULL;
        buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        buf_info.flags = 0;
        VkResult result = vk::vkCreateBuffer(device, &buf_info, NULL, &tempBuf);
        assert(result == VK_SUCCESS);

        VkMemoryRequirements mem_reqs;
        vk::vkGetBufferMemoryRequirements(device, tempBuf, &mem_reqs);
        assert(ctdo->dataByteCount <= mem_reqs.size);

        VkMemoryAllocateInfo alloc_info = {};
        alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        alloc_info.pNext = NULL;
        alloc_info.memoryTypeIndex = 0;
        alloc_info.allocationSize = mem_reqs.size;

        VkFlags requirements_mask = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        bool flag = memoryTypeFromProperties(memoryroperties, mem_reqs.memoryTypeBits, requirements_mask, &alloc_info.memoryTypeIndex);
        if (flag)
        {
            printf("确定内存类型成功 类型索引为%d", alloc_info.memoryTypeIndex);
        }
        else
        {
            printf("确定内存类型失败!");
        }


        VkDeviceMemory memTemp;
        result = vk::vkAllocateMemory(device, &alloc_info, NULL, &memTemp);
        assert(result == VK_SUCCESS);

        uint8_t *pData;
        result = vk::vkMapMemory(device, memTemp, 0, mem_reqs.size, 0, (void **)&pData);
        assert(result == VK_SUCCESS);

        memcpy(pData, ctdo->data, ctdo->dataByteCount);

        vk::vkUnmapMemory(device, memTemp);


        result = vk::vkBindBufferMemory(device, tempBuf, memTemp, 0);
        assert(result == VK_SUCCESS);


        VkImageCreateInfo image_create_info = {};
        image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        image_create_info.pNext = NULL;
        image_create_info.imageType = VK_IMAGE_TYPE_2D;
        image_create_info.format = format;
        image_create_info.extent.width = ctdo->width;
        image_create_info.extent.height = ctdo->height;
        image_create_info.extent.depth = 1;
        image_create_info.mipLevels = 1;
        image_create_info.arrayLayers = 1;
        image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
        image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
        image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        image_create_info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        image_create_info.queueFamilyIndexCount = 0;
        image_create_info.pQueueFamilyIndices = NULL;
        image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        image_create_info.flags = 0;


        VkImage textureImage;
        result = vk::vkCreateImage(device, &image_create_info, NULL, &textureImage);
        assert(result == VK_SUCCESS);
        textureImageList[texName] = textureImage;


        VkMemoryAllocateInfo mem_alloc = {};
        mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        mem_alloc.pNext = NULL;
        mem_alloc.allocationSize = 0;
        mem_alloc.memoryTypeIndex = 0;


        vk::vkGetImageMemoryRequirements(device, textureImage, &mem_reqs);
        mem_alloc.allocationSize = mem_reqs.size;

        flag = memoryTypeFromProperties(memoryroperties, mem_reqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &mem_alloc.memoryTypeIndex);

        VkDeviceMemory textureMemory;
        result = vk::vkAllocateMemory(device, &mem_alloc, NULL, &(textureMemory));
        textureMemoryList[texName] = textureMemory;

        result = vk::vkBindImageMemory(device, textureImage, textureMemory, 0);

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

        setImageLayout2D(cmdBuffer, textureImage, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        vk::vkCmdCopyBufferToImage(cmdBuffer, tempBuf, textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferCopyRegion);

        setImageLayout2D(cmdBuffer, textureImage, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        result = vk::vkEndCommandBuffer(cmdBuffer);

        result = vk::vkQueueSubmit(queueGraphics, 1, submit_info, copyFence);

        do
        {
            result = vk::vkWaitForFences(device, 1, &copyFence, VK_TRUE, 100000000);
        } while (result == VK_TIMEOUT);

        vk::vkDestroyBuffer(device, tempBuf, NULL);
        vk::vkFreeMemory(device, memTemp, NULL);
        vk::vkDestroyFence(device, copyFence, NULL);
    }
    else
    {

        VkImageCreateInfo image_create_info = {};
        image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        image_create_info.pNext = NULL;
        image_create_info.imageType = VK_IMAGE_TYPE_2D;
        image_create_info.format = format;
        image_create_info.extent.width = ctdo->width;
        image_create_info.extent.height = ctdo->height;
        image_create_info.extent.depth = 1;
        image_create_info.mipLevels = 1;
        image_create_info.arrayLayers = 1;
        image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
        image_create_info.tiling = VK_IMAGE_TILING_LINEAR;
        image_create_info.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
        image_create_info.usage = VK_IMAGE_USAGE_SAMPLED_BIT;
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

        bool flag = memoryTypeFromProperties(memoryroperties, mem_reqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &mem_alloc.memoryTypeIndex);

        VkDeviceMemory textureMemory;
        result = vk::vkAllocateMemory(device, &mem_alloc, NULL, &(textureMemory));
        textureMemoryList[texName] = textureMemory;

        result = vk::vkBindImageMemory(device, textureImage, textureMemory, 0);

        uint8_t *pData;
        vk::vkMapMemory(device, textureMemory, 0, mem_reqs.size, 0, (void**)(&pData));

        memcpy(pData, ctdo->data, mem_reqs.size);

        vk::vkUnmapMemory(device, textureMemory);
    }



    VkImageViewCreateInfo view_info = {};
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.pNext = NULL;
    view_info.image = VK_NULL_HANDLE;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.format = format;
    view_info.components.r = VK_COMPONENT_SWIZZLE_R;
    view_info.components.g = VK_COMPONENT_SWIZZLE_G;
    view_info.components.b = VK_COMPONENT_SWIZZLE_B;
    view_info.components.a = VK_COMPONENT_SWIZZLE_A;
    view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.levelCount = 1;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount = 1;
    view_info.image = textureImageList[texName];

    VkImageView viewTexture;
    VkResult result = vk::vkCreateImageView(device, &view_info, NULL, &viewTexture);
    viewTextureList[texName] = viewTexture;


    VkDescriptorImageInfo texImageInfo;
    texImageInfo.imageView = viewTexture;
    texImageInfo.sampler = samplerList[0];
    texImageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
    texImageInfoList[texName] = texImageInfo;


    delete ctdo;
}

void TextureManager::initCubemapTextures(std::string texName, VkDevice& device, VkPhysicalDevice& gpu, VkPhysicalDeviceMemoryProperties& memoryroperties, VkCommandBuffer& cmdBuffer, VkQueue& queueGraphics, VkFormat format, TexDataObject* ctdo)
{

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
    VkResult result = vk::vkCreateBuffer(device, &bufferCreateInfo, nullptr, &stagingBuffer);
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


    VkImageCreateInfo image_create_info = {};
    image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_create_info.pNext = NULL;
    image_create_info.imageType = VK_IMAGE_TYPE_2D;
    image_create_info.format = format;
    image_create_info.extent.width = ctdo->width;
    image_create_info.extent.height = ctdo->height;
    image_create_info.extent.depth = 1;
    image_create_info.mipLevels = 1;
    image_create_info.arrayLayers = 6;
    image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
    image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
    image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_create_info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    image_create_info.queueFamilyIndexCount = 0;
    image_create_info.pQueueFamilyIndices = NULL;
    image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    image_create_info.flags = 0;


    VkImage textureImage;
    result = vk::vkCreateImage(device, &image_create_info, NULL, &textureImage);
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

    bool flag = memoryTypeFromProperties(memoryroperties, mem_reqs.memoryTypeBits, 0, &mem_alloc.memoryTypeIndex);


    VkDeviceMemory textureMemory;
    result = vk::vkAllocateMemory(device, &mem_alloc, NULL, &(textureMemory));
    assert(result == VK_SUCCESS);
    textureMemoryList[texName] = textureMemory;


    result = vk::vkBindImageMemory(device, textureImage, textureMemory, 0);


    VkBufferImageCopy bufferCopyRegion = {};
    bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    bufferCopyRegion.imageSubresource.mipLevel = 0;
    bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
    bufferCopyRegion.imageSubresource.layerCount = 6;
    bufferCopyRegion.imageExtent.width = ctdo->width;
    bufferCopyRegion.imageExtent.height = ctdo->height;
    bufferCopyRegion.imageExtent.depth = 1;


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

    setImageLayoutCube(cmdBuffer, textureImage, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    vk::vkCmdCopyBufferToImage(cmdBuffer, stagingBuffer, textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferCopyRegion);

    setImageLayoutCube(cmdBuffer, textureImage, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    result = vk::vkEndCommandBuffer(cmdBuffer);

    result = vk::vkQueueSubmit(queueGraphics, 1, submit_info, copyFence);

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
    view_info.image = VK_NULL_HANDLE;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
    view_info.format = VK_FORMAT_R8G8B8A8_UNORM;
    view_info.components.r = VK_COMPONENT_SWIZZLE_R;
    view_info.components.g = VK_COMPONENT_SWIZZLE_G;
    view_info.components.b = VK_COMPONENT_SWIZZLE_B;
    view_info.components.a = VK_COMPONENT_SWIZZLE_A;
    view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.levelCount = 1;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount = 6;
    view_info.image = textureImageList[texName];

    VkImageView viewTexture;
    result = vk::vkCreateImageView(device, &view_info, NULL, &viewTexture);
    assert(result == VK_SUCCESS);
    viewTextureList[texName] = viewTexture;


    VkDescriptorImageInfo texImageInfo;
    texImageInfo.imageView = viewTexture;
    texImageInfo.sampler = samplerList[0];
    texImageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
    texImageInfoList[texName] = texImageInfo;


    delete ctdo;
}

void TextureManager::initTextures(VkDevice& device, VkPhysicalDevice& gpu, VkPhysicalDeviceMemoryProperties& memoryroperties, VkCommandBuffer& cmdBuffer, VkQueue& queueGraphics)
{
    initSampler(device, gpu);


    for (int i = 0; i<texNames.size(); i++)
    {
        std::string texName = texNames[i];
        std::vector<string> sa;
        splitString(texName, ".", sa);
        if (sa[1].compare("bntex") == 0)
        {
            TexDataObject* ctdo = FileUtil::loadCommonTexData(texName);
            init_SPEC_2D_Textures(texName, device, gpu, memoryroperties, cmdBuffer, queueGraphics, VK_FORMAT_R8G8B8A8_UNORM, ctdo);
        }
        else if (sa[1].compare("pkm") == 0)
        {
            TexDataObject* ctdo = FileUtil::load_RGBA8_ETC2_EAC_TexData(texName);
            init_SPEC_2D_Textures(texName, device, gpu, memoryroperties, cmdBuffer, queueGraphics, VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK, ctdo);
        }
        else if (sa[1].compare("bntexcube") == 0)
        {
            TexDataObject* ctdo = FileUtil::loadCubemapTexData(texName);
            printf("%s w %d h %d c %d", texName.c_str(), ctdo->width, ctdo->height, ctdo->dataByteCount);
            initCubemapTextures(texName, device, gpu, memoryroperties, cmdBuffer, queueGraphics, VK_FORMAT_R8G8B8A8_UNORM, ctdo);
        }
    }
}

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


int getVkDescriptorSetIndexGeneral(std::string texName,std::vector<std::string>& texNames)
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

int TextureManager::getVkDescriptorSetIndexForGuang(std::string texName)
{
    return  getVkDescriptorSetIndexGeneral(texName,texNamesForGuang);
}

int TextureManager::getVkDescriptorSetIndexForSkyBox(std::string texName)
{
    return  getVkDescriptorSetIndexGeneral(texName,texNamesForSkyBox);
}