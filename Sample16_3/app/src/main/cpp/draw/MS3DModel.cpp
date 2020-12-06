#include "MS3DModel.h"
#include "mylog.h"
#include <assert.h>
#include <map>
#include <MyVulkanManager.h>
#include "HelpFunction.h"
#include "MatrixState3D.h"
#include "FileUtil.h"
#include "TextureManager.h"
MS3DModel::MS3DModel(VkDevice& device, //构造函数
                     VkPhysicalDeviceMemoryProperties& memoryroperties, unsigned char* binaryData) {
    pushConstantData=new float[16]; //创建推送常量数据数组
    this->devicePointer=&device; //接收逻辑设备指针并保存
    initMs3dInfo(binaryData); //调用方法加载ms3d 文件信息
    vertexDatabuf=new VkBuffer[groCount]; //创建顶点数据缓冲数组
    vertexDataMem=new VkDeviceMemory [groCount]; //创建顶点数据所需设备内存数组
    vertexDataBufferInfo=new VkDescriptorBufferInfo[groCount]; //创建顶点数据缓冲描述信息数组
    mem_reqs=new VkMemoryRequirements[groCount]; //创建缓冲内存需求数组
    initVertexData(); //调用方法初始化顶点数据
    initVertexBuffer(memoryroperties); //调用方法创建顶点数据缓冲
}
void MS3DModel::initMs3dInfo(unsigned char* binaryData){ //加载ms3d 文件信息的方法
    int binaryData_index=0; //数据读取辅助索引（以字节计）
    header = new MS3DHeader(binaryData, &binaryData_index); //创建ms3d 文件头信息对象
    vCount=FileUtil::myReadUnsignedShort(binaryData, &binaryData_index); //读取顶点数量
    for(int i=0; i<vCount; i++){ //循环创建相应数量的顶点信息对象
        MS3DVertex* vertex = new MS3DVertex(binaryData, &binaryData_index); //创建顶点信息对象
        vertexs.push_back(vertex); //将顶点信息对象指针存入相应列表
    }
    int num_tri=FileUtil::myReadUnsignedShort(binaryData, &binaryData_index); //读取三角形面数量
    for(int i=0; i<num_tri; i++){ //循环创建相应数量的三角形组装信息对象
        MS3DTriangle* triangle= //创建三角形组装信息对象
                new MS3DTriangle (binaryData,&binaryData_index);
        triangles.push_back(triangle); //将三角形组装信息对象指针存入相应列表
    }
    groCount=FileUtil::myReadUnsignedShort(binaryData,&binaryData_index); //读取组数量
    for(int i=0; i<groCount; i++){ //循环创建相应数量的组信息对象
        MS3DGroup* group = new MS3DGroup(binaryData,&binaryData_index); //创建组信息对象
        groups.push_back(group); //将组信息对象指针存入相应列表
    }
    int num_mat=FileUtil::myReadUnsignedShort(binaryData,&binaryData_index); //读取材质数量
    for(int i=0; i<num_mat; i++){ //循环创建相应数量的材质信息对象
        MS3DMaterial* mal = new MS3DMaterial(binaryData,&binaryData_index); //创建材质信息对象
        materials.push_back(mal); //将材质信息对象指针存入相应列表
    }
    fps=FileUtil::myReadFloat(binaryData,&binaryData_index); //读取帧速率信息
    current_time = FileUtil::myReadFloat(binaryData,&binaryData_index); //读取当前时间
    frame_count = FileUtil::myReadInt(binaryData,&binaryData_index); //读取关键帧总数
    totalTime = frame_count / fps; //计算动画总时间
    joiCount=FileUtil::myReadUnsignedShort(binaryData,&binaryData_index); //读取关节数量
    map<string, MS3DJoint*> mapp; //用于保存关节id 与关节信息对象对应关系的临时map
    for(int i = 0; i < joiCount; i++){ //循环加载每个关节的信息
        MS3DJoint* joint = new MS3DJoint(binaryData,&binaryData_index); //创建关节信息对象
        mapp[joint->name]=joint; //将关节信息存入map 以备查找
        map<string,MS3DJoint*>::iterator iter=mapp.begin(); //获取mapp 的迭代器对象
        for(; iter != mapp.end(); iter++){ //遍历mapp
            string pname=iter->first; //获取关节id
            if(pname==(joint->parentName)){ //判断该关节是否为当前关节的父关节
                joint->parent = mapp[joint->parentName]; //获得父关节信息对象
                joint->ifparent=true; //设置当前关节信息对象为有父关节状态
                break;
            }}
        joint->relative =new Mat4(); //创建当前关节的相对矩阵对象
        joint->relative->loadIdentity(); //初始化相对矩阵为单位矩阵
        //将欧拉角表示的旋转数据转换为矩阵形式表示的旋转数据并存入相对矩阵中
        joint->relative->genRotationFromEulerAngle(joint->trotate);
        joint->relative->setTranslation(joint->tposition); //将初始平移信息记录进相对矩阵中
        joint->absolute = new Mat4(); //创建当前关节的绝对矩阵对象
        joint->absolute->loadIdentity(); //初始化绝对矩阵为单位矩阵
        if(joint->ifparent){ //若有父关节
        //子关节的绝对矩阵等于父关节的绝对矩阵乘以子关节的相对矩阵
            joint->absolute->mul(joint->parent->absolute,joint->relative);
        }else{ //若没有父关节
            joint->absolute->copyFrom(joint->relative); //相对矩阵即为绝对矩阵
        }joints.push_back(joint); //将关节信息对象指针存入相应列表
    }mapp.clear(); //清除临时map

}
void MS3DModel::initVertexData() { //初始化顶点数据的方法
    updateJoint(0.0f); //将关节更新到起始时间（时间为0 的时间）
    int triangleCount = 0; //表示组内三角形个数的辅助变量
    MS3DTriangle *triangle; //指向三角形组装信息对象的辅助指针
    int *indexs; //指向组内三角形索引数组首地址的指针
    int *vertexIndexs; //指向当前处理三角形顶点索引数组首地址的指针
    for (MS3DGroup *group:groups) { //对模型中的每个组进行遍历
        int count = 0; //辅助索引
        indexs = group->getIndicies(); //获取组内三角形索引数组
        triangleCount = group->getCount_ind(); //获取组内三角形数量
        float *groupVdata = new float[triangleCount * 3 * 5]; //初始化本组顶点数据数组
        for (int j = 0; j < triangleCount; j++) { //遍历组内的每个三角形
            triangle = triangles[indexs[j]]; //获取当前要处理的三角形
            vertexIndexs = triangle->getIndexs(); //获取当前三角形的顶点索引数组
            for (int k = 0; k < 3; k++) { //循环对三角形中的3 个顶点进行处理
                MS3DVertex *mvt =vertexs[vertexIndexs[k]]; //根据顶点索引获取当前顶点
                groupVdata[count++] = mvt->getInitPosition()->getX(); //顶点位置的X 坐标
                groupVdata[count++] = mvt->getInitPosition()->getY(); //顶点位置的Y 坐标
                groupVdata[count++] = mvt->getInitPosition()->getZ(); //顶点位置的Z 坐标
                groupVdata[count++] = triangle->getS()->getVector3fArray()[k];//顶点的纹理S 坐标
                groupVdata[count++] = triangle->getT()->getVector3fArray()[k];//顶点的纹理T 坐标
            }}
        vdata.push_back(groupVdata); //将当前组的顶点数据添加进总数据列表
    }}
