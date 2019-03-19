#ifndef __PING_H__
#define __PING_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>

#define BOOL    int
#define FALSE   -1
#define TRUE    0

BOOL get_ping_result(const char * domain, u_int32_t times, char * res_buffer, int buffer_len);

#ifdef __cplusplus
}
#endif

#endif