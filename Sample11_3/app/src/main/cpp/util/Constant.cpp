#include "Constant.h"
float Constant::UNIT_SIZE=30.0f;//单位宽度值
float Constant::bottom_radius=8.0f;//树的底端半径
float Constant::joint_height=8.0f;//每个节点的高度
int Constant::joint_num=20;//节点的总共数目
int Constant:: joint_available_num=14;//节点的有效数目
float Constant::TREE_HEIGHT=joint_height*joint_available_num;//树的高度
float Constant::leaves_width=40.0f;//叶子的宽度;
float Constant::leaves_height=40.0f;//叶子的高度
float Constant::water_height=0.0f;//水面高度
float Constant::LEAVE_CENTER_HEIGHT=TREE_HEIGHT+leaves_height/2;
float Constant::FLOOR_WIDTH=31*UNIT_SIZE;//地面宽度
float Constant::Sky_R=1.5*FLOOR_WIDTH;//天空盒宽度
float Constant::WATER_WIDTH=5*FLOOR_WIDTH;//水面宽度


