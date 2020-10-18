#ifndef VULKANEXBASE_SQUARE_TOUCH2D_H
#define VULKANEXBASE_SQUARE_TOUCH2D_H
#include <android/log.h>
#include <string>
using namespace std;//指定使用的命名空间
class Touch2D {
public:
    static bool Button_about;    //
    static bool Button_resume;    //
    static bool Button_start;    //
    static bool Button_exit;    //
    static bool Button_mute;   //音效
    static float height;
    static float width;
    static void Init_screen();
    static void CheckButton_about(int x,int y);
    static void CheckButton_resume(int x,int y);
    static void CheckButton_back(int x,int y);
    static void CheckButton_exit(int x,int y);
    static void CheckButton_start(int x,int y);
    static void CheckButton_sound(int x, int y);
};


#endif //VULKANEXBASE_SQUARE_TOUCH2D_H
