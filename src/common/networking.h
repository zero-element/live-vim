#ifndef __NETWORKING_H
#define __NETWORKING_H

#define MAX_CONNECTION 32

#include "log.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

// IO多路复用select基础结构
typedef struct {
    int sk_num;
    int sks[MAX_CONNECTION];
} SelectListStruct;

// IP地址结构
typedef struct {
    struct sockaddr_in sa_in;
    socklen_t addrlen;

} SocketAddrStruct;

// select相关函数：初始化、添加socket、删除socket
int select_ini(SelectListStruct *sSelectList);

int select_add(SelectListStruct *sSelectList, int sk);

int select_del(SelectListStruct *sSelectList, int sk);

// socket相关函数：创建、监听端口、绑定地址、接受请求、建立连接、关闭文件描述符、读取/发送数据
int socket_create(int domain, int type, int protocol);

int socket_listen(int sk, int listen_count);

int socket_bind(int sk, SocketAddrStruct *sAddr, SelectListStruct *sSelectList);

int socket_accept(int sk, SelectListStruct *sSelectList);

int socket_connect(int sk, SocketAddrStruct *sAddr, SelectListStruct *sSelectList);

int socket_close(int sk, SelectListStruct *sSelectList);

int socket_tcp_read(int sk, char *buf, int len);

int socket_udp_read(int sk, char *buf, int len);

int socket_tcp_write(int sk, char *buf, int len);

int socket_udp_write(int sk, char *buf, int len);

// select用到socket中
int socket_select(fd_set *fds, SelectListStruct *sSelectList, struct timeval *timeout);

// socket可选项设置：地址复用、保持连接、非阻塞
int socket_setopt_reuseaddr(int sk);

int socket_setopt_keepalive(int sk);

int socket_setopt_noblock(int sk);

#endif