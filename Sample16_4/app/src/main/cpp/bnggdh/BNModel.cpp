#include "BNModel.h"
BNModel::BNModel(string sourceName, string picName, float dtFactor, //构造函数
                 VkDevice& device, VkPhysicalDeviceMemoryProperties& memoryroperties) {
    cd=new BnggdhDraw(sourceName, device, memoryroperties);//创建用于绘制的BnggdhDraw 对象
    onceTime = cd->maxKeyTime; //获取一次动画总时间
    this->dtFactor = dtFactor; //接收设置的速率因子并保存
    this->dt = dtFactor * onceTime; //计算步长
    this->picName = picName; //接收模型的纹理图名称字符串并保存
}
void BNModel::update() {cd->updateData(time);} //调用方法更新骨骼动画到当前指定时间
void BNModel::arrange() {cd->referVertexBuffer();} //调用方法将顶点数据拷贝进绘制用内存
void BNModel::copy() {cd->copyData();} //调用方法拷贝新的顶点数据到中间传输区
void BNModel::draw(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline) {
    MatrixState3D::pushMatrix(); //保护现场
    cd->drawSelf(picName, cmd, pipelineLayout, pipeline); //调用drawSelf 方法执行绘制
    time += dt; //更新模型动画的当前时间
    if (time >= (onceTime + dt + interval)) { //判断当前播放时间是否大于总的动画时间
        time = 0; //将当前播放时间置0
    }
    MatrixState3D::popMatrix(); //恢复现场
}
float BNModel::getDtFactor() {return dtFactor;} //获取播放速率因子的方法
void BNModel::setDtFactor(float dtFactor) { //设置播放速率因子的方法
    if(dtFactor > 0 && dtFactor < 1){ //将速率值限定在0~1 范围内
        this->dtFactor = dtFactor; //设置速率因子值
        this->dt = dtFactor * onceTime; //计算步长
    }}
void BNModel::setTime(float time){ //设置当前骨骼动画时间的方法
    if(time >= 0 && time <= this->onceTime){ //判断时间是否在有意义的范围内
        this->time = time; //设置当前骨骼动画时间
    }}
float BNModel::getTime(){return this->time;} //获取当前骨骼动画时间的方法
float BNModel::getOnceTime(){return this->onceTime;} //获取一次动画所需总时间的方法
float* BNModel::getMatrix(string id){return cd->bnggdh->getMatrix(id);}//获取指定骨骼变换矩阵的方法
BNModel::~BNModel() {delete cd;} //析构函数