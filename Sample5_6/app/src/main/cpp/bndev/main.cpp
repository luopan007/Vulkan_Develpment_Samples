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
	int xPre;
    int yPre;
	float xDis;
	float yDis;
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
				//获取坐标
				int x=AMotionEvent_getRawX(event,0);
				int y=AMotionEvent_getRawY(event,0);
				//获取事件类型
				int32_t id = AMotionEvent_getAction(event);
				switch(id)
				{
				    //触摸按下消息
					case AMOTION_EVENT_ACTION_DOWN:
						xPre=x;
						yPre=y;
					break;
					//触摸移动消息
					case AMOTION_EVENT_ACTION_MOVE:
						xDis=x-xPre;
						yDis=y-yPre;
						LightManager::lx=LightManager::lx+xDis*TOUCH_SCALE_FACTOR;
						LightManager::ly=LightManager::ly-yDis*TOUCH_SCALE_FACTOR;
						if(LightManager::lx<=-4.0f){//控制光照方向范围
							LightManager::lx=-4.0f;
						}
						if(LightManager::lx>=4.0f){
							LightManager::lx=4.0f;
						}
						if(LightManager::ly<=-4.0f){//控制光照方向范围
							LightManager::ly=-4.0f;
						}
						if(LightManager::ly>=4.0f){
							LightManager::ly=4.0f;
						}
						xPre=x;
						yPre=y;
					break;
					//触摸弹起消息
					case AMOTION_EVENT_ACTION_UP:
					break;
				}
			}
			return true;
		}
		return false;
	}
	//命令回调方法
	static void engine_handle_cmd(struct android_app* app, int32_t cmd)
	{
		//struct engine* engine = (struct engine*)app->userData;
		switch (cmd)
		{
			case APP_CMD_SAVE_STATE://保存窗口事件
				LOGI("APP_CMD_SAVE_STATE");
			break;
			case APP_CMD_INIT_WINDOW://初始化窗口事件
				MyVulkanManager::doVulkan();
				LOGI("APP_CMD_INIT_WINDOW");
			break;
			case APP_CMD_TERM_WINDOW://终止窗口事件
				LOGI("APP_CMD_TERM_WINDOW");
			break;
			case APP_CMD_GAINED_FOCUS://获取焦点事件
				LOGI("APP_CMD_GAINED_FOCUS");
			break;
			case APP_CMD_LOST_FOCUS://失去焦点事件
				MyVulkanManager::loopDrawFlag=false;
				LOGI("APP_CMD_LOST_FOCUS");
			break;
		}
	}
	//入口函数
	void android_main(struct android_app* app)
	{
		//这一句必须写
		app_dummy();
		MyVulkanManager::Android_application=app;
		MyData md;
		//设置应用的用户数据对象
		app->userData = &md;
		//设置应用的命令回调方法
		app->onAppCmd = engine_handle_cmd;
		//设置应用的事件处理回调方法
		app->onInputEvent = engine_handle_input;
		//将应用指针设置给MyData
		md.app = app;
		//标志位
		bool beginFlag=false;
		while (true)
		{
			int events;
			struct android_poll_source* source;
			//ALooper_pollAll函数的第一个参数为0 表示获取不到输入事件则立即返回
			//为-1表示获取不到就等待
			//循环获取事件消息并处理掉
			while ((ALooper_pollAll((beginFlag?0:-1), NULL, &events,(void**)&source)) >= 0)
			{
				beginFlag=true;
				//处理事件
				if (source != NULL)
				{
					source->process(app, source);
				}
			}

			//做不断循环要做的工作，比如刷帧
		}
	}

//extern "C"的右花括号
}
