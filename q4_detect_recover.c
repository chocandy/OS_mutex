#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
int done1 = 0;
int done2 = 0;

void* thread1_func(void* arg) {
    printf("Thread 1: trying to lock mutex1\n");
    pthread_mutex_lock(&mutex1);
    printf("Thread 1: locked mutex1\n");

    sleep(1);

    printf("Thread 1: trying to lock mutex2\n");
    if (pthread_mutex_trylock(&mutex2) == 0) {
        printf("Thread 1: locked mutex2\n");
        printf("Thread 1: working...\n");
        pthread_mutex_unlock(&mutex2);
    } else {
        printf("Thread 1: failed to lock mutex2\n");
    }

    pthread_mutex_unlock(&mutex1);
    done1 = 1;
    return NULL;
}

void* thread2_func(void* arg) {
    printf("Thread 2: trying to lock mutex2\n");
    pthread_mutex_lock(&mutex2);
    printf("Thread 2: locked mutex2\n");

    sleep(1);

    printf("Thread 2: trying to lock mutex1\n");
    if (pthread_mutex_trylock(&mutex1) == 0) {
        printf("Thread 2: locked mutex1\n");
        printf("Thread 2: working...\n");
        pthread_mutex_unlock(&mutex1);
    } else {
        printf("Thread 2: failed to lock mutex1\n");
    }

    pthread_mutex_unlock(&mutex2);
    done2 = 1;
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);

    pthread_create(&t1, NULL, thread1_func, NULL);
    pthread_create(&t2, NULL, thread2_func, NULL);

    sleep(3);

    if (!done1 || !done2) {
        printf("Deadlock detected or possible deadlock situation!\n");
        printf("Recovering by terminating program.\n");
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);

    return 0;
}
