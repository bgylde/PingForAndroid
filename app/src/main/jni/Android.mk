LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := native-color
LOCAL_SRC_FILES := native_color.c ping.c

# for logging
LOCAL_LDLIBS    += -llog -lEGL -lGLESv2

include $(BUILD_SHARED_LIBRARY)
