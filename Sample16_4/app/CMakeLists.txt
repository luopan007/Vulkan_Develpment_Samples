cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fexceptions -Wall -D VK_USE_PLATFORM_ANDROID_KHR")

include_directories(${ANDROID_NDK}/sources/android/native_app_glue)
include_directories(${ANDROID_NDK}/sources/third_party/shaderc/include)
include_directories(src/main/cpp/vksysutil)
include_directories(src/main/cpp/util)
include_directories(src/main/cpp/bndev)
include_directories(src/main/cpp/bnggdh)

add_library(shaderc_lib STATIC IMPORTED)
set_target_properties(shaderc_lib PROPERTIES IMPORTED_LOCATION
                      ${ANDROID_NDK}/sources/third_party/shaderc/libs/${ANDROID_STL}/${ANDROID_ABI}/libshaderc.a)

add_library(
             bn-vulkan-lib
             SHARED
             ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
             src/main/cpp/vksysutil/vulkan_wrapper.cpp
             src/main/cpp/util/FileUtil.cpp
             src/main/cpp/util/MatrixState3D.cpp
             src/main/cpp/util/HelpFunction.cpp
             src/main/cpp/util/FPSUtil.cpp
             src/main/cpp/util/ObjObject.cpp
             src/main/cpp/util/TextureManager.cpp
             src/main/cpp/util/TexDataObject.cpp
             src/main/cpp/util/CameraUtil.cpp
             src/main/cpp/bndev/ShaderCompileUtil.cpp
             src/main/cpp/bndev/ShaderQueueSuit_Common.cpp
             src/main/cpp/bndev/MyVulkanManager.cpp
             src/main/cpp/bndev/ThreadTask.cpp
             src/main/cpp/bndev/main.cpp

             src/main/cpp/bnggdh/Mat4.cpp
             src/main/cpp/bnggdh/Animation.cpp
             src/main/cpp/bnggdh/Vector3f.cpp
             src/main/cpp/bnggdh/Vector4f.cpp
             src/main/cpp/bnggdh/Bnggdh.cpp
             src/main/cpp/bnggdh/MyInputStream.cpp
             src/main/cpp/bnggdh/Quaternion.cpp
             src/main/cpp/bnggdh/QuatKey.cpp
             src/main/cpp/bnggdh/Vec3Key.cpp
             src/main/cpp/bnggdh/VertexDataForDraw.cpp
             src/main/cpp/bnggdh/BNModel.cpp
             src/main/cpp/bnggdh/BnggdhDraw.cpp

             )


target_link_libraries(
                       bn-vulkan-lib
                       GLESv3
                       android
                       log
                       shaderc_lib)
