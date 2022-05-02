#include "networking.h"
#include <stdlib.h>


int main(void) {
    int ready_fd_num;

    SelectListStruct sSelectList;
    select_ini(&sSelectList);

    fd_set rfds;
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 100;

    int tcpfd, udpfd;
    if ((tcpfd = socket_create(AF_INET, SOCK_STREAM, 0)) < 0) {
        LOG_ERROR("tcp socket create failed");
        exit(1);
    }
    if ((udpfd = socket_create(AF_INET, SOCK_DGRAM, 0)) < 0) {
        LOG_ERROR("udp socket create failed");
        exit(1);
    }

    socket_setopt_noblock(tcpfd);
    socket_setopt_noblock(udpfd);

    SocketAddrStruct addr = {
            .sa_in = {
                    .sin_family = AF_INET,
                    .sin_port = htons(8080),
                    .sin_addr.s_addr = INADDR_ANY},
            .addrlen = sizeof(struct sockaddr_in)
    };

    if (socket_bind(tcpfd, &addr, &sSelectList) == -1) {
        LOG_ERROR("tcpfd bind error");
        exit(1);
    }
    if (socket_bind(udpfd, &addr, &sSelectList) == -1) {
        LOG_ERROR("udpfd bind error");
        exit(1);
    }

    if (socket_listen(tcpfd, 10) == -1) {
        LOG_ERROR("tcpfd listen error");
        exit(1);
    }

    printf("sk_num: %d\n", sSelectList.sk_num);
    for (int idx = 0; idx < sSelectList.sk_num; ++idx) {
        printf("%d\n", sSelectList.sks[idx]);
    }


    while (1) {
        ready_fd_num = socket_select(&rfds, &sSelectList, &timeout);
        if (ready_fd_num < 0) {
            LOG_INFO("select error\n");
            exit(1);
        } else if (ready_fd_num > 0) {
            if (FD_ISSET(tcpfd, &rfds)) {

            }
            if (FD_ISSET(udpfd, &rfds)) {
                printf("udpfd ready\n");
            }
        }
    }
}