/* toralizer.h */

#define _GNU_SOURCE


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <dlfcn.h>


#define     PROXY       "127.0.0.1"
#define     PROXYPORT   9050
#define     USERNAME    "toraliz"
#define     reqsize     sizeof(struct proxy_request)
#define     ressize     sizeof(struct proxy_response)

typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;


/*

		+----+----+----+----+----+----+----+----+----+----+....+----+
		| VN | CD | DSTPORT |      DSTIP        | USERID       |NULL|
		+----+----+----+----+----+----+----+----+----+----+....+----+
	           1    1      2              4           variable       1
from: https://www.openssh.com/txt/socks4.protocol
userid will involve only 7 chars, 8th char will be null terminator
*/

struct proxy_request {

    int8 vn;
    int8 cd;
    int16 dstport;
    int32 dstip;
    unsigned char userid[8];

};
typedef struct proxy_request Req;

/*
        	+----+----+----+----+----+----+----+----+
		| VN | CD | DSTPORT |      DSTIP        |
		+----+----+----+----+----+----+----+----+
        	   1    1      2              4
*/

struct proxy_response {
    int8 vn;
    int8 cd;
    int16 _;    // ignored
    int32 __;   // ignored
};
typedef struct proxy_response Res;

Req *request(struct sockaddr_in*);
int connect(int, const struct sockaddr*, socklen_t);
