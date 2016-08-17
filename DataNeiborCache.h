#ifndef _DATANEIBORCACHE_H_ 
#define _DATANEIBORCACHE_H_ 


#include <iostream>
#include <map>

#define MAX_NEIBOR_CELLS_PER_ROW 100 
#define Max_ROW_PER_TABLE 1500000 

using namespace std;

#pragma pack(4)
namespace dataNeiborCache 
{
    class DataCell 
    {
        public:
            DataCell(){};
            DataCell(int uCol, float fDataItem):uCol(uCol),fDataItem(fDataItem) {};
            unsigned int uCol = 0;
            float fDataItem = 0;
            bool operator < (const DataCell &objDataCell);
            bool operator > (const DataCell &objDataCell);
            bool operator == (const DataCell &objDataCell);
    } ;

    class DataRow
    {
        public:
            unsigned int uFileId;
            unsigned int uCuruentIndex = 0;
            DataCell m_objCells[MAX_NEIBOR_CELLS_PER_ROW];
    } ;

    typedef struct
    {
        DataRow m_objRows[Max_ROW_PER_TABLE];
    } DataTable;

    class DataOperator
    {
        public:
            DataOperator();
            ~DataOperator();	

            int CreateTable(const std::string strTableName);	
            int InsertDataItem(const std::string strTableName, const unsigned int uRow, const unsigned int uCol,const float fDataItem);
            int GetDataItem(const std::string strTableName, const unsigned int uRow, const unsigned int uCol, float & fDataItem);
            int GetDataRow(const std::string strTableName, const unsigned int uRow, DataRow & objDataRow);
            
            int AddDataTable(const std::string strTableName, DataTable * ptrDataTable); 

            typedef std::map<std::string , DataTable *> StoreDataMap;

            StoreDataMap m_mapDataMap;

    };
    bool CompareDataCellReverse(const DataCell objCell1, const DataCell objCell2);

    bool CompareDataCell(const DataCell objCell1, const DataCell objCell2);

    void PrintDataCell(const DataCell &objCell);
}
#endif