void MS3DModel::initVertexBuffer(VkPhysicalDeviceMemoryProperties& memoryroperties){
    MS3DGroup* group;
    for (int i = 0; i < groups.size();i++)          
    {
        group = groups[i];	                                
        int triangleCount  = group->getCount_ind();     
        int dataByteCount=triangleCount*3*5*sizeof(float);
        VkBufferCreateInfo buf_info = {};
        buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        buf_info.pNext = NULL;
        buf_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        buf_info.size = dataByteCount;
        buf_info.queueFamilyIndexCount = 0;
        buf_info.pQueueFamilyIndices = NULL;
        buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        buf_info.flags = 0;
        VkResult result = vk::vkCreateBuffer(*devicePointer, &buf_info, NULL, &vertexDatabuf[i]);
        assert(result == VK_SUCCESS);
        vk::vkGetBufferMemoryRequirements(*devicePointer, vertexDatabuf[i], &mem_reqs[i]);
        assert(dataByteCount<=mem_reqs[i].size);
        VkMemoryAllocateInfo alloc_info = {};
        alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        alloc_info.pNext = NULL;
        alloc_info.memoryTypeIndex = 0;
        alloc_info.allocationSize = mem_reqs[i].size;
        VkFlags requirements_mask=VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        bool flag=memoryTypeFromProperties(memoryroperties, mem_reqs[i].memoryTypeBits,requirements_mask,&alloc_info.memoryTypeIndex);
        if(flag)
        {
            LOGE("确定内存类型成功 类型索引为%d",alloc_info.memoryTypeIndex);
        }
        else
        {
            LOGE("确定内存类型失败!");
        }
        result = vk::vkAllocateMemory(*devicePointer,&alloc_info, NULL, &vertexDataMem[i]);
        assert(result == VK_SUCCESS);
        uint8_t *pData;
        result = vk::vkMapMemory(*devicePointer,vertexDataMem[i], 0, mem_reqs[i].size, 0, (void **)&pData);
        assert(result == VK_SUCCESS);
        memcpy(pData, vdata[i], dataByteCount);
        vk::vkUnmapMemory(*devicePointer, vertexDataMem[i]);
        result = vk::vkBindBufferMemory(*devicePointer,vertexDatabuf[i], vertexDataMem[i], 0);
        assert(result == VK_SUCCESS);
        vertexDataBufferInfo[i].buffer = vertexDatabuf[i];
        vertexDataBufferInfo[i].offset = 0;
        vertexDataBufferInfo[i].range = mem_reqs[i].size;
    }
}
void MS3DModel::animate(float ttime,VkCommandBuffer& cmd, //执行动画的方法
                        VkPipelineLayout& pipelineLayout,VkPipeline& pipeline){
    if (current_time != ttime) { //判断动画时间是否有更新
        updateJoint(ttime); //更新关节数据
        updateAllVectexs(); //更新顶点数据
        reAssemVertexData(); //重新组装数据
        drawSelf(true,cmd,pipelineLayout,pipeline); //调用drawSelf 方法执行绘制
    } else {
        drawSelf(false,cmd,pipelineLayout,pipeline); //调用drawSelf 方法执行绘制
    }}
