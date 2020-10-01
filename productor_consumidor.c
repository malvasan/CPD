#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
int thread_count;
sem_t* semaphores;
char** messages;
const int MSG_MAX = 100;


void* Send_msg(void* rank)
{
    long my_rank=(long) rank;
    long dest= (my_rank+1)%thread_count;
    char* my_msg= (char*)malloc(MSG_MAX*sizeof(char));


    sprintf(my_msg, "Hello to %ld from %ld", dest, my_rank);
    messages[dest]=my_msg;
    sem_post(&semaphores[dest]);
    
    sem_wait(&semaphores[my_rank]);
    printf("Thread %ld > %s\n", my_rank, messages[my_rank]);

    return NULL;
}

int main(int argc,char* argv[])
{
    long thread;
    pthread_t* thread_handles;
	
    thread_count=strtol(argv[1],NULL,10);
    messages = malloc(thread_count*sizeof(char*));
    semaphores = malloc(thread_count*sizeof(sem_t));
    thread_handles=malloc(thread_count*sizeof(pthread_t));

    for (thread = 0; thread < thread_count; thread++) {
      messages[thread] = NULL;
      sem_init(&semaphores[thread], 0, 0);
   }

    for(thread=0 ; thread<thread_count ; thread++){
        pthread_create(&thread_handles[thread],(pthread_attr_t*)NULL,Send_msg,(void*)thread);
    }


    for(thread=0 ; thread<thread_count ; thread++){
        pthread_join(thread_handles[thread],NULL);
    }

    for(thread=0 ; thread<thread_count ; thread++){
        free(messages[thread]);
        sem_destroy(&semaphores[thread]);
    }
    free(messages);
    free(semaphores);
    free(thread_handles);
    return 0;
}
