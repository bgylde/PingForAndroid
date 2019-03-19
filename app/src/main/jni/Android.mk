LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := native
LOCAL_SRC_FILES := native_jni.c ping.c utils.c

# for logging
LOCAL_LDLIBS    += -llog -lEGL -lGLESv2

include $(BUILD_SHARED_LIBRARY)
