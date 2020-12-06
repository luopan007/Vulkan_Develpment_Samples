#ifndef VULKANEXBASE_FPSUTIL_H
#define VULKANEXBASE_FPSUTIL_H

#include <vector>

class FPSUtil
{
public:
    static int FPSCount;
    static long long startTime;
    static float currFPS;
    static long long beforeTime;
    static std::vector<int> drawCount;

    static void init();
    static void calFPS();
    static void before();
    static void after(int dstFPS);
};
#endif

