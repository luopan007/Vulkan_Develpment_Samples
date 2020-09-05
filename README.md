# Vulkan_Develpment_Samples
Vulkan开发实战详解配套书籍代码

# 环境搭建

## Window 开发Android环境 

1. JDK的下载/安装和配置：官网下载

2. Android Studio及SDK/NDK下载：官网下载 

3. 安装Cmake插件

4. 编译shaderc, 命令如下

~~~
ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=Android.mk APP_STL:=gnustl_static APP_ABI=all libshaderc_combined
~~~

## Window 编译 Sample1_1 问题解决

### 1. CMakeLists.txt 环境变量修改 
    ANDROID_NDK 需要修改成各自在环境变量中设置的值

### 2. 升级gradle的版本
    1. 修改 gradle/wrapper/gradle-wrapper.properties --- 最后一行：gradle的版本
    2. 修改 build.gradle  中的 classpath 值 --- 需要与 1. 中的保持一致

### 3. 国内网络代理问题
    1. 修改 build.gradle 中的 repositories --- 填写 阿里云的仓 ---  可以查询的地址为 ：https://maven.aliyun.com/mvn/guide 






