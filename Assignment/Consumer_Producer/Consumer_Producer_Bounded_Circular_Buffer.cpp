//  BOUNDED CIRCULAR BUFFER

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include <pthread.h>
#include "semaphore.cpp"

using namespace std;

#define BUFFER_SIZE 10

struct item{
	int data;
} buf[BUFFER_SIZE],t;

Semaphore empty,full,mutex1;
int in=0,out=0;

int produce(pthread_t self){
		int p = 1 + rand()%40;
		printf("\nBuffer position : %d Producer produced %d \n",in,p);
		int x1=full.value;
		printf("Producer Time Buffer : ");
		for(int i=out;x1>0;i=(i+1)%10,x1--)
		{
			printf("%d ",buf[i].data);
			
		}
		printf("%d ",p);
		if(empty.value==0)
		{
			printf(" Buffer Full");
		}
		printf("\n");
		// for( int i=0;i<=full;i++)
		// {
		// 	printf("%d ",buf[i].data);
		// 	printf("\n");
		// }
		return p;
}


void consume(int x,pthread_t self){
		printf("\nBuffer position : %d Consumer consumed %d\n",out,x);
		int x1=full.value;
		printf("Consumer Time Buffer : ");
		for( int i=(out+1)%10;x1>0;i=(i+1)%10,x1--)
		{
			printf("%d ",buf[i].data);
			
		}
		if(full.value==0)
		{
			printf(" Buffer Empty");
		}
		printf("\n");
		// for( int i=0;i<=full;i++)
		// {
		// 	printf("%d ",buf[i].data);
		// 	printf("\n");
		// }
	
}


void* producer(void *args){

	while(1){
		//int p = produce(pthread_self());
		//printf("empty producer\n");
		Semaphore_wait(&empty);
		//printf("mutex1 producer\n");
		Semaphore_wait(&mutex1);
		int p = produce(pthread_self());
		buf[in].data=p;		// critical section
		in=(in+1)%BUFFER_SIZE;
		//printf("mutex1 producer\n");
		Semaphore_signal(&mutex1);
		//printf("full producer\n");
		Semaphore_signal(&full);
		//printf("full---------%d\n",full.value );
		sleep(1 + rand()%3);
		
	}
	return NULL;
}


void* consumer(void *args){
	int x;
	while(1){
		//printf("full consumer\n");
		Semaphore_wait(&full);
		//printf("mutex1 consumer\n");
		Semaphore_wait(&mutex1);
		x=buf[out].data;
		consume(x,pthread_self());
		out=(out+1)%BUFFER_SIZE;
		//printf("mutex1 consumer\n");
		Semaphore_signal(&mutex1);
		//printf("empty consumer\n");
		Semaphore_signal(&empty);
		sleep(1+rand()%5);

	}

	return NULL;
}

int main()
{
	SEM_INIT(&empty,BUFFER_SIZE); 
	//empty->value=10;
	SEM_INIT(&mutex1,1); 
	//mutex1->value=1;
	SEM_INIT(&full,0); 
	//full->value=0;
	int p1,c1;
	srand(time(NULL));
	pthread_t producer1,consumer1;
	p1 = pthread_create(&producer1, NULL, producer, NULL);
	if (p1) {
         exit(-1);
      }
    c1 = pthread_create(&consumer1, NULL, consumer, NULL);
	if (c1) {
         exit(-1);
      }

	(void) pthread_join(producer1, NULL);
    (void) pthread_join(consumer1, NULL);


	return 0;
}