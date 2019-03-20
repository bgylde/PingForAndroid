#include <jni.h>
#include <stdlib.h>
#include <string.h>

#include "ping.h"
#include "utils.h"
#include "native_jni.h"

#define PING_RES_BUFFER_LEN 1024

/*
 * Class:     com_sohu_opengl_NativeMethodHelper
 * Method:    ping
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jstring JNICALL Java_com_sohu_ping_NativeMethodHelper_ping
  (JNIEnv *env, jclass nclass, jstring nstring)
{
    char * domain = NULL;
    char res_buffer[PING_RES_BUFFER_LEN];

    domain = jstringToChar(env, nstring);
    if (domain == NULL)
    {
        pri_error("convert string error.\n");
        return NULL;
    }

    pri_debug("convert string: %s\n", domain);
    memset(res_buffer, 0, PING_RES_BUFFER_LEN);

    get_ping_result(domain, 4, res_buffer, PING_RES_BUFFER_LEN);
    if (domain != NULL)
    {
        free(domain);
        domain = NULL;
    }

    pri_debug("%s\n", res_buffer);
    return charTojstring(env, res_buffer);
}