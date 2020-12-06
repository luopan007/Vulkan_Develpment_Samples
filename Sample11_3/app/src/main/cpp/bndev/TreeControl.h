#ifndef SAMPLE11_3_TREECONTROL_H
#define SAMPLE11_3_TREECONTROL_H

#include <vector>
#include <TexLeafDrawableObject.h>
#include <TexTrunkDrawableObject.h>

class TreeControl {
public:
    TreeControl(); //构造函数
    ~TreeControl(); //析构函数
    static std::vector<TexLeafDrawableObject*> leafVector; //存放所有树叶的列表
    static std::vector<TexTrunkDrawableObject*> trunkVector; //存放所有树干部件的列表
    static std::vector<float*> leafInitialPos; //存放每片树叶中心点初始位置的列表
    void drawSelf(float bend_RIn,float direction_degreeIn); //绘制椰子树的方法
    static void calculateDistance(); //根据树叶中心点与摄像机距离排序的方法
    static bool compare(TexLeafDrawableObject* ta,TexLeafDrawableObject* tb);//计算树叶与摄像机距离
    static float* calculateLastPosition(float bend_R,float direction_degree,float pointX,float pointY,float pointZ); //计算树叶最终姿态的方法
    static void calculateCenterPosition(float bend_RIn,float direction_degreeIn);//计算树叶中心位置的方法
};


#endif //SAMPLE11_3_TREECONTROL_H
