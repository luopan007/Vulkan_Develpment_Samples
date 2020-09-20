#include <TexDrawableObject.h>
#include "TreeControl.h"
#include "MyVulkanManager.h"
#include <math.h>
#include <algorithm>
void TreeControl::sortForTree() { //对植物组中各个植物进行排序的方法
    std::sort(MyVulkanManager::trees, MyVulkanManager::trees + 9, compare);//排序
}
bool TreeControl::compare(TexDrawableObject *A,TexDrawableObject *B) {//排序方法所需比较规则的实现
    float xa=A->x-MyVulkanManager::cx; //计算当前植物位置到摄像机位置向量的x 分量
    float za=A->z-MyVulkanManager::cz; //计算当前植物位置到摄像机位置向量的z 分量
    float xb=B->x-MyVulkanManager::cx; //计算另一植物位置到摄像机位置向量的x 分量
    float zb=B->z-MyVulkanManager::cz; //计算另一植物位置到摄像机位置向量的z 分量
    float disA=xa*xa+za*za; //计算当前植物到摄像机距离的平方
    float disB=xb*xb+zb*zb; //计算另一植物到摄像机距离的平方
    return disA>disB; } //返回距离比较结果
void TreeControl::calculateBillboardDirection() { //计算植物组中每棵树的朝向
    for(int i = 0; i < 9; i ++) { //遍历植物组中的各个植物
        MyVulkanManager::trees[i]->calculateBillboardDirection();//计算每个植物纹理矩形的朝向
    }
    sortForTree(); //对植物组进行排序
}