void MS3DModel::updateJoint(float ttime){ //更新关节数据的方法
    current_time = ttime; //设置当前时间
    if(current_time > totalTime){ //若当前动画时间超过总动画时间
        current_time = 0.0f; //设置当前时间为0
    }
    for (MS3DJoint* jt : joints){ //对模型中的每个关节进行遍历
        jt->update(current_time); //调用当前关节的更新方法
    }}
void MS3DModel::updateAllVectexs(){ //更新所有顶点位置数据的方法
    for (MS3DVertex* vtHelper : vertexs){ //对模型中的所有顶点进行遍历
        if (vtHelper->getBone() == -1) { //若无关节控制
            vtHelper->setCurrPosition(vtHelper->getInitPosition()); //初始位置即为当前位置
        }
        else { //若有关节控制
            int it = vtHelper->getBone(); //获取对应关节索引
            jointHelper = joints[it]; //获取对应的关节信息对象
            Vector3f* v3Helper1 = jointHelper->getAbsolute()-> //获取顶点在关节坐标系中的初始坐标
                    invTransformAndRotate(vtHelper->getInitPosition());
            Vector3f* v3Helper2 = //根据关节的实时变换状态计算出顶点经关节影响后的位置
                    jointHelper->getMatrix()->transform(v3Helper1);
            vtHelper->setCurrPosition(v3Helper2); //设置顶点当前位置
            delete v3Helper1;delete v3Helper2; //释放两个辅助变量
        }}}
