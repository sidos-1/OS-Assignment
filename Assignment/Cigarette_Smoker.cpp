// Cigarette Smoker Problem's solution using Semaphores


#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include<bits/stdc++.h>
#include "semaphore.cpp"

// Semaphores
Semaphore Tobacco_Paper;
Semaphore Paper_Matches;
Semaphore Matches_Tobacco;
Semaphore DoneSmoking;
Semaphore lock1;
Semaphore agent;


void smoke()
{
	printf("Done Smoking.\n");
}

void* agent_func(void* arg)
{
	while(1){
			
			Semaphore_wait(&agent);
			Semaphore_wait(&DoneSmoking);
			Semaphore_wait(&lock1);
			int r=rand()%3;
			switch( r ) {
		            case 0: Semaphore_signal(&Tobacco_Paper);
		            		printf("On Table : Tobacco and Paper\n");
		            		break;
		            case 1: Semaphore_signal(&Paper_Matches);
		            		printf("On Table : Matches and Paper\n");
		            break;
		            case 2: Semaphore_signal(&Matches_Tobacco);
				            printf("On Table : Matches and Tobacco\n");
				            break;
		      }
		    Semaphore_signal(&lock1);
		    sleep(1);
		}

	return NULL;
}

//smoker_A has Matches
//smoker_B has Tobacco
//smoker_C has paper

void* smoker_A(void* arg)
{

	while(true) {
            Semaphore_wait(&Tobacco_Paper);
            Semaphore_wait(&lock1);
            printf("Smoker_A : with Matches\n");
            smoke();
            Semaphore_signal(&DoneSmoking);
            Semaphore_signal(&agent);
            Semaphore_signal(&lock1);
            //sleep(1+rand()%5);
      }
}

void* smoker_B(void* arg)
{
	while(true) {

            Semaphore_wait(&Paper_Matches);
            Semaphore_wait(&lock1);
            printf("Smoker_B : with Tobacco\n");
            smoke();
            Semaphore_signal(&DoneSmoking);
            Semaphore_signal(&agent);
            Semaphore_signal(&lock1);
            //sleep(1+rand()%5);
      }
}

void* smoker_C(void* arg)
{
	while(true) {

            Semaphore_wait(&Matches_Tobacco);
            Semaphore_wait(&lock1);
            printf("Smoker_C : with Paper\n");
            smoke();
            Semaphore_signal(&DoneSmoking);
            Semaphore_signal(&agent);
            Semaphore_signal(&lock1);
            //sleep(1+rand()%5);
      }
}


int main(int argc, char* arvg[])
{
	SEM_INIT(&agent, 1);
	SEM_INIT(&DoneSmoking, 1);
	SEM_INIT(&lock1, 1);
	SEM_INIT(&Paper_Matches, 0);
	SEM_INIT(&Matches_Tobacco, 0);
	SEM_INIT(&Tobacco_Paper, 0);


	pthread_t smoker_A_thread,smoker_B_thread,smoker_C_thread,agent_thread;

	int s[3],a;
	srand(time(NULL));
	s[0] = pthread_create(&smoker_A_thread, NULL, smoker_A, NULL);
	if (s[0]) {
         exit(-1);
      }
      s[1] = pthread_create(&smoker_B_thread, NULL, smoker_B, NULL);
	if (s[1]) {
         exit(-1);
      }
      s[2] = pthread_create(&smoker_C_thread, NULL, smoker_C, NULL);
	if (s[2]) {
         exit(-1);
      }
    a = pthread_create(&agent_thread, NULL, agent_func, NULL);
	if (a) {
         exit(-1);
      }

	(void) pthread_join(smoker_A_thread, NULL);
	(void) pthread_join(smoker_B_thread, NULL);
	(void) pthread_join(smoker_B_thread, NULL);
    (void) pthread_join(agent_thread, NULL);

	return 0;
}