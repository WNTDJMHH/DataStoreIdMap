#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include "DataContent.h"
#include "ReverseIndex.h"
#include "IdService.h"

#include "DataPickle.h"

#define TEST_TYPE DataPickle
#define TEST_PTR ptrDataPicke

using namespace std;
int main(void)
{
    int shmid;
    
    TEST_TYPE * TEST_PTR;

    shmid=shmget(IPC_PRIVATE,sizeof(TEST_TYPE),IPC_CREAT|0666);
    if(shmid==-1)
    {
        perror("shmget");
    }
    if((long)(TEST_PTR=(TEST_TYPE  *)shmat(shmid,NULL,0))==-1)
    {
        perror("shmat");
        exit(1);
    }

    printf("%d\n",shmid);
    return 0;
}
