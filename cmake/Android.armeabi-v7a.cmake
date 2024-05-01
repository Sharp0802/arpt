
execute_process(
    COMMAND $ENV{SHELL} -c "dirname \"$(which ndk-build))\""
    OUTPUT_STRIP_TRAILING_WHITESPACE
    OUTPUT_VARIABLE NDK_HOME)

message(STATUS "NDK found in '" ${NDK_HOME} "'")

set(CMAKE_ANDROID_NDK ${NDK_HOME})

set(CMAKE_SYSTEM_NAME       "Android")
set(CMAKE_SYSTEM_VERSION    24)
set(CMAKE_ANDROID_ARCH_ABI  "armeabi-v7a")
set(CMAKE_ANDROID_STL_TYPE  "c++_static")
set(CMAKE_ANDROID_TOOLCHAIN "clang")
