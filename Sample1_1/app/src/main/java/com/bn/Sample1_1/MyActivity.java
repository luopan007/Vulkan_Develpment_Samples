package com.bn.Sample1_1;


import android.annotation.TargetApi;
import android.os.Build;

@TargetApi(Build.VERSION_CODES.GINGERBREAD)
public class MyActivity extends android.app.NativeActivity {
    static {
        System.loadLibrary("bn-vulkan-lib");
    }
}