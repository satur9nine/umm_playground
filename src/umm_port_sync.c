#include <pthread.h>
#include <stdlib.h>

static pthread_mutex_t g_umm_mutex;
static pthread_once_t g_umm_mutex_once = PTHREAD_ONCE_INIT;

static void umm_mutex_init_once(void) {
    pthread_mutexattr_t attr;
    if (pthread_mutexattr_init(&attr) != 0) {
        abort();
    }
    if (pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE) != 0) {
        pthread_mutexattr_destroy(&attr);
        abort();
    }
    if (pthread_mutex_init(&g_umm_mutex, &attr) != 0) {
        pthread_mutexattr_destroy(&attr);
        abort();
    }
    pthread_mutexattr_destroy(&attr);
}

void umm_port_critical_entry(void) {
    pthread_once(&g_umm_mutex_once, umm_mutex_init_once);
    if (pthread_mutex_lock(&g_umm_mutex) != 0) {
        abort();
    }
}

void umm_port_critical_exit(void) {
    if (pthread_mutex_unlock(&g_umm_mutex) != 0) {
        abort();
    }
}
