#include <jni.h>
#include <errno.h>
#include <vulkan/vulkan.h>
#include <GLES/gl.h>
#include <EGL/egl.h>
#include <android_native_app_glue.h>
#include <LightManager.h>
#include "help.h"
#include "MyVulkanManager.h"
#include "mylog.h"

extern "C"
{
	int xPre;//上次的触控位置X坐标
	int yPre;//上次的触控位置Y坐标
	float xDis;//存储触控笔X位移的变量
	float yDis;	//存储触控笔Y位移的变量
	float TOUCH_SCALE_FACTOR = 10.0f/320;//角度缩放比例
	//事件处理回调方法
	static int32_t engine_handle_input(struct android_app* app, AInputEvent* event)
	{
		//如果是MOTION事件(包含触屏和轨迹球)
		if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
		{
			//如果是触屏
			if(AInputEvent_getSource(event)==AINPUT_SOURCE_TOUCHSCREEN)
			{
				int x=AMotionEvent_getRawX(event,0);//获取触控点X坐标
				int y=AMotionEvent_getRawY(event,0);//获取触控点Y坐标
				int32_t id = AMotionEvent_getAction(event);//获取事件类型编号
				switch(id)
				{
					case AMOTION_EVENT_ACTION_DOWN:	//触摸点按下
						xPre=x;//记录触控笔上次X点坐标
						yPre=y;//记录触控笔上次Y点坐标
					break;

					case AMOTION_EVENT_ACTION_MOVE://触摸点移动
						xDis=x-xPre;//计算触控笔X位移
						yDis=y-yPre;//计算触控笔Y位移
						LightManager::lx=LightManager::lx+xDis*TOUCH_SCALE_FACTOR;//计算光源位置X点坐标
						LightManager::ly=LightManager::ly-yDis*TOUCH_SCALE_FACTOR;//计算光源位置Y点坐标
						xPre=x;//记录触控笔X点坐标
						yPre=y;//记录触控笔Y点坐标
					break;
					case AMOTION_EVENT_ACTION_UP://触控点抬起
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
