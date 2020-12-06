#ifndef _BNMODEL_H
#define _BNMODEL_H
#include "Bnggdh.h"
#include "BnggdhDraw.h"
class BNModel {
private:
	float time = 0; //当前动画时刻
	float onceTime; //一次动画所需的总时间
	float interval = 2.0f; //一次动画和下一次动画的间隔时间
	float dt; //时间步长
	float dtFactor; //播放速率因子
	string picName; //纹理图名称
public:
	BnggdhDraw* cd; //指向对应的骨骼动画对象的指针
	BNModel(string sourceName, string picName, float dtFactor, //构造函数
			VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties);
	~BNModel(); //析构函数
	void update(); //用于更新动画的方法
	void arrange(); //用于拷贝顶点数据到绘制用内存的方法
	void copy(); //用于拷贝顶点数据到中间传输区的方法
	void draw(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline);
	float getDtFactor(); //获取播放速率因子的方法
	void setDtFactor(float dtFactor); //设置播放速率因子的方法
	void setTime(float time); //设置当前骨骼动画时间的方法
	float getTime(); //获取当前骨骼动画时间的方法
	float getOnceTime(); //获取一次动画所需总时间的方法
	float* getMatrix(string id); //获取指定骨骼变换矩阵的方法
};
#endif
