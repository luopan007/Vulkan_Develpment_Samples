@echo off
set workingDir=%~dp0

REM # 复制gradle文件
set targetFile=wrapper
set targetproperties="D:\Project\Vulkan_Develpment_Samples\Sample1_1\gradle\wrapper\*"

REM # 复制项目属性
set gradle_properties=gradle.properties
set gradle_properties_path="D:\Project\Vulkan_Develpment_Samples\Sample1_1\gradle.properties"

REM # 项目全局配置 
set local_properties=local.properties
set local_properties_path="D:\Project\Vulkan_Develpment_Samples\Sample1_1\local.properties"

REM # 依次编译项目
set project_root_startwith=Sample

for /d %%a in (*) do (
    cd %%a
	call gradlew clean assembledebug
	cd ..
)

REM # 开启延迟变量
setlocal enabledelayedexpansion
REM # 开始遍历文件
for /r %workingDir% %%i in (. *) do ( 
    set "abs=%%i"
    set "fileName=%%~nxi"
REM    if "!fileName!" equ "!targetFile!" (
REM #	    xcopy !targetproperties! !abs! /s /e
REM	)
REM	if "!fileName!" equ "!gradle_properties!" (
REM #	    xcopy !gradle_properties_path! !abs! /s /e
REM	)
REM	if "!fileName!" equ "!local_properties!" (
REM # 	    xcopy !local_properties_path! !abs! /s /e
REM	)
)
setlocal disabledelayedexpansion

pause