# PingForAndroid
> C库采用SOCK_DGRAM方式构建icmp包，避开raw socket必须root权限的限制，实现ping功能。 

## ping实现方式
### 1. 通常实现方式
这种方式是直接创建网络层的socket，可以自己构建ip包，也可以委托系统填充。好处是返回的数据包也包括ip数据包，可以获取ttl数据，但是创建raw socket需要拥有root权限这也限制了不能在android上面采用。

```java
socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
```

### 2. 避过root权限限制解决方案
这种方式采用DGRAM方式创建socket，不会被root权限所限制，缺点是返回的数据包不包含ip包数据，无法获取ttl数据。经过测试可以在Android平台通过jni使用，可以满足基本需求。

```java
socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
```
采用这种方式利用了ICMP协议的特性，数据部分不会被修改，会原样返回，所以可以填充发送时间和自己计算首部的校验和来验证自己发送的包；

```java
typedef struct _TAG_ICMP_PACKET
{
    ICMP_HEADER     icmp_header;			//icmp头部，需要根据icmp协议构建
    struct timeval  icmp_time;			    //自己填充发送时间，用于计算往返时间
    u_int16_t       icmp_sum_flag;          //自己填充校验和，验证是否是自己发送的ICMP包
    u_int8_t        imcp_data[DATA_SIZE];   //其它数据，直接填充0
} ICMP_PACKET;
```
经过验证，在mac上面还是可以收到ip首部，移植到android平台就不会收到ip头的数据；所以还是添加了对ip首部的处理逻辑，自己的校验和也添加上，可以在mac平台正常使用。

> 如果对您有用的话麻烦star支持一下~非常感谢