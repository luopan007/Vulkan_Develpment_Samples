#include <MyVulkanManager.h>
#include "HelpFunction.h"
bool memoryTypeFromProperties(VkPhysicalDeviceMemoryProperties& memoryProperties, uint32_t typeBits,VkFlags requirements_mask,uint32_t *typeIndex)
{
    for (uint32_t i = 0; i < 32; i++)
    {
        if ((typeBits & 1) == 1)
        {
            if ((memoryProperties.memoryTypes[i].propertyFlags & requirements_mask) == requirements_mask)
            {
                *typeIndex = i;
                return true;
            }
        }
        typeBits >>= 1;
    }
    return false;
}
void calculateABPosition( //计算A、B 点在世界坐标系中坐标的方法
        float x, float y, //触控点x、y 坐标
        float w, float h, //屏幕宽度、高度
        float rightIn,float topIn, float nearIn,float farIn, //近平面相关参数
        float* A,float* B){ //指向存储A、B 点世界坐标系中坐标的数组首地址的指针
    float x0 = x - w / 2; //将以屏幕左上角为原点的触控点X 坐标换算到屏幕中心坐标系
    float y0 = h / 2 - y; //将以屏幕左上角为原点的触控点Y 坐标换算到屏幕中心坐标系
    float xNear = 2 * x0*rightIn / w; //计算对应的近平面上的点A 的x 坐标
    float yNear = 2 * y0*topIn / h; //计算对应的近平面上的点A 的y 坐标
    float ratio = farIn / nearIn; //计算far 与near 的比值
    float xFar = ratio*xNear; //计算对应的远平面上点B 的x 坐标
    float yFar = ratio*yNear; //计算对应的远平面上点B 的y 坐标
    float ax = xNear; float ay = yNear;float az = -nearIn; //摄像机坐标系中A 点的坐标
    float bx = xFar; float by = yFar; float bz = -farIn; //摄像机坐标系中B 点的坐标
    MatrixState3D::fromPtoPreP( ax, ay, az,A); //求世界坐标系中A 点的坐标
    MatrixState3D::fromPtoPreP( bx, by, bz,B); //求世界坐标系中B 点的坐标
}
void calSQ(float x, float y,float* A,float* B){ //计算拾取的方法
    calculateABPosition(
            x, y, //触控点x、y 坐标
            MyVulkanManager::screenWidth,MyVulkanManager::screenHeight, //屏幕宽度、高度
            MatrixState3D::right,MatrixState3D::top,MatrixState3D::nearSelf,MatrixState3D::farSelf,//近平面参数
            A,B //指向存储A、B 点坐标的数组首地址的指针
    );
    Vector3f* start = new Vector3f(A[0], A[1], A[2]); //拾取射线线段起点A 坐标(世界坐标系)
    Vector3f* end = new Vector3f(B[0], B[1], B[2]); //拾取射线线段终点B 坐标(世界坐标系)
    int checkedIndex = -1; //选中物体的索引(-1 表示没有选中物体)
    int tmpIndex = -1; //用于记录距离A 点最近物体的临时索引值
    float minT = 1; //用于临时记录列表中各物体AABB 包围盒与拾取射线最近交点的t 值
    for (int i = 0; i<MyVulkanManager::objList.size(); i++){ //遍历列表中的物体
        AABB* box = MyVulkanManager::objList[i]->getCurrBox(); //获得当前物体的AABB 包围盒
        Vector3f* rayStart = //将拾取射线的起点A 与终点B 变换到物体坐标系
                MatrixState3D::fromGToO(start, MyVulkanManager::objList[i]->m);
        Vector3f* rayEnd = MatrixState3D::fromGToO(end, MyVulkanManager::objList[i]->m);
        float t=box->rayIntersect(rayStart,rayEnd);//计算当前物体AABB 包围盒与拾取射线最近交点的t
        if (t <= minT) { //若小于原最小t 值
            minT = t; //更新最小t 值
            tmpIndex = i; //更新最近物体索引
        }
        delete rayStart;delete rayEnd; //删除辅助对象
    }
    delete start;delete end; //删除辅助对象
    checkedIndex = tmpIndex; //记录最近(选中)物体索引的索引
    changeObj(checkedIndex); //改变被拾取物体的尺寸及颜色
}
void changeObj(int index){ //根据索引改变物体尺寸及颜色的方法
    if (index != -1) { //如果有物体被拾取
        for (int i = 0; i<MyVulkanManager::objList.size(); i++){ //遍历所有可拾取物体列表
            if (i == index) { //若为被拾取物体
                MyVulkanManager::objList[i]->changeOnTouch(true);//改变物体颜色及放大率到选中状态
            }else {//若不是选中物体
                MyVulkanManager::objList[i]->changeOnTouch(false);//改变物体颜色及放大率到未选状态
            }}}
    else{//如果没有物体被选中
        for (int i = 0; i<MyVulkanManager::objList.size(); i++){ //遍历所有可拾取物体列表
            MyVulkanManager::objList[i]->changeOnTouch(false); //改变物体颜色及放大率到未选状态
        }}}
