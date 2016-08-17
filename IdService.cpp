#include "IdService.h"

IdService::IdService()
{
}

IdService::~IdService()
{
}

int IdService::FidAlloc(const uInt32 uDbId, uInt32 &uFid)
{
    uFid = m_uCurId;
    m_IdArray[uFid] = uDbId;
    m_uCurId ++;
    return 0;
};

int IdService::GetDbId(uInt32 &uDbId, const uInt32 uFid)
{
    if(uFid > Max_ROW_PER_TABLE)
        return -1;
    uDbId = m_IdArray[uFid];
    return 0;
};


