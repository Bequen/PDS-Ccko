#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

char resource[150];
volatile int count = 0;

pthread_mutex_t mutex_obj = PTHREAD_MUTEX_INITIALIZER;
pthread_spinlock_t spinlock_obj;
pthread_rwlock_t rwlock_obj = PTHREAD_RWLOCK_INITIALIZER;
sem_t sem_obj;
pthread_barrier_t barrier_obj;
pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;
pthread_mutex_t cond_mutex = PTHREAD_MUTEX_INITIALIZER;

void* no_synchronization(void* param) {
    int number = *(int*)param;
    pthread_t thread_id = pthread_self();
    sleep(rand() % 2);
    snprintf(resource, sizeof(resource), "Thread %lu wrote %d", thread_id, number);
    sleep(rand() % 2);
    printf("%s\n", resource);
    return NULL;
}

void* mutex(void* param) {
    int number = *(int*)param;
    pthread_t thread_id = pthread_self();

    pthread_mutex_lock(&mutex_obj);
    sleep(rand() % 2);
    snprintf(resource, sizeof(resource), "Thread %lu wrote %d", thread_id, number);
    sleep(rand() % 2);
    printf("%s\n", resource);
    pthread_mutex_unlock(&mutex_obj);

    return NULL;
}

void* spin_lock(void* param) {
    int number = *(int*)param;
    pthread_t thread_id = pthread_self();

    pthread_spin_lock(&spinlock_obj);
    sleep(rand() % 2);
    snprintf(resource, sizeof(resource), "Thread %lu wrote %d", thread_id, number);
    sleep(rand() % 2);
    pthread_spin_unlock(&spinlock_obj);

    return NULL;
}

void* write_lock(void* param) {
    int number = *(int*)param;
    pthread_t thread_id = pthread_self();

    pthread_rwlock_wrlock(&rwlock_obj);
    printf("Thread %lu is writing\n", thread_id);
    snprintf(resource, sizeof(resource), "Thread %lu wrote %d", thread_id, number);
    sleep(1);
    printf("Thread %lu stopped writing\n", thread_id);
    pthread_rwlock_unlock(&rwlock_obj);

    return NULL;
}

void* read_resource(void* param) {
    pthread_t thread_id = pthread_self();
    for (int i = 0; i < 5; i++) {
		pthread_rwlock_rdlock(&rwlock_obj);
        printf("READER %lu: %s\n", thread_id, resource);
		pthread_rwlock_unlock(&rwlock_obj);
        sleep(1);
    }

    return NULL;
}

void* semaphore(void* param)
{
    int number = *(int*)param;
    pthread_t thread_id = pthread_self();

    sem_wait(&sem_obj);
    sleep(rand() % 2);
    snprintf(resource, sizeof(resource), "Thread %lu wrote %d", thread_id, number);
    sleep(rand() % 2);
    printf("%s\n", resource);
    sem_post(&sem_obj);

    return NULL;
}

void* barrier(void* param) {
    pthread_t thread_id = pthread_self();

    sleep(rand() % 2);
    printf("Thread %lu is working\n", thread_id);
    sleep(rand() % 2);

    printf("Thread %lu reached barrier and is waiting for others\n", thread_id);
    pthread_barrier_wait(&barrier_obj);
    printf("Thread %lu continues its work\n", thread_id);

    return NULL;
}

void* thread1_func(void* param) {
    pthread_t thread_id = pthread_self();

    printf("Thread %lu is doing some work\n", thread_id);
    sleep(1);

    pthread_mutex_lock(&cond_mutex);
    printf("Thread %ld is waiting for count to be greater or equal to 5\n", thread_id);
    while (count < 5) {
        pthread_cond_wait(&cond_var, &cond_mutex);
    }
    pthread_mutex_unlock(&cond_mutex);

    printf("Thread %lu is continuing its work\n", thread_id);

    return NULL;
}

void* thread2_func(void* param) {
    pthread_t thread_id = pthread_self();

    printf("Thread %lu is doing some work\n", thread_id);
    sleep(3);

    for (int i = 0; i < 5; i++)
    {
        pthread_mutex_lock(&cond_mutex);
        count++;
        printf("Thread %ld increases count to %d\n", thread_id, count);
        pthread_cond_signal(&cond_var);
        pthread_mutex_unlock(&cond_mutex);
        sleep(1);
    }

    printf("Thread %lu is continuing its work\n", thread_id);

    return NULL;
}

void condition_variable() {
    pthread_t thread1, thread2;

    if (pthread_create(&thread1, NULL, thread1_func, NULL) != 0) {
        perror("Error creating thread1");
        return;
    }

    if (pthread_create(&thread2, NULL, thread2_func, NULL) != 0) {
        perror("Error creating thread2");
        return;
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_mutex_destroy(&cond_mutex);
    pthread_cond_destroy(&cond_var);
}

int main() {
    pthread_t threads[10];
    int t_params[10];

    printf("No synchronization:\n");
    for (int i = 0; i < 10; i++) {
        t_params[i] = i;
        pthread_create(&threads[i], NULL, no_synchronization, (void*)&t_params[i]);
    }
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\nMutex:\n");
    for (int i = 0; i < 10; i++) {
        t_params[i] = i;
        pthread_create(&threads[i], NULL, mutex, (void*)&t_params[i]);
    }
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&mutex_obj);

    printf("\nSpin lock:\n");
    pthread_spin_init(&spinlock_obj, 0);
    for (int i = 0; i < 10; i++) {
        t_params[i] = i;
        pthread_create(&threads[i], NULL, spin_lock, (void*)&t_params[i]);
    }
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }
    // pthread_spin_destroy(&spinlock_obj);

    printf("\nRead-write lock:\n");
    pthread_t read1_thread;
    pthread_t read2_thread;
    pthread_create(&read1_thread, NULL, read_resource, NULL);
    pthread_create(&read2_thread, NULL, read_resource, NULL);
    for (int i = 0; i < 5; i++) {
        t_params[i] = i;
        pthread_create(&threads[i], NULL, write_lock, (void*)&t_params[i]);
    }
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }
    pthread_join(read1_thread, NULL);
    pthread_join(read2_thread, NULL);
    pthread_rwlock_destroy(&rwlock_obj);

    printf("\nSemaphore:\n");
    sem_init(&sem_obj, 0, 1);
    for (int i = 0; i < 10; i++) {
        t_params[i] = i;
        pthread_create(&threads[i], NULL, semaphore, (void*)&t_params[i]);
    }
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }
    sem_destroy(&sem_obj);

    printf("\nBarrier:\n");
    pthread_barrier_init(&barrier_obj, NULL, 10);
    for (int i = 0; i < 10; i++) {
        t_params[i] = i;
        pthread_create(&threads[i], NULL, barrier, (void*)&t_params[i]);
    }
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }
    pthread_barrier_destroy(&barrier_obj);

    printf("\nCondition variable:\n");
    condition_variable();
    pthread_cond_destroy(&cond_var);
    pthread_mutex_destroy(&cond_mutex);

    return 0;
}