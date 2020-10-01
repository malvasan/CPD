#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int thread_count;
long long n;
double sum=0.0;
long flag=0;

pthread_mutex_t mutex;

/*void* Thread_sum(void* rank)
{
    long my_rank=(long)rank;
    double factor;
    long long i;
    long long my_n=n/thread_count;
    long long my_first_i=my_n*my_rank;
    long long my_last_i= my_first_i + my_n;

    if(my_first_i % 2 == 0)
        factor=1.0;
    else
        factor=-1.0;
    for(i=my_first_i; i<my_last_i; i++,factor=-factor){
        sum+=factor/(2*i+1);
    }
    return NULL;
}*/
/*
void* Thread_sum(void* rank)
{
    long my_rank=(long)rank;
    double factor;
    long long i;
    long long my_n=n/thread_count;
    long long my_first_i=my_n*my_rank;
    long long my_last_i= my_first_i + my_n;
    double my_sum;

    if(my_first_i % 2 == 0)
        factor=1.0;
    else
        factor=-1.0;

    for(i=my_first_i; i<my_last_i; i++,factor=-factor){
        my_sum+=factor/(2*i+1);
    }

    while(flag != my_rank);
    sum+=my_sum;
    flag=(flag+1)%thread_count;

    return NULL;
}*/

void* Thread_sum(void* rank)
{
    long my_rank=(long)rank;
    double factor;
    long long i;
    long long my_n=n/thread_count;
    long long my_first_i=my_n*my_rank;
    long long my_last_i= my_first_i + my_n;
    double my_sum;

    if(my_first_i % 2 == 0)
        factor=1.0;
    else
        factor=-1.0;

    for(i=my_first_i; i<my_last_i; i++,factor=-factor){
        my_sum+=factor/(2*i+1);
    }

    pthread_mutex_lock(&mutex);
    sum+=my_sum;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc,char* argv[])
{
    long thread;
    pthread_t* thread_handles;

    thread_count=strtol(argv[1],NULL,10);
    thread_handles=malloc(thread_count*sizeof(pthread_t));

    for(thread=0 ; thread<thread_count ; thread++){
        pthread_create(&thread_handles[thread],NULL,Thread_sum,(void*)thread);
    }

    printf("Hello from the main thread!\n");

    for(thread=0 ; thread<thread_count ; thread++){
        pthread_join(thread_handles[thread],NULL);
    }

    free(thread_handles);
    double pi=4.0*sum;
    printf("pi: %f\n",pi);
    return 0;
}
