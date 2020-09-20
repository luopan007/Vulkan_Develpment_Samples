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
bool isClick=true;
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
				case AMOTION_EVENT_ACTION_DOWN:	//触控点按下
					isClick=true;//点击标志位置为true
					xPre=x;	//记录触控点x坐标
					yPre=y;//记录触控点y坐标
					break;
				case AMOTION_EVENT_ACTION_MOVE:	//触控点移动
					xDis=x-xPre;//触控点x方向位移
					yDis=y-yPre;//触控点y方向位移
					if(abs((int)xDis)>10||abs((int)yDis)>10) {	//判断触控点位移是否超过阈值
						isClick= false;	//点击标志位置为false
					}
					if(!isClick){//判断是否为滑动操作
						MyVulkanManager::xAngle+=yDis/10;//计算绕x轴转角
						MyVulkanManager::yAngle+=xDis/10;//计算绕y轴转角
						xPre=x;	//更新触控点x坐标
						yPre=y;	//更新触控点y坐标
					}
					break;
				case AMOTION_EVENT_ACTION_UP://触控点抬起
					if(isClick) {//判断是否为点击操作
						MyVulkanManager:: depthOffsetFlag =
								++MyVulkanManager:: depthOffsetFlag %3;	//计算depthOffsetFlag的值
					}
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
