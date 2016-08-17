#include <unistd.h>
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
#include "comm.h"
#include <algorithm>
#include "DataPickle.h"
#include "DataNeiborCache.h"

using namespace std;

int LoadRowTomem(
        dataContent::DataOperator & objDataContentOperator,
        reverseIndex::DataOperator &objReverseIndexOperator,
        const std::string strContentTableName, 
        const std::string strReverseTableName,
        const unsigned int uFid, CellMap & objResMap)
{
    for(CellMap::iterator it = objResMap.begin(); it != objResMap.end(); it++)
    {
        unsigned int uCol = it->first;
        float fData = it->second;
        int ret = objDataContentOperator.InsertDataItem(strContentTableName,uFid, uCol,fData);    
        if(ret != 0)
        {
            cout<<"objDataContentOperator InsertDataItem fail, ret"<<ret<<endl;
            cout<<"fail data: uCol " << uCol << " fData "<< fData<<endl;
            continue;
        }
        
        ret = objReverseIndexOperator.InsertDataItem(strReverseTableName, uCol,uFid, fData);
        if(ret != 0)
        {
            cout<<"objReverseIndexOperator InsertDataItem fail, ret"<<ret<<endl;
            cout<<"fail data: uCol " << uCol << " fData "<< fData<<endl;
            continue;
        }

    }
    return 0;
}


int main(int argc, char *argv[])
{
    if(argc <2)
    {
        cout<<"testcomm <DataFileName>"<<endl;;
        return -1;
    }
    
    std::string filename = argv[1];
    
    int shmid;
    
    shmid=shmget(IPC_PRIVATE, sizeof(DataPickle) +1000000, IPC_CREAT|0666);
    if(shmid==-1)
    {
        perror("shmget");
    }

    DataPickle *ptrDataPickle;
    if((long)(ptrDataPickle =(DataPickle *)shmat(shmid,NULL,0))==-1)
    {
        perror("shmat");
        exit(1);
    }
    
    cout<<"################################"<<endl;
    printf("shmid %d\n",shmid);
    cout<<"################################"<<endl;
    
    std::string strContentTableName = "ContentTable";
    std::string strReverseIndexName = "ReverseIndex";

    dataContent::DataOperator objDataContentOper;
    objDataContentOper.AddDataTable(strContentTableName, &ptrDataPickle->objContentTable);

    reverseIndex::DataOperator objReverseIndexOper;
    objReverseIndexOper.AddDataTable(strReverseIndexName, &ptrDataPickle->objReverseIndexTable);

    IdService * IdServer = &ptrDataPickle->objIdServer;
   

    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(filename.c_str(), "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    

    CellMap objCellMap;
    
    while ((read = getline(&line, &len, fp)) != -1) {
        std::string strLine = line;
        
        unsigned int uDbId,uFid;
        //cout<<"strline"<< strLine <<endl;
        
        ParseLine2(strLine, uDbId, objCellMap);
       
        IdServer->FidAlloc(uDbId, uFid);
        
        cout<<"UDbId:"<< uDbId <<" uFid:"<<uFid<<endl;
    
        for(CellMap::iterator it  = objCellMap.begin(); it != objCellMap.end(); it ++)
        {
            cout<<it->first<<":"<<it->second<<" ";
        }

        int ret = LoadRowTomem(
                objDataContentOper,
                objReverseIndexOper,
                strContentTableName,
                strReverseIndexName,
                uFid,
                objCellMap
                );
        if(ret != 0)
            {
                cout<<"load mem fail"<<endl;
            }


        objCellMap.clear();
    }
    
    cout<<"copy ok cp data"<< sizeof(DataPickle)<<endl;
    dataContent::DataRow objDataRow;
    objDataContentOper.GetDataRow(strContentTableName, 1, objDataRow);
    
    for_each(objDataRow.m_objCells, objDataRow.m_objCells+MAX_CELLS_PER_ROW, dataContent::PrintDataCell);


    free(line);
    exit(EXIT_SUCCESS);

    cout<<"hello,world"<<endl;
    
    return 0;
}
