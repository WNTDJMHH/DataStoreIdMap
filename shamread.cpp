#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include "DataContent.h"
#include "ReverseIndex.h"
#include "IdService.h"
#include "DataPickle.h"


#define TEST_TYPE DataPickle
#define TEST_PTR ptrDataPickle
int main(int argc,char **argv)
{
    int shmid;
    shmid=atoi(argv[1]);
   

    TEST_TYPE * TEST_PTR;

    if((long)(TEST_PTR=(TEST_TYPE *)shmat(shmid,NULL,0))==-1)
    {
        perror("shmat");
        exit(1);
    }

    cout<<TEST_PTR->objIdServer.m_IdArray[0]<<endl;
    cout<<"okkkk"<<endl;
    shmdt(TEST_PTR);
    return 0;
}
