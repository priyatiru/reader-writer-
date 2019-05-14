//**************************************************************************//

// THIS IS THE WRITERS PRIORITY CODE//
// author @priyatiru_

//**************************************************************************//
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<stdlib.h>

sem_t mutex,writeblock,read;
int data = 0,rcount = 0,aw=0,ww=0,wr=0;  //aw is active writer, ww is waiting writer, wr=waiting reader

void *reader(void *arg)
{
  int f;
  f = ((int)arg);
  sem_wait(&mutex);
  if(aw+ww>0)
  wr++;
  else{
    sem_post(&read);
    rcount=rcount+1;
  }
  sem_post(&mutex);
  sem_wait(&read);
  printf("Data read by the reader%d is %d\n",f,data);
  sleep(1);
  sem_wait(&mutex);
  rcount=rcount-1;
  if(rcount==0&&ww>0)
  {
    sem_post(&writeblock);
    aw++;
    ww--;
  }
  sem_post(&mutex);
}

void *writer(void *arg)
{
  int f;
  f = ((int)arg);
  sem_wait(&mutex);
  if(aw+rcount>0)
  ww++;
  else{
    sem_post(&writeblock);
    aw=aw+1;
  }
  sem_post(&mutex);
  sem_wait(&writeblock);
  printf("Data writen by the writer%d is %d\n",f,data);
  sleep(1);
  sem_wait(&mutex);
  aw=aw-1;
  if(ww>0)
  {
    sem_post(&writeblock);
    aw++;
    ww--;
  }
  else if(wr>0)
  {
    sem_post(&read);
    rcount++;
    wr++;
  }
  sem_post(&mutex);
}

main()
{
  int i,b;
  pthread_t rtid[5],wtid[5];
  sem_init(&mutex,0,1);
  sem_init(&writeblock,0,1);
  for(i=0;i<=5;i++)
  {
    pthread_create(&wtid[i],NULL,writer,(void *)i);
    pthread_create(&rtid[i],NULL,reader,(void *)i);
  }
  for(i=0;i<=5;i++)
  {
    pthread_join(wtid[i],NULL);
    pthread_join(rtid[i],NULL);
  }
}
