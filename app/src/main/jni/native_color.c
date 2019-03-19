#include <jni.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include "ping.h"
#include "com_sohu_opengl_NativeMethodHelper.h"

extern int ping_host_ip(const char * domain);

/*
 * Class:     com_sohu_opengl_NativeColorRenderer
 * Method:    surfaceCreated
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_sohu_opengl_NativeMethodHelper_surfaceCreated(JNIEnv *env, jobject obj, jint color) {
    GLfloat redF = ((color >> 16) & 0xFF) * 1.0f / 255;
    GLfloat greenF = ((color >> 8) & 0xFF) * 1.0f / 255;
    GLfloat blueF = (color & 0xFF) * 1.0f / 255;
    GLfloat alphaF = ((color >> 24) & 0xFF) * 1.0f / 255;
    glClearColor(redF, greenF, blueF, alphaF);
}

/*
 * Class:     com_sohu_opengl_NativeColorRenderer
 * Method:    surfaceChanged
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_sohu_opengl_NativeMethodHelper_surfaceChanged
  (JNIEnv *env, jobject obj, jint width, jint height) {
    glViewport(0, 0, width, height);
}

/*
 * Class:     com_sohu_opengl_NativeColorRenderer
 * Method:    onDrawFrame
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_sohu_opengl_NativeMethodHelper_onDrawFrame
  (JNIEnv *env, jobject obj){
    glClear(GL_COLOR_BUFFER_BIT);
}

/*
 * Class:     com_sohu_opengl_NativeMethodHelper
 * Method:    ping
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_sohu_opengl_NativeMethodHelper_ping
  (JNIEnv *env, jclass nclass, jstring nstring) {
    ping_host_ip("www.baidu.com");
}