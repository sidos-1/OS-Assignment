//Reader-Writer-Starve-Free Solution using semaphores

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include <pthread.h>
#include "semaphore.cpp"

using namespace std;

//SEMAPHORES
Semaphore resourceAccess;
Semaphore readCountAccess;      
Semaphore serviceQueue;

int readCount=0;
int writerCount=1;

void writeResource()
{
    printf("Readers (before writing): %d\n",readCount);
    printf("Writer : %d\n",writerCount++ );
    printf("Writing to Resource. \n");
    //sleep(1+rand()%10); 
    printf("Writing Finished. \n");
}

void* writer(void* arg)
{
    while(1)
    {
    	
        Semaphore_wait(&serviceQueue);
        printf("\tSimultaneous Readers: %d\n",readCount);
        Semaphore_wait(&resourceAccess);
        
        writeResource();
        Semaphore_signal(&resourceAccess);
        Semaphore_signal(&serviceQueue);
        sleep(1 + rand()%10);
    }
}

void readResource()
{
	printf("Starting to Read. \n");
	sleep(1+rand()%10);
    printf("\tReading Finished. \n");
}

void* reader(void* arg)
{
    while(1)
    {
        Semaphore_wait(&serviceQueue);
        Semaphore_wait(&readCountAccess);
        if (++readCount == 1)        
            Semaphore_wait(&resourceAccess); 
        Semaphore_signal(&readCountAccess);            
        Semaphore_signal(&serviceQueue);
          
        readResource();
           
        Semaphore_wait(&readCountAccess);       
        if (--readCount == 0)
            Semaphore_signal(&resourceAccess);
        Semaphore_signal(&readCountAccess);

        sleep(1+rand()%6);
        
    }
}


int main()
{
    SEM_INIT(&resourceAccess,1); 
    SEM_INIT(&readCountAccess,1); 
    SEM_INIT(&serviceQueue,1);

    // int r1=rand()%20;
    // int r2=rand()%5;

    int r1=10;
    int r2=2;

    int p1[r1],c1[r2],m1[r1+r2];
    srand(time(NULL));
    pthread_t reader1[r1],writer1[r2];
    pthread_t threads[r1+r2];

    int k=rand()%2;
    int r_1=0,r_2=0;
    for(int i=0;i<r1+r2;i++)
    {
        if(k==0&&r_1<r1)
        {
            m1[i]=pthread_create(&threads[i],NULL,reader,NULL);
            r_1++;
        }
        else if(k==1&&r_2<r2)
        {
            m1[i]=pthread_create(&threads[i],NULL,writer,NULL);
            r_2++;
        }
        else{
            if(r_1<r1)
            {
                m1[i]=pthread_create(&threads[i],NULL,reader,NULL);
            }
            else
            {
                m1[i]=pthread_create(&threads[i],NULL,writer,NULL);
            }
        }
        k=rand()%2;
    }

    // for(int i=0;i<r1;i++)
    // {
    //     p1[i] = pthread_create(&reader1[i], NULL, reader, NULL);
    //     if (p1[i]) {
    //          exit(-1);
    //       }
    // }

    // for(int i=0;i<r2;i++)
    // {
    //     c1[i] = pthread_create(&writer1[i], NULL, writer, NULL);
    //     if (c1[i]) {
    //          exit(-1);
    //       }
    // }
    // p1= pthread_create(&reader1, NULL, reader, NULL);
    //    if (p1) {
    //           exit(-1);
    //     }
    // c1 = pthread_create(&writer1, NULL, writer, NULL);
    // if (c1) {
    //      exit(-1);
    //   }


    for(int i=0;i<r1+r2;i++)
    {
        (void) pthread_join(threads[i], NULL);
    }

    // for(int i=0;i<r1;i++)
    // {
    //     (void) pthread_join(reader1[i], NULL);
    // }

    // for(int i=0;i<r2;i++)
    // {
    //     (void) pthread_join(writer1[i], NULL);
    // }
    //(void) pthread_join(reader1, NULL);
    


    return 0;
}