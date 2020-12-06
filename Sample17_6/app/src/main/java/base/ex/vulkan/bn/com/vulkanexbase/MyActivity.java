package base.ex.vulkan.bn.com.vulkanexbase;

public class MyActivity extends android.app.NativeActivity
{
    static
    {
        System.loadLibrary("bn-vulkan-lib");
    }
}
