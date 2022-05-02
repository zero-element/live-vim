#ifndef LIVE_VIM_THREAD_H
#define LIVE_VIM_THREAD_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdatomic.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>

#define NTHREAD 64

/* 简易的线程状态 */
enum { T_FREE = 0,
       T_LIVE,
       T_DEAD,
};

/* 线程结构体 */
struct thread {
    int id, status;
    pthread_t thread;
    void (*entry)(int);
};

void thread_create(void *fn);
void thread_join();

#endif // LIVE_VIM_THREAD_H
