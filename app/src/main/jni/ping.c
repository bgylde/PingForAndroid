#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdint.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/stat.h>
#include <pthread.h>
#include <libgen.h>

#ifdef __ANDROID__
#include<android/log.h>
#define TAG "ping"
#define pri_debug(format, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, "[%s:%d]" format, basename(__FILE__), __LINE__, ##args)
#define pri_error(format, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, "[%s:%d]" format, basename(__FILE__), __LINE__, ##args)
#else
#define pri_debug(format, args...) printf("[%s:%d]" format, basename(__FILE__), __LINE__, ##args)
#define pri_error(format, args...) printf("[%s:%d]" format, basename(__FILE__), __LINE__, ##args)
#endif

#define DATA_SIZE 32
#define MAX_RECV_SIZE 1024

#define BOOL    int
#define FALSE   -1
#define TRUE    0

typedef struct _TAG_IP_HEADER
{
    u_int8_t    ip_head_verlen;
    u_int8_t    ip_tos;
    u_int16_t   ip_length;
    u_int16_t   ip_id;
    u_int16_t   ip_flags;
    u_int8_t    ip_ttl;
    u_int8_t    ip_protacol;
    u_int16_t   ip_checksum;
    u_int32_t   ip_source;
    u_int32_t   ip_destination;
} IP_HEADER;

typedef struct _TAG_IMCP_HEADER
{
    u_int8_t    icmp_type;
    u_int8_t    icmp_code;
    u_int16_t   check_sum;
    u_int16_t   icmp_id;
    u_int16_t   icmp_seq;
} ICMP_HEADER;

typedef struct _TAG_ICMP_PACKET
{
    ICMP_HEADER     icmp_header;
    struct timeval  icmp_time;
    u_int16_t       icmp_sum_flag;
    u_int8_t        imcp_data[DATA_SIZE];
} ICMP_PACKET;

typedef struct _TAG_THREAD_DATA
{
    int         fd;
    u_int32_t   times;
    ICMP_PACKET * icmp_packet;
    char        * buffer;
    u_int8_t    send_flag;
    struct sockaddr_in * sockaddr;
} THREAD_DATA;

static u_int16_t generation_checksum(u_int16_t * buf, u_int32_t size);
static double get_time_interval(struct timeval * start, struct timeval * end);

u_int16_t generation_checksum(u_int16_t * buf, u_int32_t size)
{
    u_int64_t cksum = 0;
    while(size > 1)
    {
        cksum += *buf++;
        size -= sizeof(u_int16_t);
    }

    if(size)
    {
        cksum += *buf++;
    }

    cksum =  (cksum>>16) + (cksum & 0xffff);
    cksum += (cksum>>16);

    return (u_int16_t)(~cksum);
}

static double get_time_interval(struct timeval * start, struct timeval * end)
{
    double interval;
    struct timeval tp;

    tp.tv_sec = end->tv_sec - start->tv_sec;
    tp.tv_usec = end->tv_usec - start->tv_usec;
    if(tp.tv_usec < 0)
    {
        tp.tv_sec -= 1;
        tp.tv_usec += 1000000;
    }

    interval = tp.tv_sec * 1000 + tp.tv_usec * 0.001;
    return interval;
}

