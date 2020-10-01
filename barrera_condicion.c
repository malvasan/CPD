#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int thread_count;
int counter;

pthread_mutex_t mutex;
pthread_cond_t cond_var;

void* Thread_work(void* rank)
{
    long my_rank=(long)rank;
    pthread_mutex_lock(&mutex);
    counter++;
    if (counter == thread_count){
        counter=0;
        pthread_cond_broadcast(&cond_var);
    }
    else{
        while(pthread_cond_wait(&cond_var, &mutex)!=0);
    }
    pthread_mutex_unlock(&mutex);

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

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_var, NULL);

    for(thread=0 ; thread<thread_count ; thread++){
        pthread_create(&thread_handles[thread],NULL,Thread_work,(void*)thread);
    }




    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_var);
    for(thread=0 ; thread<thread_count ; thread++){
        pthread_join(thread_handles[thread],NULL);
    }

    free(thread_handles);
    return 0;
}
