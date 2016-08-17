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

#define MIN_NEIBOR_FREQ 5
using namespace std;

int main(int argc,char *argv[])
{ 
    if(argc <2)
    {
        cout<<"LoadFileAndContentFromShm <shmid>"<<endl;;
        return -1;
    }

    int shmid;
    shmid=atoi(argv[1]);
    cout<<"shmid"<<shmid<<endl;
 
    DataPickle *ptrDataPickle;
    if((long)(ptrDataPickle = (DataPickle *)shmat(shmid,NULL,0))==-1)
    {
        perror("shmat");
        exit(1);
    }
    cout<<"Load from sham ok size"<< sizeof(DataPickle)<<endl;

    dataContent::DataTable  & objDataTable = ptrDataPickle->objContentTable;
    
    cout<<objDataTable.m_objRows[0].uCuruentIndex<<endl;

    std::string strContentTableName = "ContentTable";
    std::string strReverseIndexName = "ReverseIndex";
    std::string strNeiborRawTableName = "neiborRaw";
    
    dataContent::DataOperator objDataContentOper;
    objDataContentOper.AddDataTable(strContentTableName, &ptrDataPickle->objContentTable);

    reverseIndex::DataOperator objReverseIndexOper;
    objReverseIndexOper.AddDataTable(strReverseIndexName, &ptrDataPickle->objReverseIndexTable);

    IdService * IdServer = &ptrDataPickle->objIdServer;

  //  dataNeiborCache::DataTable *ptrDataTableNeiborRaw = new dataNeiborCache::DataTable();
    dataNeiborCache::DataOperator objNeiborOper;
   // objNeiborOper.AddDataTable(strNeiborRawTableName, ptrDataTableNeiborRaw);
    objNeiborOper.AddDataTable(strNeiborRawTableName,  &ptrDataPickle->objDataTableNeiborRaw);
    // getDBidToFid map

    //
    //###################################################
    cout<<"we begin to enter the main algorithm"<<endl;

    typedef unsigned int uint32;
    typedef map<uint32, uint32> NeiborMap;
   
    float *fSortBuffer = new float[MAX_CELLS_PER_ROW];


    for(int iCurFid = 0; iCurFid< Max_ROW_PER_TABLE; iCurFid ++)// 140 0000
    {
        cout<<"cal clulate iCurFid"<<endl;
        dataContent::DataRow objDataRow;
        objDataContentOper.GetDataRow(strContentTableName, iCurFid, objDataRow);
        
        NeiborMap mapNeibor;
        
        for(int iCurCell = 0; iCurCell < MAX_CELLS_PER_ROW; iCurCell++) // 100
        {   
            dataContent::DataCell &objTargetDataCell = objDataRow.m_objCells[iCurCell];  
            if(objTargetDataCell.uCol == 0 || objTargetDataCell.fDataItem == 0)
            {
                continue;
            }
            
            reverseIndex::DataRow objRIndexRow;
            objReverseIndexOper.GetDataRow(strReverseIndexName, objTargetDataCell.uCol, objRIndexRow);
            
            for(int iNeiborCell = 0; iNeiborCell< MAX_CELLS_PER_INDEX_ROW; iNeiborCell++)//4000
            {
                reverseIndex::DataCell & objNeiborCell= objRIndexRow.m_objCells[iNeiborCell];
                uint32 uNeiborFid = objNeiborCell.uCol;
                float fTfidf = objNeiborCell.fDataItem;

                if(mapNeibor.find(uNeiborFid) != mapNeibor.end())
                {
                    mapNeibor[uNeiborFid] = mapNeibor[uNeiborFid] + 1;
                }
                else
                {
                    mapNeibor[uNeiborFid] = 1;
                }
 
            }
        }
        cout <<"find neibor ok, neibor size :"<< mapNeibor.size()<<endl;
        
        int count = 0;
        for(NeiborMap::iterator it = mapNeibor.begin(); it != mapNeibor.end(); it ++)
        {
            uint32 uNeiborId = it->first;
            float fFreq = it->second;
            if(fFreq < MIN_NEIBOR_FREQ)
            {
                continue;
            };
            count ++;
            int ret = objNeiborOper.InsertDataItem(strNeiborRawTableName, iCurFid, uNeiborId, fFreq); 
            if(ret != 0)
            {
                cout<<"objNeiborOper.InsertDataItem fail"<<endl;
            }
        }
        cout<<"objNeiborOper.InsertDataItem count"<<count<<endl;
        cout<< "we begin to insert NeiborRawTable"<<endl;
       //...................calculate similar........................
        cout<<"we begin to cal sim"<<endl;
        dataNeiborCache::DataRow objNeiborDataRow ;  
        int ret = objNeiborOper.GetDataRow(strNeiborRawTableName, iCurFid, objNeiborDataRow);
        if(ret != 0)
        {
            cout<<"get row fail"<<endl;
            continue;
        }
        
        for(int iCurNeibor = 0; iCurNeibor< MAX_NEIBOR_CELLS_PER_ROW; iCurNeibor++)
        {
            dataNeiborCache::DataCell &objCurNeiborCell = objNeiborDataRow.m_objCells[iCurNeibor];
            uint32 uNeiborId = objCurNeiborCell.uCol;
            float fFreq = objCurNeiborCell.fDataItem;
            dataContent::DataRow objNeiborContentDataRow;
            objDataContentOper.GetDataRow(strContentTableName, uNeiborId, objNeiborContentDataRow); 
            //float fSimilar = dataContent::CalculateSimilarBetweenTowRow(objNeiborContentDataRow, objDataRow, fSortBuffer);
            float fSimilar = dataContent::CalculateSimilarBetweenTowRow(objNeiborContentDataRow, objDataRow);
            objCurNeiborCell.fDataItem = fSimilar; 
        }
        cout<<"iCurFid"<< iCurFid<<" calculate ok"<<endl;
    }
    //delete [] fSortBuffer;

    //###################################################
    dataContent::DataRow objDataRow;
    objDataContentOper.GetDataRow(strContentTableName, 1, objDataRow);
    
    for_each(objDataRow.m_objCells, objDataRow.m_objCells+MAX_CELLS_PER_ROW, dataContent::PrintDataCell);
    
    cout<<"hello,world"<<endl;
    

    return 0;
}
