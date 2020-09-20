#include <jni.h>
#include <errno.h>
#include <vulkan/vulkan.h>
#include <GLES/gl.h>
#include <EGL/egl.h>
#include <android_native_app_glue.h>
#include <stdlib.h>
#include "help.h"
#include "MyVulkanManager.h"
#include "mylog.h"
extern "C"
{
	int xPre;
    int yPre;
	float xDis;
	float yDis;
	bool isClick= true;
	static int32_t engine_handle_input(struct android_app* app, AInputEvent* event){ //事件处理回调方法
		if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION){ //如果是 MOTION 事件
			if(AInputEvent_getSource(event)==AINPUT_SOURCE_TOUCHSCREEN){ //如果是触屏
				int x=AMotionEvent_getRawX(event,0); //获取触控点 x 坐标
				int y=AMotionEvent_getRawY(event,0); //获取触控点 y 坐标
				int32_t id = AMotionEvent_getAction(event); //获取事件类型编号
				switch(id){
					case AMOTION_EVENT_ACTION_DOWN: //触控点按下
						isClick=true; //点击标志设为 true
						xPre=x; //记录触控点 x 坐标
						yPre=y; //记录触控点 y 坐标
						break;
					case AMOTION_EVENT_ACTION_MOVE: //触控点移动
						xDis=x-xPre; //计算触控点 x 位移
						yDis=y-yPre; //计算触控点 y 位移
						MyVulkanManager::yAngle=MyVulkanManager::yAngle+xDis*180.0/600; //y 轴旋转角
						MyVulkanManager::zAngle=MyVulkanManager::zAngle+yDis*180.0/600; //z 轴旋转角
						xPre=x; //记录触控点 x 坐标
						yPre=y; //记录触控点 y 坐标
						if(abs((int)xDis)>10||abs((int)yDis)>10){ //判断移动距离
							isClick= false; //点击标志设为 false
						}break;
					case AMOTION_EVENT_ACTION_UP: //触控点抬起
						if(isClick){ //若点击标志为 true
							if(x<MyVulkanManager::screenWidth/2){ //触控位置在屏幕左侧
								MyVulkanManager::samplerType=(++MyVulkanManager::samplerType%4);
							}else{ //触控位置在屏幕右侧
								MyVulkanManager::texType=(++MyVulkanManager::texType%3); //改变值
							}}
						break;}}
			return true;}
		return false;}
	static void engine_handle_cmd(struct android_app* app, int32_t cmd)
	{
		switch (cmd)
		{
			case APP_CMD_SAVE_STATE:
				LOGI("APP_CMD_SAVE_STATE");
			break;
			case APP_CMD_INIT_WINDOW:
				MyVulkanManager::doVulkan();
				LOGI("APP_CMD_INIT_WINDOW");
			break;
			case APP_CMD_TERM_WINDOW:
				LOGI("APP_CMD_TERM_WINDOW");
			break;
			case APP_CMD_GAINED_FOCUS:
				LOGI("APP_CMD_GAINED_FOCUS");
			break;
			case APP_CMD_LOST_FOCUS:
				MyVulkanManager::loopDrawFlag=false;
				LOGI("APP_CMD_LOST_FOCUS");
			break;
		}
	}
	void android_main(struct android_app* app)
	{
		app_dummy();
		MyVulkanManager::Android_application=app;
		MyData md;
		app->userData = &md;
		app->onAppCmd = engine_handle_cmd;
		app->onInputEvent = engine_handle_input;
		md.app = app;
		bool beginFlag=false;
		while (true)
		{
			int events;
			struct android_poll_source* source;
			while ((ALooper_pollAll((beginFlag?0:-1), NULL, &events,(void**)&source)) >= 0)
			{
				beginFlag=true;
				if (source != NULL)
				{
					source->process(app, source);
				}
			}
		}
	}
}
