#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int thread_count;
int counter;

pthread_mutex_t barrier_mutex;

sem_t count_sem;
sem_t barrier_sem;

void* Thread_work(void* rank)
{
    long my_rank=(long)rank;
    sem_wait(&count_sem);
    if (counter == thread_count - 1) {
        counter = 0;
        sem_post(&count_sem);
        for (int j = 0; j < thread_count-1; j++)
            sem_post(&barrier_sem);
    } else {
        counter++;
        sem_post(&count_sem);
        sem_wait(&barrier_sem);
    }
    if (my_rank == 0) {
        printf("Termino la barrera");
    }
}

int main(int argc,char* argv[])
{
    long thread;
    pthread_t* thread_handles;

    thread_count=strtol(argv[1],NULL,10);
    thread_handles=malloc(thread_count*sizeof(pthread_t));
    counter=0;

    sem_init(&barrier_sem, 0, 0);
    sem_init(&count_sem, 0, 1);

    for(thread=0 ; thread<thread_count ; thread++){
        pthread_create(&thread_handles[thread],NULL,Thread_work,(void*)thread);
    }

    //printf("Hello from the main thread!\n");


    sem_destroy(&count_sem);
    sem_destroy(&barrier_sem);
    for(thread=0 ; thread<thread_count ; thread++){
        pthread_join(thread_handles[thread],NULL);
    }

    free(thread_handles);
    return 0;
}
