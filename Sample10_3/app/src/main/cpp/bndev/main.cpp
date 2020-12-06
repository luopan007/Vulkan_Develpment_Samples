#include <jni.h>
#include <errno.h>
#include <vulkan/vulkan.h>
#include <GLES/gl.h>
#include <EGL/egl.h>
#include <android_native_app_glue.h>
#include <CameraUtil.h>
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
	bool isClick;
	static int32_t engine_handle_input(struct android_app* app, AInputEvent* event)
	{
		if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
		{
			if(AInputEvent_getSource(event)==AINPUT_SOURCE_TOUCHSCREEN)
			{
				int x=AMotionEvent_getRawX(event,0);
				int y=AMotionEvent_getRawY(event,0);
				int32_t id = AMotionEvent_getAction(event);
				switch(id){
					case AMOTION_EVENT_ACTION_DOWN: //触控点按下
						isClick=true; //点击标志置为true
						xPre=x; yPre=y; //记录触控点x、y 坐标
						break;
					case AMOTION_EVENT_ACTION_MOVE: //触控点移动
						xDis=x-xPre; yDis=y-yPre; //计算触控点x、y 方向位移
						if(abs(xDis)>10||abs(yDis)>10){ isClick=false;} //位移超过阈值则将点击标志置为false
						if(!isClick){ //若为滑动操作
							CameraUtil::calCamera(-yDis*180/1000.0,-xDis*180/1000.0);//重新计算摄像机参数
							xPre=x; yPre=y; //更新触控点x、y 坐标
						}break;
					case AMOTION_EVENT_ACTION_UP: //触控点抬起
						if(isClick){ //若为点击操作
							MyVulkanManager::ifDiscard=!MyVulkanManager::ifDiscard;//更新是否丢弃片元标志
						}break;
				}

			}
			return true;
		}
		return false;
	}
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
