#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cv_read;
    pthread_cond_t cv_write;
    uint64_t count_readers;
    uint64_t count_waiting_readers;
    uint64_t count_waiting_writers;
    uint8_t is_writer_active; // 0 - NO, 1 - YES
    pthread_t rank_writer;
} my_rwlock_t;

void print_rwlock_t(my_rwlock_t* lock) {
    printf("%llu ", lock->count_readers);
    printf("%llu ", lock->count_waiting_readers);
    printf("%llu ", lock->count_waiting_writers);
    printf("%u ", lock->is_writer_active);
    printf("%lu\n", (unsigned long)lock->rank_writer);
}

int32_t rwlock_init(my_rwlock_t* lock) {
    int rc;
    if (lock == NULL) return EINVAL;
    if ((rc = pthread_mutex_init(&lock->mutex, NULL)) != 0) return rc;
    if ((rc = pthread_cond_init(&lock->cv_read, NULL)) != 0) {
        pthread_mutex_destroy(&lock->mutex);
        return rc;
    }
    if ((rc = pthread_cond_init(&lock->cv_write, NULL)) != 0) {
        pthread_mutex_destroy(&lock->mutex);
        pthread_cond_destroy(&lock->cv_read);
        return rc;
    }
    lock->count_readers = 0;
    lock->count_waiting_readers = 0;
    lock->count_waiting_writers = 0;
    lock->is_writer_active = 0;
    return 0;
}

int32_t rwlock_destroy(my_rwlock_t* lock) {
    int rc;
    if (lock == NULL) return EINVAL;
    if ((rc = pthread_mutex_lock(&lock->mutex)) != 0) return rc;
    if (lock->count_readers != 0 || lock->is_writer_active ||
        lock->count_waiting_readers != 0 || lock->count_waiting_writers != 0) {
        pthread_mutex_unlock(&lock->mutex);
        return EBUSY;
    }
    pthread_mutex_unlock(&lock->mutex);
    if ((rc = pthread_cond_destroy(&lock->cv_read)) != 0) return rc;
    if ((rc = pthread_cond_destroy(&lock->cv_write)) != 0) return rc;
    return pthread_mutex_destroy(&lock->mutex);
}

int32_t rwlock_rdlock(my_rwlock_t* lock) {
    int rc;
    if ((rc = pthread_mutex_lock(&lock->mutex)) != 0) return rc;
    if (lock->is_writer_active || lock->count_waiting_writers > 0) {
        lock->count_waiting_readers++;
        while (lock->is_writer_active || lock->count_waiting_writers > 0) {
            if ((rc = pthread_cond_wait(&lock->cv_read, &lock->mutex)) != 0) {
                lock->count_waiting_readers--;
                pthread_mutex_unlock(&lock->mutex);
                return rc;
            }
        }
        lock->count_waiting_readers--;
    }
    lock->count_readers++;
    return pthread_mutex_unlock(&lock->mutex);
}

int32_t rwlock_wrlock(my_rwlock_t* lock) {
    int rc;
    if ((rc = pthread_mutex_lock(&lock->mutex)) != 0) return rc;
    if (lock->is_writer_active || lock->count_readers > 0) {
        lock->count_waiting_writers++;
        while (lock->is_writer_active || lock->count_readers > 0) {
            if ((rc = pthread_cond_wait(&lock->cv_write, &lock->mutex)) != 0) {
                lock->count_waiting_writers--;
                pthread_mutex_unlock(&lock->mutex);
                return rc;
            }
        }
        lock->count_waiting_writers--;
    }
    lock->is_writer_active = 1;
    lock->rank_writer = pthread_self();
    print_rwlock_t(lock); // test
    return pthread_mutex_unlock(&lock->mutex);
}

// int32_t rwlock_unlock(my_rwlock_t* lock) {
//     int rc;
//     print_rwlock_t(lock);
//     if ((rc = pthread_mutex_lock(&lock->mutex)) != 0) {
//         printf("rc=%d\n", rc);
//         return rc;
//     }
//     if (lock->is_writer_active) {
//         lock->is_writer_active = 0;
//     } else if (lock->count_readers > 0) {
//         lock->count_readers--;
//     } else {
//         pthread_mutex_unlock(&lock->mutex);
//         return EPERM;
//     }
//     if (lock->count_waiting_writers > 0 && 
//         !lock->is_writer_active && lock->count_readers == 0) {
//         if ((rc = pthread_cond_signal(&lock->cv_write)) != 0) {
//             pthread_mutex_unlock(&lock->mutex);
//             return rc;
//         }
//     }
//     else if (lock->count_waiting_writers == 0 && lock->count_waiting_readers > 0) {
//         if ((rc = pthread_cond_broadcast(&lock->cv_read)) != 0) {
//             pthread_mutex_unlock(&lock->mutex);
//             return rc;
//         }
//     }
//     return pthread_mutex_unlock(&lock->mutex);
// }

int32_t rwlock_unlock(my_rwlock_t* lock) {
    int rc;
    if ((rc = pthread_mutex_lock(&lock->mutex)) != 0) return rc;
    if (lock->is_writer_active) {
        if (!pthread_equal(lock->rank_writer, pthread_self())) {
            pthread_mutex_unlock(&lock->mutex);
            return EPERM;
        }
        pthread_t id = pthread_self();
        printf("%lu %lu\n", (unsigned long)id, (unsigned long)lock->rank_writer);

        lock->is_writer_active = 0;
    }
    // if (lock->is_writer_active && lock->rank_writer == pthread_self()) {
        // printf("%lu %lu\n", (unsigned long)id, (unsigned long)lock->rank_writer);
        // lock->is_writer_active = 0;
    // } 
    else if (lock->count_readers > 0) {
        lock->count_readers--;
    } else {
        pthread_mutex_unlock(&lock->mutex);
        return EPERM;
    }
    if (lock->count_waiting_writers > 0 && 
        !lock->is_writer_active && lock->count_readers == 0) {
        if ((rc = pthread_cond_signal(&lock->cv_write)) != 0) {
            pthread_mutex_unlock(&lock->mutex);
            return rc;
        }
    }
    else if (lock->count_waiting_writers == 0 && lock->count_waiting_readers > 0) {
        if ((rc = pthread_cond_broadcast(&lock->cv_read)) != 0) {
            pthread_mutex_unlock(&lock->mutex);
            return rc;
        }
    }
    return pthread_mutex_unlock(&lock->mutex);
}

my_rwlock_t lock;
pthread_mutex_t mutex;
sem_t writer_locked;
sem_t try_unlock;

void* routine1(void* arg) {
    int rc = rwlock_wrlock(&lock);
    printf("thread 1: wrlock = %d\n", rc);
    
    sem_post(&writer_locked);
    sem_wait(&try_unlock);

    sleep(1);
    
    rc = rwlock_unlock(&lock);
    printf("thread 1: unlock = %d\n", rc);
    return NULL;
}

void* routine2(void* arg) {
    sem_wait(&writer_locked);
    printf("thread 2: trying unlock\n");
    
    int rc = rwlock_unlock(&lock);
    
    printf("thread 2: unlock = %d\n", rc);
    sem_post(&try_unlock);
    return NULL;
}

int main(void) {
    pthread_t thread1;
    pthread_t thread2;

    rwlock_init(&lock);

    sem_init(&writer_locked, 0, 0);
    sem_init(&try_unlock, 0, 0);

    pthread_create(&thread1, NULL, routine1, NULL);
    pthread_create(&thread2, NULL, routine2, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    sem_destroy(&writer_locked);
    sem_destroy(&try_unlock);

    rwlock_destroy(&lock);

    return 0;
}
