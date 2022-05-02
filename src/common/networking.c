#include "networking.h"

/* SelectStruct结构初始化 */
int select_ini(SelectListStruct *sSelectList) {
    if (sSelectList == NULL) {
        LOG_ERROR("select_ini: sSelectList is NULL");
        return -1;
    }
    sSelectList->sk_num = 0;
    return 0;
}

/* 将socket添加到SelectList中 */
int select_add(SelectListStruct *sSelectList, int sk) {
    if (sSelectList == NULL) {
        LOG_ERROR("select_add: sSelectList is NULL");
        return -1;
    }

    if (sSelectList->sk_num >= MAX_CONNECTION) {
        LOG_ERROR("select_add: too many connections\n");
        return -1;
    }
    sSelectList->sks[sSelectList->sk_num++] = sk;
    return sk;
}

/* 从SelectList中删除对应的socket */
int select_del(SelectListStruct *sSelectList, int sk) {
    if (sSelectList == NULL) {
        LOG_ERROR("select_del: sSelectList is NULL");
        return -1;
    }
    int idx;
    for (idx = 0; idx < sSelectList->sk_num; idx++) {
        if (sSelectList->sks[idx] == sk)
            break;
    }

    if (idx == sSelectList->sk_num) {
        LOG_ERROR("select_del: socket not found\n");
        return -1;
    }

    for (; idx < sSelectList->sk_num; idx++) {
        sSelectList->sks[idx] = sSelectList->sks[idx + 1];
    }

    --sSelectList->sk_num;
    return sk;
}

/* 创建一个socket */
int socket_create(int domain, int type, int protocol) {
    int sk = socket(domain, type, protocol);
    if (sk < 0) {
        LOG_ERROR("socket_create: socket error\n");
        return -1;
    }
    return sk;
}

/* 请求监听 */
int socket_listen(int sk, int listen_count) {
    LOG_INFO("socket_listen: sk = %d, listen_count = %d\n", sk, listen_count);

    if (listen(sk, listen_count) < 0) {
        LOG_ERROR("socket_listen: listen error\n");
        return -1;
    }
    return 0;
}

/* 绑定地址 */
int socket_bind(int sk, SocketAddrStruct *sAddr, SelectListStruct *sSelectList) {
    LOG_INFO("socket_bind: sk = %d, sAddr = %p\n", sk, sAddr);

    if (bind(sk, (struct sockaddr *) &sAddr->sa_in, sAddr->addrlen) < 0) {
        LOG_ERROR("socket_bind: bind error\n");
        return -1;
    }

    if (select_add(sSelectList, sk) < 0) {
        LOG_ERROR("socket_bind: select_add error\n");
        return -1;
    }

    return 0;
}

/* 接受请求 */
int socket_accept(int sk, SelectListStruct *sSelectList) {
    LOG_INFO("socket_accept: sk = %d\n", sk);
    int connfd;
    SocketAddrStruct client_addr;

    if (sSelectList == NULL) {
        LOG_ERROR("socket_accept: sSelectList is NULL");
        return -1;
    }

    if (sSelectList->sk_num >= MAX_CONNECTION) {
        LOG_ERROR("socket_accept: too many connections\n");
        return -1;
    }

    if ((connfd = accept(sk, (struct sockaddr *) &client_addr.sa_in, &client_addr.addrlen)) < 0) {
        LOG_ERROR("socket_accept: accept error\n");
        return -1;
    }

    LOG_INFO("socket_accept: connfd = %d\n", connfd);

    if (select_add(sSelectList, connfd) < 0) {
        LOG_ERROR("socket_accept: select_add error\n");
        return -1;
    }
    return connfd;
}

/* 建立连接 */
int socket_connect(int sk, SocketAddrStruct *sAddr, SelectListStruct *sSelectList) {
    // write your code here
    return 0;
}

/* 释放socket */
int socket_close(int sk, SelectListStruct *sSelectList) {
    LOG_INFO("socket_close: sk = %d\n", sk);
    if (sSelectList == NULL) {
        LOG_ERROR("socket_close: sSelectList is NULL");
        return -1;
    }

    if (select_del(sSelectList, sk) < 0) {
        LOG_ERROR("socket_close: select_del error\n");
        return -1;
    }

    if (close(sk) < 0) {
        LOG_ERROR("socket_close: close error\n");
        return -1;
    }
    return 0;
}

/* 读取tcp数据 */
int socket_tcp_read(int sk, char *buf, int len) {
    // write your code here
    return 0;
}

/* 读取udp数据 */
int socket_udp_read(int sk, char *buf, int len) {
    // write your code here
    return 0;
}

/* 写入tcp数据 */
int socket_tcp_write(int sk, char *buf, int len) {
    // write your code here
    return 0;
}

/* 写入udp数据 */
int socket_udp_write(int sk, char *buf, int len) {
    // write your code here
    return 0;
}

/* 内核轮询SelectList中的socket，返回有数据的socket数量并且设置标志位 */
int socket_select(fd_set *fds, SelectListStruct *sSelectList, struct timeval *timeout) {
    FD_ZERO(fds);
    int maxfd = 0, idx = 0;

    for (; idx < sSelectList->sk_num; ++idx) {
        FD_SET(sSelectList->sks[idx], fds);
        if (sSelectList->sks[idx] > maxfd) {
            maxfd = sSelectList->sks[idx];
        }
    }
    return select(maxfd + 1, fds, NULL, NULL, timeout);
}

/* socket选项配置：地址复用 */
int socket_setopt_reuseaddr(int sk) {
    LOG_INFO("socket_setopt_reuseaddr: sk = %d\n", sk);
    int optval = 1;
    if (setsockopt(sk, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        LOG_ERROR("socket_setopt_reuseaddr: setsockopt error\n");
        return -1;
    }
    return 0;
}

/* socket选项配置：保持连接 */
int socket_setopt_keepalive(int sk) {
    LOG_INFO("socket_setopt_keepalive: sk = %d\n", sk);
    int optval = 1;
    if (setsockopt(sk, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval)) < 0) {
        LOG_ERROR("socket_setopt_keepalive: setsockopt error\n");
        return -1;
    }
    return 0;
}

/* socket选项配置：IO非阻塞 */
int socket_setopt_noblock(int sk) {
    LOG_INFO("socket_setopt_noblock: sk = %d\n", sk);
    int flags;
    if ((flags = fcntl(sk, F_GETFL, 0)) < 0) {
        LOG_ERROR("socket_setopt_noblock: fcntl error\n");
        return -1;
    }
    if (fcntl(sk, F_SETFL, flags | O_NONBLOCK) < 0) {
        LOG_ERROR("socket_setopt_noblock: fcntl error\n");
        return -1;
    }
    return 0;
}

