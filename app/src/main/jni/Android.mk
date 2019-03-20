LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := native
LOCAL_SRC_FILES := native_jni.c ping.c utils.c

# for logging
#LOCAL_C_INCLUDES := $(LOCAL_PATH)/httping
#VERSION := 1.0
#LOCAL_CFLAGS += -DNO_SSL -DVERSION=\"$(VERSION)\"
LOCAL_LDLIBS    += -llog -lm

include $(BUILD_SHARED_LIBRARY)
