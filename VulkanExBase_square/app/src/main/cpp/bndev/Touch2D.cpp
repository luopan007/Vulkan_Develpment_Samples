#include "Touch2D.h"
#include "../bndev/MyVulkanManager.h"
#include "../square/My3DLayer.h"
#include "PlaySound.h"
#define Touch_value 96
bool Touch2D::Button_about=false;
bool Touch2D::Button_resume=false;
bool Touch2D::Button_exit=false;
bool Touch2D::Button_start=false;
bool Touch2D::Button_mute = false;
float Touch2D::height;
float Touch2D::width;

void Touch2D::Init_screen() {
    height=MyVulkanManager::screenHeight;
    width=MyVulkanManager::screenWidth;
}
void Touch2D::CheckButton_about(int x,int y){
    if(y>(7*height/10.0f)&&y<(8*height/10.0f)){
        if(x>((width/height-0.4)*height/2-height/20.0f)&&x<((width/height-0.4)*height/2+height/20.0f)) {//关于按钮
            LOGE("关于按钮");
            Touch2D::Button_about= !Touch2D::Button_about;
        }
    }
}

void Touch2D::CheckButton_sound(int x, int y) {
    if (y>(7 * height / 10.0f) && y<(8 * height / 10.0f)){
        if (x>((width/height )*height / 2 - height / 20.0f) && x<((width/height)*height / 2 + height / 20.0f))
        {
            LOGE("音效按钮");
            Touch2D::Button_mute= !Touch2D::Button_mute;
            if(Button_mute)
            {
                PlaySound::shutDown("sound/bgm.wav");
            } else{
                PlaySound::playBGM("sound/bgm.wav");
            }
        }
    }
}


void Touch2D::CheckButton_exit(int x, int y) {
    if (y>(7 * height / 10.0f) && y<(8 * height / 10.0f)) {
        if (x>((width/height+0.4)*height / 2 - height / 20.0f) && x<((width/height+0.4)*height / 2 + height / 20.0f)) {//退出按钮
            LOGE("退出游戏");
            MyVulkanManager::destroy_vulkan_instance();
        }
    }
}


void Touch2D::CheckButton_resume(int x, int y){
    if (y>(7 * height / 10.0f) && y<(8 * height / 10.0f)) {
        if (x>((width/height - 0.11)*height / 2 - height / 20.0f) && x<((width/height -0.11)*height / 2 + height / 20.0f)) {//重新开始按钮
            LOGE("重新开始");
            Touch2D::Button_resume= true;
        }
    }
}

void Touch2D::CheckButton_back(int x, int y){
    if (y>(7 * height / 10.0f) && y<(8 * height / 10.0f)) {
        if (x>((width/height + 0.11)*height / 2 - height/ 20.0f) && x<((width/height + 0.11)*height / 2 + height / 20.0f)) {//返回主菜单按钮
            LOGE("返回");
            My3DLayer::RepeatLevel();//重置关卡
            Touch2D::Button_start=false;
            Touch2D::Button_about=false;
            Touch2D::Button_resume=false;
            Touch2D::Button_exit=false;
        }
    }
}

void Touch2D::CheckButton_start(int x, int y) {
    if (y>(0.3f*height ) && y<(0.45f*height)) {
        if (x>((width/height )*height / 2 - 0.1f*height) && x<((width/height )*height / 2 + 0.1f*height)) {//开始按钮
            LOGE("开始游戏");
            Touch2D::Button_start=true;
            Touch2D::Button_about=false;
        }
    }
}