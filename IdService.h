#ifndef _IDSERVICE_H_ 
#define _IDSERVICE_H_ 


#include <iostream>
#include <fstream>
#include <map>
#include <cstdio>

#define Max_ROW_PER_TABLE 1500000 

using namespace std;

#pragma pack(4)

typedef unsigned int uInt32;
typedef map<uInt32,uInt32> DbIdToFidMap;

class IdService
{
    public:
        IdService();
        ~IdService();
        int FidAlloc(const uInt32 uDbId, uInt32 &uFid);
        int GetDbId(uInt32 &uDbId, const uInt32 uFid);
        
        uInt32 m_uCurId = 0;
        int m_IdArray[Max_ROW_PER_TABLE];

};

#endif
