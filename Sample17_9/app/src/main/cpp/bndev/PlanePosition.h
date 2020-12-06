#include <vector>
#ifndef SAMPLE7_5_PLANEPOSITION_H
#define SAMPLE7_5_PLANEPOSITION_H
class PlanePosition {
public:
    std::vector<float> curPosition;//当前飞机的位置列表
    int count;//当前飞机的数量
    int id;
    float width;//当前飞机队列的宽度
    float widSpan;//宽度的跨度
    float YMax;//最大Y跨度
    float YSpan;//y跨度
    float ZMax;//最大Z跨度
    void initInstanceData();//初始化数据的方法
    void update();
    PlanePosition(int id);
};


#endif //SAMPLE7_5_PLANEPOSITION_H
