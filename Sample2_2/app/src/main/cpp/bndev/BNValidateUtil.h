#ifndef VULKANEXBASE_BNValidateUtil_H
#define VULKANEXBASE_BNValidateUtil_H

#include <vector>
#include <vulkan/vulkan.h>
#include <string>

typedef
typedef struct BNDeviceLayerAndExtensionType {
    std::vector<std::string *> layerNames;                //支持的验证Layer名称列表
    std::vector<std::string *> extensionNames;                //支持的验证Layer所需扩展的名称列表
} BNDeviceLayerAndExtension;

class BNValidateUtil {
public:
    static std::vector<VkLayerProperties> layerList;                //获取的验证Layer属性列表
    static PFN_vkCreateDebugReportCallbackEXT dbgCreateDebugReportCallback;
    static PFN_vkDestroyDebugReportCallbackEXT dbgDestroyDebugReportCallback;
    static VkDebugReportCallbackEXT debugReportCallback;    //调试报告回调
    static BNDeviceLayerAndExtension getLayerProperties
            (std::vector<const char *> exceptedLayerNames);

    static std::vector<std::string *> getLayerDeviceExtension(VkPhysicalDevice &gpu,
                                                              std::vector<std::string *> supportedlayerNames);

    static void createDebugReportCallbackSelf(VkInstance &instance);    //创建调试报告回调的方法
    static void destroyDebugReportCallbackSelf(VkInstance &instance);    //销毁调试报告回调的方法
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugFunction(    //用于被回调以打印验证信息的方法
            VkFlags msgFlags,                        //触发此回调执行的调试事件类型标志
            VkDebugReportObjectTypeEXT objType,        //由此回调处理的对象类型
            uint64_t srcObject,                        //此回调创建或处理的对象的句柄
            size_t location,                            //描述对应调试事件代码的位置
            int32_t msgCode,                        //消息代码
            const char *layerPrefix,                    //触发此回调的验证Layer
            const char *msg,                        //消息字符串
            void *pUserData                        //用户自定义数据
    );
};

#endif
