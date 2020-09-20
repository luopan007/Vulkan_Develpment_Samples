cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fexceptions -Wall -D VK_USE_PLATFORM_ANDROID_KHR")

include_directories(${ANDROID_NDK}/sources/android/native_app_glue)
include_directories(${ANDROID_NDK}/sources/third_party/shaderc/include)
include_directories(src/main/cpp/vksysutil)
include_directories(src/main/cpp/util)
include_directories(src/main/cpp/bndev)
include_directories(src/main/cpp/bullet)
include_directories(src/main/cpp/bullet/bulletCollision)
include_directories(src/main/cpp/bullet/bulletCollision/BroadphaseCollision)
include_directories(src/main/cpp/bullet/bulletCollision/NarrowPhaseCollision)
include_directories(src/main/cpp/bullet/bulletCollision/CollisionDispatch)
include_directories(src/main/cpp/bullet/bulletCollision/CollisionShapes)
include_directories(src/main/cpp/bullet/bulletCollision/Gimpact)
include_directories(src/main/cpp/bullet/bulletSoftBody)
include_directories(src/main/cpp/bullet/bulletDynamics)
include_directories(src/main/cpp/bullet/bulletDynamics/Vehicle)
include_directories(src/main/cpp/bullet/bulletDynamics/ConstraintSolver)
include_directories(src/main/cpp/bullet/bulletDynamics/Dynamics)
include_directories(src/main/cpp/bullet/bulletDynamics/Character)
include_directories(src/main/cpp/bullet/bulletDynamics/Featherstone)
include_directories(src/main/cpp/bullet/bulletDynamics/MLCPSolvers)
include_directories(src/main/cpp/bullet/LinearMath)
include_directories(src/main/cpp/bullet/vectormath)


add_library(shaderc_lib STATIC IMPORTED)
set_target_properties(shaderc_lib PROPERTIES IMPORTED_LOCATION
                      ${ANDROID_NDK}/sources/third_party/shaderc/libs/${ANDROID_STL}/${ANDROID_ABI}/libshaderc.a)

add_library(
             bn-vulkan-lib
             SHARED
             ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
             src/main/cpp/vksysutil/vulkan_wrapper.cpp
             src/main/cpp/util/TexDataObject.cpp
             src/main/cpp/util/FileUtil.cpp
             src/main/cpp/util/MatrixState3D.cpp
             src/main/cpp/util/HelpFunction.cpp
             src/main/cpp/util/TextureManager.cpp
             src/main/cpp/util/FPSUtil.cpp
             src/main/cpp/util/TexDrawableObject.cpp
             src/main/cpp/bndev/ShaderCompileUtil.cpp
             src/main/cpp/bndev/ShaderQueueSuit_CommonTex.cpp
             src/main/cpp/bndev/MyVulkanManager.cpp
             src/main/cpp/bndev/ThreadTask.cpp
             src/main/cpp/bndev/main.cpp
             )


target_link_libraries(
                       bn-vulkan-lib
                       GLESv3
                       android
                       log
                       shaderc_lib)
