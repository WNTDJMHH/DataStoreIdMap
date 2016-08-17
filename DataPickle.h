#include "DataContent.h"
#include "ReverseIndex.h"
#include "IdService.h"
#include "DataNeiborCache.h"

class DataPickle
{
    public:
        dataContent::DataTable objContentTable;
        reverseIndex::DataTable objReverseIndexTable;
        IdService objIdServer;
        dataNeiborCache::DataTable objDataTableNeiborRaw;
};

