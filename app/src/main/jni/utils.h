#ifndef __UTILS_H__
#define __UTILS_H__

#include <libgen.h>

#ifdef __ANDROID__
#include <jni.h>
#include <android/log.h>
#define TAG "ping"
#define pri_debug(format, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, "[%s:%d]" format, basename(__FILE__), __LINE__, ##args)
#define pri_error(format, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, "[%s:%d]" format, basename(__FILE__), __LINE__, ##args)

#else

#define pri_debug(format, args...) printf("[%s:%d]" format, basename(__FILE__), __LINE__, ##args)
#define pri_error(format, args...) printf("[%s:%d]" format, basename(__FILE__), __LINE__, ##args)

#endif

#ifdef __cplusplus
extern "C" {
#endif
#ifdef __ANDROID__
// return需要手动释放
char* jstringToChar(JNIEnv *env, jstring jstr);
jstring charTojstring(JNIEnv* env, const char* str);
#endif
#ifdef __cplusplus
}
#endif

#endif // __UTILS_H__
