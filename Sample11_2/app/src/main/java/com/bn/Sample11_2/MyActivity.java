package com.bn.Sample11_2;

public class MyActivity extends android.app.NativeActivity
{
    static
    {
        System.loadLibrary("bn-vulkan-lib");
    }
}