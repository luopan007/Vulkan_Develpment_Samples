@echo off
set workingDir=%~dp0

REM # 复制gradle文件
set targetFile=wrapper
set targetproperties="D:\Project\Vulkan_Develpment_Samples\Sample1_1\gradle\wrapper\*"

REM # 复制项目属性
set gradle_properties=gradle.properties
set gradle_properties_path="D:\Project\Vulkan_Develpment_Samples\Sample1_1\gradle.properties"

REM # 开始遍历文件
setlocal enabledelayedexpansion
for /r %workingDir% %%i in (. *) do ( 
    set "abs=%%i"
    set "fileName=%%~nxi"
    if "!fileName!" equ "!targetFile!" (
REM #	    xcopy !targetproperties! !abs! /s /e
	)
	if "!fileName!" equ "!gradle_properties!" (
	    xcopy !gradle_properties_path! !abs! /s /e
	)
)
setlocal disabledelayedexpansion

pause