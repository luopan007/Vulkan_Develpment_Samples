#ifndef _BNMODEL_H
#define _BNMODEL_H
#include "Bnggdh.h"
#include "BnggdhDraw.h"
class BNModel {
private:
    float time = 0;
    float onceTime;
    float interval = 2.0f;
    float dt;
    float dtFactor;
    string picName;
public:
    BnggdhDraw* cd;
	/*
	* @param sourceName		模型名称
	* @param picName		图片名称
	* @param dtFactor		速率，范围在0-1
	*/
    BNModel(string sourceName, string picName, float dtFactor,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties);
    ~BNModel();
	void update();
	void arrange();
	void copy();
    void draw(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline);
    float getDtFactor();
    void setDtFactor(float dtFactor);
    void setTime(float time);
    float getTime();
    float getOnceTime();
    float* getMatrix(string id);
};
#endif
