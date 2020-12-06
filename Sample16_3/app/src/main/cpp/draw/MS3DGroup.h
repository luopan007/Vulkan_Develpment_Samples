#ifndef MS3DGROUP_H_
#define MS3DGROUP_H_
class MS3DGroup{
private:
	int* indicies; //指向组内三角形索引数组首地址的指针
	int materialIndex; //组对应的材质索引
	int count_ind; //组内的三角形数量
public:
	MS3DGroup(unsigned char* binaryData,int* binaryData_index); //构造函数
	~MS3DGroup(); //析构函数
	int* getIndicies(); //获取组内三角形索引的方法
	int getMaterialIndex(); //获取组对应的材质索引的方法
	int getCount_ind(); //获取组内三角形数量的方法
};
#endif