static void * send_imcp(void *arg)
{
    u_int8_t *flag  = NULL;
    int times       = -1;
    int fd          = -1;
    char * buffer   = NULL;
    struct sockaddr_in * dest_socket_addr;
    ICMP_HEADER *icmp_header = NULL;
    ICMP_PACKET *icmp_packet = NULL;

    THREAD_DATA *thread_data = (THREAD_DATA *)arg;
    if (thread_data == NULL)
    {
        return NULL;
    }

    dest_socket_addr = thread_data->sockaddr;
    if (dest_socket_addr == NULL)
    {
        return NULL;
    }

    flag  = &thread_data->send_flag;
    if (flag == NULL)
    {
        return NULL;
    }

    times = thread_data->times;
    fd = thread_data->fd;
    if (fd <= 0)
    {
        return NULL;
    }

    icmp_packet = thread_data->icmp_packet;
    if (icmp_packet == NULL)
    {
        return NULL;
    }

    icmp_header = &(icmp_packet->icmp_header);
    if (icmp_header == NULL)
    {
        return NULL;
    }

    for (int i = 0; i < thread_data->times; i++)
    {
        long result = -1;
        icmp_header->icmp_seq = htons(i);
        icmp_header->check_sum = 0;

        //pri_debug("send packet. %s\n", inet_ntoa(*((struct in_addr*)&(dest_socket_addr->sin_addr.s_addr))));
        gettimeofday(&icmp_packet->icmp_time, NULL);
        icmp_packet->icmp_sum_flag = 0;
        icmp_header->check_sum = generation_checksum((u_int16_t *) icmp_packet, sizeof(ICMP_PACKET));
        //pri_debug("send sum: %x\n", icmp_header->check_sum);
        result = sendto(fd, icmp_packet, sizeof(ICMP_PACKET), 0, (struct sockaddr *)dest_socket_addr,
                        sizeof(struct sockaddr_in));
        if (result == -1)
        {
            pri_error("PING: sendto: Network is unreachable\n");
            continue;
        }

        sleep(1);
    }

    *flag = 0;
    return NULL;
}

static void * recv_imcp(void *arg)
{
    u_int8_t *flag  = NULL;
    int times       = -1;
    int fd          = -1;
    char * buffer   = NULL;
    ICMP_HEADER *icmp_header = NULL;
    ICMP_PACKET *icmp_packet = NULL;

    THREAD_DATA *thread_data = (THREAD_DATA *)arg;
    if (thread_data == NULL)
    {
        return NULL;
    }

    flag  = &thread_data->send_flag;
    if (flag == NULL)
    {
        return NULL;
    }

    times = thread_data->times;
    fd = thread_data->fd;
    if (fd <= 0)
    {
        return NULL;
    }

    icmp_packet = thread_data->icmp_packet;
    if (icmp_packet == NULL)
    {
        return NULL;
    }

    icmp_header = &(icmp_packet->icmp_header);
    if (icmp_header == NULL)
    {
        return NULL;
    }

    struct sockaddr_in from;
    socklen_t from_packet_len;
    long read_length;
    char recv_buf[MAX_RECV_SIZE];
    struct timeval end;

    from_packet_len = sizeof(struct sockaddr_in);
    for (int index = 0; index < times && *flag == 1;)
    {
        read_length = recvfrom(fd, recv_buf, MAX_RECV_SIZE, 0,
                               (struct sockaddr*)&from, &from_packet_len);
        gettimeofday( &end, NULL );
        if(read_length != -1)
        {
            IP_HEADER * recv_ip_header = (IP_HEADER*)recv_buf;
            int ip_ttl = (int)recv_ip_header->ip_ttl;
            ICMP_PACKET * recv_icmp = (ICMP_PACKET *)(recv_buf +
                                                      (recv_ip_header->ip_head_verlen & 0x0F) * 4);
            //icmp_header * recv_icmp_header = (icmp_header *)(recv_buf +
            //    (recv_ip_header->ip_head_verlen & 0x0F) * 4);

            //pri_debug("id: %d, seq: %d\n", recv_icmp->icmp_header.icmp_id, recv_icmp->icmp_header.icmp_seq);
            if(recv_icmp->icmp_header.icmp_type != 0)
            {
                pri_error("error type %d received, error code %d \n", recv_icmp->icmp_header.icmp_type, recv_icmp->icmp_header.icmp_code);
                continue;
            }

            //if(recv_icmp->icmp_header.id != icmp_head->id)
            //{
            //pri_error("some else's packet\n");
            //break;
            //}

            if (recv_icmp->icmp_sum_flag != icmp_packet->icmp_sum_flag)
            {
                pri_error("check sum fail.\n");
                continue;
            }

            if(read_length >= (0 + sizeof(ICMP_PACKET)))
            {
                index++;
                pri_debug("%ld bytes from (%s): icmp_seq=%d ttl=%d time=%.2f ms\n",
                          read_length, inet_ntoa(from.sin_addr), recv_icmp->icmp_header.icmp_seq / 256,
                          ip_ttl, get_time_interval(&recv_icmp->icmp_time, &end));
            }
        }
        else
        {
            if (errno != EAGAIN)
            {
                pri_error("receive data error: %s\n", strerror(errno));
            }
        }
    }

    return NULL;
}

