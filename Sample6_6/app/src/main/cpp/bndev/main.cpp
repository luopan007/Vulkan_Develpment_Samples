#include <jni.h>
#include <errno.h>
#include <vulkan/vulkan.h>
#include <GLES/gl.h>
#include <EGL/egl.h>
#include <android_native_app_glue.h>
#include <CameraUtil.h>
#include <LightManager.h>
#include "help.h"
#include "MyVulkanManager.h"
#include "mylog.h"
extern "C"
{
	int xPre;
    int yPre;
	float xDis;
	float yDis;
	static int32_t engine_handle_input(struct android_app* app, AInputEvent* event)//事件处理回调方法
	{
		if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)//如果是MOTION事件
		{
			if(AInputEvent_getSource(event)==AINPUT_SOURCE_TOUCHSCREEN)//如果是触屏
			{
				int x=AMotionEvent_getRawX(event,0);//获取触控点x坐标
				int y=AMotionEvent_getRawY(event,0);//获取触控点y坐标
				int32_t id = AMotionEvent_getAction(event);//获取事件类型编号
				switch(id)
				{
					case AMOTION_EVENT_ACTION_DOWN://触控点移动
						xPre=x;//计算触控点x位移
						yPre=y;//计算触控点y位移
					break;
					case AMOTION_EVENT_ACTION_MOVE://触控点移动
						xDis=x-xPre;//计算触控点x位移
						yDis=y-yPre;//计算触控点y位移
						CameraUtil::calCamera(float(-yDis * 180 / 1000.0), 0);//更新摄像机9参数
						LightManager::move(float(xDis * 180 / 1000.0));//移动光源
						xPre=x;//记录触控点x坐标
						yPre=y;//记录触控点y坐标
					break;
					case AMOTION_EVENT_ACTION_UP:
					break;
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
