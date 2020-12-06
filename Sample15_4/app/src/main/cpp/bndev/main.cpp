#include <jni.h>
#include <errno.h>
#include <vulkan/vulkan.h>
#include <GLES/gl.h>
#include <EGL/egl.h>
#include <android_native_app_glue.h>
#include <stdlib.h>
#include <math.h>
#include "help.h"
#include "MyVulkanManager.h"
#include "../util/CameraUtil.h"
#include "../util/LightManager.h"
#include "mylog.h"
extern "C"
{
	int xPre;
    int yPre;
	float xDis;
	float yDis;
	bool isClick;
	float maxAngle=8.0f;
	float TOUCH_SCALE_FACTOR = 18.0f/360;
float cAngle=0;
float xAngle=0;
	static int32_t engine_handle_input(struct android_app* app, AInputEvent* event){ 
		if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION){ 	
			if(AInputEvent_getSource(event)==AINPUT_SOURCE_TOUCHSCREEN){ 
				int x=AMotionEvent_getRawX(event,0); 						
				int y=AMotionEvent_getRawY(event,0); 						
				int32_t id = AMotionEvent_getAction(event); 					
					switch(id){
						case AMOTION_EVENT_ACTION_DOWN: 					
							xPre=x; 											
							yPre=y; 											
							isClick=true;											
						break;
						case AMOTION_EVENT_ACTION_MOVE: 					
							xDis=x-xPre; 										
							yDis=y-yPre; 										
							if(abs((int)xDis)>10||abs((int)yDis)>10) {						
								isClick= false;										
							}
							if(!isClick){											
								cAngle+=xDis * TOUCH_SCALE_FACTOR;
								xAngle+=yDis*TOUCH_SCALE_FACTOR;
								if(cAngle<=-maxAngle){
									cAngle=-maxAngle;
								}
								if(cAngle>=maxAngle){
									cAngle=maxAngle;
								}
								MyVulkanManager::tx=(float)(sin(cAngle*3.1415926535898/180)*80);
								MyVulkanManager::tz=(float)(cos(cAngle*3.1415926535898/180)*80)-90;
								xPre=x;											
								yPre=y;											
							}
						break;
						case AMOTION_EVENT_ACTION_UP: 						
							if(isClick) {											
								if(MyVulkanManager::pipeLineIndex==0){
									MyVulkanManager::pipeLineIndex=1;
								}else{
									MyVulkanManager::pipeLineIndex=0;
								}
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
