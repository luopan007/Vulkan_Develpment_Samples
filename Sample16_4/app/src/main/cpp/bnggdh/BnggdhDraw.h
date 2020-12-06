#ifndef _BNGGDHDRAW_H
#define _BNGGDHDRAW_H    
#include <vulkan/vulkan.h>
#include <../util/MatrixState3D.h>
#include <../util/HelpFunction.h>
#include "Bnggdh.h"
class BnggdhDraw {
public:
	Bnggdh* bnggdh; //用于指向加载的bnggdh 文件对应对象的指针
	float maxKeyTime; //最大关键帧时间
	float* positionData; //指向顶点位置坐标数据数组首地址的指针
	float* textureData; //指向顶点纹理坐标数据数组首地址的指针
	float* pushConstantData; //指向推送常量数据首地址的指针
	VkDevice* devicePointer; //逻辑设备指针
	VkPhysicalDeviceMemoryProperties* //指向物理设备内存属性列表首地址的指针
			memoryPropertiesPointer;
	float* vDataTransfer; //指向中间传输区顶点数据数组首地址的指针
    float* vdata;
    int vCount;
    int vdataByteCount;
    VkBuffer vertexDatabuf;
    VkDeviceMemory vertexDataMem;
    VkMemoryRequirements vmem_reqs;
	VkDescriptorBufferInfo vertexDataBufferInfo;
    uint16_t* idata;
    int iCount;
	int idataByteCount;
    VkBuffer indexDatabuf;
    VkDeviceMemory indexDataMem;
    VkMemoryRequirements imem_reqs;
    VkDescriptorBufferInfo indexDataBufferInfo;
	BnggdhDraw(string sourceName, //构造函数
			   VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties);
	~BnggdhDraw(); //析构函数
	void initVertexData(); //初始化顶点数据的方法
	void initIndexData(); //初始化索引数据的方法
	void initVertexBuffer(); //创建顶点数据缓冲的方法
	void initIndexBuffer(); //创建索引数据缓冲的方法
	void referVertexBuffer(); //将顶点数据拷贝到绘制用内存的方法
	void drawSelf(string picName, //绘制方法
				  VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline);
	void updateData(float time); //更新动画的方法
	void copyData(); //拷贝顶点数据的方法
};
#endif
