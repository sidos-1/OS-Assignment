// Sleeping Barber Problem's solution using Semaphores


#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include<bits/stdc++.h>
#include "semaphore.cpp"

#define SEATS 5
int available = SEATS;
int wait=0;
int id=0;

// Semaphores
Semaphore mutex;   
Semaphore customers;
Semaphore barber;

void hair_cut_done()
{
	sleep(1 + rand()%10);
	printf("\nHair Cut Done.\t\t Waiting Customers: %d\n\n",wait);
}

void* barber_func(void* arg)
{
	while(1){
			
			Semaphore_wait(&customers);
			Semaphore_wait(&mutex);
			available++;
			wait--;
			Semaphore_signal(&barber);
			printf("A customer goes to barber for cutting.\n");
			Semaphore_signal(&mutex);
			hair_cut_done();
			Semaphore_wait(&barber);
		}

	return NULL;
}

void* customer_func(void* arg)
{

	while(true) {
            Semaphore_wait(&mutex);
            if(available>0)
            {
            	available--;
            	wait++;
            	id++;
            	printf("Customer (%d) joining the waiting room\t Waiting Customers:%d \n",id,wait );
            	Semaphore_signal(&customers);
            	Semaphore_signal(&mutex);
            	//Semaphore_wait(&barber);
            }
            else{
            	Semaphore_signal(&mutex);
            }
            sleep(1+rand()%5);
      }
}

int main(int argc, char* arvg[])
{

	SEM_INIT(&mutex, 1);
	SEM_INIT(&barber, 0);
	SEM_INIT(&customers, 0);


	//int r=rand()%5;
	int r=rand()%3;

	pthread_t barber_thread,customer_thread[r];

	int b,c[r];
	srand(time(NULL));

			b = pthread_create(&barber_thread, NULL, barber_func, NULL);
			if (b) {
		         exit(-1);
		      }

    for(int i=0;i<r;i++)
    {
		    c[i] = pthread_create(&customer_thread[i], NULL, customer_func, NULL);
			if (c[i]) {
		         exit(-1);
		      }
    }
    
	(void) pthread_join(barber_thread, NULL);
	for(int i=0;i<r;i++)
	{
		(void) pthread_join(customer_thread[i], NULL);	
	}

	return 0;
}