void MS3DModel::reAssemVertexData(){
    int triangleCount = 0;
    MS3DGroup* group;
    MS3DTriangle* triangle;
    MS3DVertex* vertex;
    int* indexs;
    int* vertexIndexs;
    for(int i=0;i<groCount;i++)
    {
        int count=0;
        group=groups[i];
        triangleCount=group->getCount_ind();
        indexs=group->getIndicies();
        float *groupVdata=vdata[i];
        for(int j=0;j<triangleCount;j++)
        {
            triangle=triangles[indexs[j]];
            vertexIndexs=triangle->getIndexs();
            for(int k=0;k<3;k++)
            {
                vertex=vertexs[vertexIndexs[k]];
                groupVdata[count++]=vertex->getCurrPosition()->getX();
                groupVdata[count++]=vertex->getCurrPosition()->getY();
                groupVdata[count++]=vertex->getCurrPosition()->getZ();
                groupVdata[count++]=triangle->getS()->getVector3fArray()[k];
                groupVdata[count++]=triangle->getT()->getVector3fArray()[k];
            }
        }
    }
}
void MS3DModel::drawSelf(bool isUpdate,VkCommandBuffer& cmd, //绘制方法
                         VkPipelineLayout& pipelineLayout,VkPipeline& pipeline){
    MS3DGroup* group; //临时辅助用组信息对象指针
    MS3DMaterial* material; //临时辅助用材质信息对象指针
    VkDescriptorSet* desSetPointer; //描述集指针
    for(int i=0;i<groups.size();i++) { //对模型中的所有组进行遍历
        group = groups[i]; //获取当前组信息对象
        int triangleCount = group->getCount_ind(); //获取组内三角形的数量
        if (group->getMaterialIndex() > -1) { //若有材质（即需要贴纹理）
            material = materials[group->getMaterialIndex()]; //获取材质信息对象
            desSetPointer = &MyVulkanManager::sqsCL->descSet //指定描述集（绑定纹理）
            [TextureManager::getVkDescriptorSetIndex(
                            "texture/" + texName2bntex(material->textureName))];
        }
        if (isUpdate) { //若需要更新顶点数据缓冲
            groVdata = vdata[i]; //获取当前组顶点数据数组
            uint8_t *pData; //CPU 访问时的辅助指针
            VkResult result = vk::vkMapMemory(*devicePointer, //将设备内存映射为CPU 可访问
                                              vertexDataMem[i], 0, mem_reqs[i].size, 0,
                                              (void **) &pData);
            assert(result == VK_SUCCESS); //判断内存映射是否成功
            memcpy(pData, groVdata, triangleCount * 3 * 5 * sizeof(float)); //将数据拷贝进设备内存
            vk::vkUnmapMemory(MyVulkanManager::device, vertexDataMem[i]); //解除内存映射
        }
        vk::vkCmdBindPipeline(cmd, //将当前使用的命令缓冲与指定管线绑定
                              VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
        vk::vkCmdBindDescriptorSets(cmd, //将命令缓冲、管线布局、描述集绑定
                                    VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1,
                                    desSetPointer, 0, NULL);
        const VkDeviceSize offsetsVertex[1] = {0}; //顶点数据偏移量数组
        vk::vkCmdBindVertexBuffers(cmd, //将当前组顶点数据与当前使用的命令缓冲绑定
                                   0, 1, &(vertexDatabuf[i]), offsetsVertex);
        float *mvp = MatrixState3D::getFinalMatrix(); //获取总变换矩阵
        memcpy(pushConstantData, mvp, sizeof(float) * 16); //将总变换矩阵拷贝进推送常量数据数组
        vk::vkCmdPushConstants(cmd, //将推送常量数据送入管线
                               pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(float) * 16,
                               pushConstantData);
        vk::vkCmdDraw(cmd, triangleCount * 3, 1, 0, 0); //绘制当前组
    }
}
MS3DModel::~MS3DModel() {
    delete header;
    vector<MS3DVertex*>::iterator iter=vertexs.begin();
    for(;iter!=vertexs.end();iter++)
    {
        MS3DVertex* vt=(*iter);
        delete vt;
    }
    vertexs.clear();
    vector<MS3DTriangle*>::iterator iter0=triangles.begin();
    for(;iter0!=triangles.end();iter0++)
    {
        MS3DTriangle* vt=(*iter0);
        delete vt;
    }
    triangles.clear();
    vector<MS3DGroup*>::iterator iter1=groups.begin();
    for(;iter1!=groups.end();iter1++)
    {
        MS3DGroup* vt=(*iter1);
        delete vt;
    }
    groups.clear();
    vector<MS3DMaterial*>::iterator iter2=materials.begin();
    for(;iter2!=materials.end();iter2++)
    {
        MS3DMaterial* vt=(*iter2);
        delete vt;
    }
    materials.clear();
    vector<MS3DJoint*>::iterator iter3=joints.begin();
    for(;iter3!=joints.end();iter3++)
    {
        MS3DJoint* vt=(*iter3);
        delete vt;
    }
    joints.clear();
    for (float* f:vdata)
    {
        delete [] f;
    }
    vector<MS3DGroup*>::iterator iter4=groups.begin();
    for(int i=0;iter4!=groups.end();iter4++,i++)        
    {
        vk::vkDestroyBuffer(*devicePointer, vertexDatabuf[i], NULL);
        vk::vkFreeMemory(*devicePointer, vertexDataMem[i], NULL);
    }
    delete  jointHelper;
}
