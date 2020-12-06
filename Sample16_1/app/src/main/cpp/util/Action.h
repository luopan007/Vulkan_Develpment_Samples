#ifndef ACTION_H
#define ACTION_H
class Action
{
public:
    float *data[10]; //要执行动作的相关数据
    int totalStep; //总步骤数
    ~Action();//析构函数
};
#endif
