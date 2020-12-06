#include "BNModel.h"
BNModel::BNModel(string sourceName, string picName, float dtFactor, VkDevice& device, VkPhysicalDeviceMemoryProperties& memoryroperties) {
	cd = new BnggdhDraw(sourceName, device, memoryroperties);
	onceTime = cd->maxKeyTime;
	this->dtFactor = dtFactor;
	this->dt = dtFactor * onceTime;
	this->picName = picName;
}
void BNModel::update()
{
	cd->updateData(time);
}
void BNModel::arrange()
{
	cd->referVertexBuffer();
}
void BNModel::copy()
{
	cd->copyData();
}
void BNModel::draw(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline) 
{	
    MatrixState3D::pushMatrix();
	cd->drawSelf(picName, cmd, pipelineLayout, pipeline);
	time += dt;
    if (time >= (onceTime + dt + interval)) {
        time = 0;
    }
    MatrixState3D::popMatrix();
}
float BNModel::getDtFactor() {
    return dtFactor;
}
void BNModel::setDtFactor(float dtFactor) {
    if(dtFactor > 0 && dtFactor < 1){
        this->dtFactor = dtFactor;
        this->dt = dtFactor * onceTime;
    }
}
void BNModel::setTime(float time){
    if(time >= 0 && time <= this->onceTime){
        this->time = time;
    }
}
float BNModel::getTime(){
    return this->time;
}
float BNModel::getOnceTime(){
    return this->onceTime;
}
float* BNModel::getMatrix(string id){
	return cd->bnggdh->getMatrix(id);
}
BNModel::~BNModel() 
{
	delete cd;
}
