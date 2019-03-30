// Dining Philosophers Problem's solution using Semaphores


#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include<bits/stdc++.h>
#include "semaphore.cpp"
  
#define N 5  
#define l (p + 4) % N 
#define r (p + 1) % N 


//SEMAPHORES
Semaphore mutex; 
Semaphore philosopher[N]; 


  
int state[N];       // EATING:0, HUNGRY:1, THINKING:2
int l1[N]={0,1,2,3,4};

void check(int p) 
{ 
    if (state[p] == 1 && state[l] != 0 && state[r] != 0) 
    { 
        state[p] = 0;
        printf("Philosopher %d takes fork %d and %d\n", p + 1, l + 1, p + 1); 
        printf("Philosopher %d: Eating\n", p + 1); 
        Semaphore_signal(&philosopher[p]); 
    } 
} 

void entry(int p) 
{ 
  
    Semaphore_wait(&mutex); 
    state[p] = 1; 
    printf("Philosopher %d: Hungry\n", p + 1); 
    check(p); 
    Semaphore_signal(&mutex); 
    Semaphore_wait(&philosopher[p]);   
    sleep(2); 
} 
 
void exit1(int p) 
{ 
  
    Semaphore_wait(&mutex);  
    state[p] = 2; 
    printf("Philosopher %d putting fork %d and %d down\n", p + 1, l + 1, p + 1); 
    printf("Philosopher %d: Thinking\n", p + 1); 
    check(l); 
    check(r); 
    Semaphore_signal(&mutex);
    sleep(2);  
} 
  
void* philospher1(void* arg) 
{ 
    while (1) {
        sleep(rand()%5);
        entry(0);
        sleep(rand()%5); 
        exit1(0);
        sleep(rand()%5);  
    } 
} 

void* philospher2(void* arg) 
{ 
    while (1) {
        sleep(rand()%5);
        entry(1);
        sleep(rand()%5); 
        exit1(1);
        sleep(rand()%5);  
    } 
}

void* philospher3(void* arg) 
{ 
    while (1) {
        sleep(rand()%5);
        entry(2);
        sleep(rand()%5); 
        exit1(2);
        sleep(rand()%5);  
    } 
}

void* philospher4(void* arg) 
{ 
    while (1) {
        sleep(rand()%5);
        entry(3);
        sleep(rand()%5); 
        exit1(3);
        sleep(rand()%5);  
    } 
}

void* philospher5(void* arg) 
{ 
    while (1) {
        sleep(rand()%5);
        entry(4);
        sleep(rand()%5); 
        exit1(4);
        sleep(rand()%5);  
    } 
}
  
int main() 
{ 
    pthread_t Philosopher[N];
    SEM_INIT(&mutex,1); 
    for (int i = 0; i < N; i++) 
    {
        SEM_INIT(&philosopher[i],0); 
    }


        pthread_create(&Philosopher[0], NULL,philospher1, NULL); 
        printf("Philosopher %d is thinking\n", 1);
        pthread_create(&Philosopher[1], NULL,philospher2, NULL); 
        printf("Philosopher %d is thinking\n", 2);
        pthread_create(&Philosopher[2], NULL,philospher3, NULL); 
        printf("Philosopher %d is thinking\n", 3);
        pthread_create(&Philosopher[3], NULL,philospher4, NULL); 
        printf("Philosopher %d is thinking\n", 4);
        pthread_create(&Philosopher[4], NULL,philospher5, NULL); 
        printf("Philosopher %d is thinking\n", 5);

  
    for (int i = 0; i < N; i++) 
    {
        pthread_join(Philosopher[i], NULL); 
    }
} 