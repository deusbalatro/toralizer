/* toralizer.c */
#include "toralizer.h"

int connect(int sfd2, const struct sockaddr *sock2, socklen_t addrlen)
{

    int sfd; //sfd is socket file descriptor
    struct sockaddr_in sock;
    Req *req;
    Res *res;
    char buf[ressize];
    int success;
    char tmp[512];
    int (*pconnect)(int, const struct sockaddr*, socklen_t);



    pconnect = dlsym(RTLD_NEXT, "connect");
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd < 0)
    {
        perror("socket");
        return -1;
    }

    sock.sin_family = AF_INET;
    sock.sin_port = htons(PROXYPORT);
    sock.sin_addr.s_addr = inet_addr(PROXY);

    if (pconnect(sfd, (struct sockaddr *)&sock, sizeof(sock))!= 0)
    {
        perror("connect");
        return -1;
    }

    printf("Connected to the proxy server\n");
    req = request((struct sockaddr_in*) sock2);
    write(sfd, req, reqsize);

    memset(buf, 0, ressize);
    if(read(sfd, buf, ressize) < 1)
    {
        perror("read");
        free(req);
        close(sfd);

        return -1;
    }

    res = (Res*)buf;
    success = (res->cd == 90);
    if (!success)
    {
        fprintf(stderr, "Unable to traverse the proxy"
            "\nerror code: %d", res->cd);

        close(sfd);
        free(req);

        return -1;
    }

    printf("Successfully connected through the proxy\n");


    dup2(sfd, sfd2);
    free(req);


    return 0;

}

Req *request(struct sockaddr_in *sock2)
{
    Req* req;

    req = malloc(reqsize);

    req->vn = 4;
    req->cd = 1;
    req->dstport = sock2->sin_port;
    req->dstip = sock2->sin_addr.s_addr;
    strncpy(req->userid, USERNAME, 8);

    return req;
}

