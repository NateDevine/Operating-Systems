
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define N       200
#define HALF    100
#define SPACES    8

void *producer(int *nothing);
void *consumer(int *nothing);

int buffer[SPACES];
sem_t spaces, items, mutex;
int nthreads=0;

int main()
{
  for(int i=0;i<SPACES;i++) buffer[i]=0;
  pthread_t th[N+1];
  int i;
  int procount=0, concount=0;
  int id[N];
  
  sem_init(&spaces, 0, SPACES);
  sem_init(&items, 0, 0);
  sem_init(&mutex, 0, 1);

  srandom(getpid());

  for(i=1;i<=N;i++){
    id[i] = i;
    if(procount == HALF){
      pthread_create(&th[i], 0, (void *) consumer, &id[i]);
      concount++;
    } else if(concount == HALF){
      pthread_create(&th[i], 0, (void *) producer, &id[i]);
      procount++;
    } else if(random() % 100 < 50){
      pthread_create(&th[i], 0, (void *) producer, &id[i]);
      procount++;
    } else {
      pthread_create(&th[i], 0, (void *) consumer, &id[i]);
      concount++;
    }
  }

  for(i=1;i<=N;i++)
     pthread_join(th[i], 0);

}
void do_work()
{
  usleep(1 + rand() % 100);
}

void *producer(int *id){
  sem_wait(&spaces);
  sem_wait(&mutex);
  buffer[nthreads++]=*id;
  do_work();
  printf("Thread %d (producer) stored item %d from buffer\n", *id, *id);
  sem_post(&items);
  sem_post(&mutex); 
}

void *consumer(int *id){
  sem_wait(&items);
  sem_wait(&mutex);
  int temp=buffer[--nthreads];
  buffer[nthreads]=0;
  do_work();
  printf("Thread %d (consumer) removed item %d from buffer\n", *id, temp);
  sem_post(&mutex);
  sem_post(&spaces);
}