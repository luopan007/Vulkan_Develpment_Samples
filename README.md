# Vulkan_Develpment_Samples
Vulkan开发实战详解配套书籍代码

# 环境搭建

## Window 开发Android环境 

1. JDK的下载/安装和配置：官网下载

2. Android Studio及SDK/NDK下载：官网下载 

3. 安装Cmake插件

4. 编译shaderc, 命令如下

‘‘‘Makefile
ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=Android.mk APP_STL:=gnustl_static APP_ABI=all libshaderc_combined
’’’