static BOOL get_ping_result(char * domain, char * res_buffer, u_int32_t times)
{
    int ret = FALSE;
    int client_fd = -1;
    int size = 50 * MAX_RECV_SIZE;
    struct timeval timeout;

    ICMP_PACKET * icmp_packet = NULL;
    ICMP_HEADER * icmp_header = NULL;
    struct timeval * icmp_send_time = NULL;

    in_addr_t dest_ip;
    struct sockaddr_in dest_socket_addr;

    pthread_t send_pid;
    pthread_t recv_pid;

    THREAD_DATA thread_data;

    if (res_buffer == NULL || domain == NULL)
    {
        return ret;
    }

    dest_ip = inet_addr(domain);
    if (dest_ip == INADDR_NONE)
    {
        struct hostent* p_hostent = gethostbyname(domain);
        if(p_hostent)
        {
            dest_ip = (*(in_addr_t*)p_hostent->h_addr);
        }
    }

    if (dest_ip == INADDR_NONE)
    {
        return ret;
    }

    client_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
    if (client_fd == -1)
    {
        pri_error("socket error: %s!\n", strerror(errno));
        return ret;
    }

    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    setsockopt(client_fd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
    if(setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval)))
    {
        pri_error("setsocketopt SO_RCVTIMEO error: %s\n", strerror(errno));
        return ret;
    }

    if(setsockopt(client_fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(struct timeval)))
    {
        pri_error("setsockopt SO_SNDTIMEO error: %s\n", strerror(errno));
        return ret;
    }

    memset(dest_socket_addr.sin_zero, 0, sizeof(dest_socket_addr.sin_zero));
    dest_socket_addr.sin_family = AF_INET;
    dest_socket_addr.sin_addr.s_addr = dest_ip;
    dest_socket_addr.sin_port = htons(0);

    icmp_packet = (ICMP_PACKET *)malloc(sizeof(ICMP_PACKET));
    if (icmp_packet == NULL)
    {
        pri_error("malloc error.\n");
        return ret;
    }

    memset(icmp_packet, 0, sizeof(ICMP_PACKET));

    icmp_header = &icmp_packet->icmp_header;
    icmp_header->icmp_type = 8;
    icmp_header->icmp_code = 0;
    icmp_header->icmp_id = getpid();

    icmp_packet->icmp_sum_flag = generation_checksum((u_int16_t *)icmp_packet, sizeof(ICMP_PACKET));
    pri_debug("PING %s (%s).\n", domain, inet_ntoa(*((struct in_addr*)&dest_ip)));

    thread_data.send_flag   = 1;
    thread_data.sockaddr    = &dest_socket_addr;
    thread_data.fd          = client_fd;
    thread_data.buffer      = res_buffer;
    thread_data.times       = times;
    thread_data.icmp_packet = icmp_packet;

    ret = pthread_create(&send_pid, NULL, send_imcp, (void *)&thread_data);
    if (ret < 0)
    {
        pri_error("pthread create error: %s\n", strerror(errno));
        goto FAIL_EXIT;
    }

    ret = pthread_create(&recv_pid, NULL, recv_imcp, (void *)&thread_data);
    if (ret < 0)
    {
        pri_error("pthread create error: %s\n", strerror(errno));
        pthread_detach(send_pid);
        goto FAIL_EXIT;
    }

    pthread_join(send_pid, NULL);
    pthread_join(recv_pid, NULL);

    FAIL_EXIT:
    if (icmp_packet != NULL)
    {
        free(icmp_packet);
        icmp_packet = NULL;
    }

    if (client_fd >= 0)
    {
        close(client_fd);
        client_fd = -1;
    }

    return ret;
}

int ping_host_ip(const char * domain)
{
    char buffer[MAX_RECV_SIZE];
    get_ping_result(domain, buffer, 10);

    pri_debug("ping end.\n");
    return 0;
}

#ifndef __ANDROID__
int main(int argc, char * argv[])
{
    char buffer[MAX_RECV_SIZE];
    BOOL ret = get_ping_result("www.baidu.com", buffer, 10);

    pri_debug("ping result: %d\n", ret);

    return 0;
}
#endif