#include "thread.h"

struct thread tpool[NTHREAD], *tptr = tpool;

/* 对真正执行的函数做了简单的装饰 */
void *wrapper(void *arg) {
    struct thread *thread = (struct thread *)arg;
    thread->entry(thread->id);
    return NULL;
}

/* 根据函数句柄创建线程 */
void thread_create(void *fn) {
    assert(tptr - tpool < NTHREAD);
    *tptr = (struct thread){
        .id = tptr - tpool + 1,
        .status = T_LIVE,
        .entry = fn,
    };
    pthread_create(&(tptr->thread), NULL, wrapper, tptr);
    ++tptr;
}

/* 等待线程池的线程执行 */
void thread_join() {
    for (int i = 0; i < NTHREAD; i++) {
        struct thread *t = &tpool[i];
        if (t->status == T_LIVE) {
            pthread_join(t->thread, NULL);
            t->status = T_DEAD;
        }
    }
}

/* 由编译器自动释放 */
__attribute__((destructor)) void cleanup() {
    thread_join();
}
