#include "DataContent.h"
#include "string.h"
#include <algorithm>
#include <cmath>

namespace dataContent
{
   
    float CalculateSimilarBetweenTowRow(const DataRow & objTargetRow,const DataRow & objRow, float *fSortBuffer)
    {
        //zero objTargetRowCell.uCol position
        for(int i = 0; i< MAX_CELLS_PER_ROW; i++)
        {
            const DataCell & objTargetRowCell = objTargetRow.m_objCells[i];
            if(objTargetRowCell.uCol != 0 && objTargetRowCell.fDataItem != 0)
            {
                fSortBuffer[objTargetRowCell.uCol] = 0;
            }
        }
        
        //write [objRowCell.uCol position
        for(int i = 0; i< MAX_CELLS_PER_ROW; i++)
        {
            const DataCell & objRowCell = objRow.m_objCells[i];
            if(objRowCell.uCol != 0 && objRowCell.fDataItem != 0)
            {
                fSortBuffer[objRowCell.uCol] = objRowCell.fDataItem;
            }
        }
       
        float fSimilar = 0.0;
        for(int i = 0; i< MAX_CELLS_PER_ROW; i++)
        {
            const DataCell & objTargetRowCell = objTargetRow.m_objCells[i];
            if(objTargetRowCell.uCol != 0 && objTargetRowCell.fDataItem != 0)
            {
               fSimilar += (fSortBuffer[objTargetRowCell.uCol] * objTargetRowCell.fDataItem);
            }
        }
        fSimilar = sqrt(fSimilar);
        return fSimilar; 
    };
     
    float CalculateSimilarBetweenTowRow(const DataRow & objTargetRow,const DataRow & objRow)
    {
        //zero objTargetRowCell.uCol position
        typedef map<unsigned int , unsigned int> SimilarMap;
        SimilarMap mapSimilar;

        for(int i = 0; i< MAX_CELLS_PER_ROW; i++)
        {
            const DataCell & objTargetRowCell = objTargetRow.m_objCells[i];
            if(objTargetRowCell.uCol != 0 && objTargetRowCell.fDataItem != 0)
            {
                mapSimilar[objTargetRowCell.uCol] = objTargetRowCell.fDataItem; 
            }
        }

        float fSimilar = 0.0;
        //write [objRowCell.uCol position
        for(int i = 0; i< MAX_CELLS_PER_ROW; i++)
        {
            const DataCell & objRowCell = objRow.m_objCells[i];
            if(objRowCell.uCol != 0 && objRowCell.fDataItem != 0)
            {
                if(mapSimilar.find(objRowCell.uCol) != mapSimilar.end())
                {
                    float fTargetDataItem = mapSimilar[objRowCell.uCol];
                    fSimilar = fSimilar + objRowCell.fDataItem * fTargetDataItem;
                }
            }
        }
       
        fSimilar = sqrt(fSimilar);
        return fSimilar; 
    };

    //########################################
    bool CompareDataCell(const DataCell objCell1, const DataCell objCell2)
    {
        return objCell1.fDataItem < objCell2.fDataItem;
    }

    bool CompareDataCellReverse(const DataCell objCell1, const DataCell objCell2)
    {
        return objCell1.fDataItem > objCell2.fDataItem;

    }

    void PrintDataCell(const DataCell &objCell)
    {
        cout<<objCell.uCol<<":"<<objCell.fDataItem<<" ";
    };

    //##############################################################
    bool DataCell::operator < (const DataCell &objDataCell)
    {
        return this->fDataItem < objDataCell.fDataItem;

    };

    bool DataCell::operator > (const DataCell &objDataCell)
    {
        return this->fDataItem > objDataCell.fDataItem;
    };

    bool DataCell::operator == (const DataCell &objDataCell)
    {
        return this->fDataItem == objDataCell.fDataItem;
    }

    //##########################################################
    DataOperator::DataOperator()
    {
    };

    DataOperator::~DataOperator()
    {
    };	
    int DataOperator::CreateTable(const std::string strTableName)
    {
        DataTable *objTable =  new DataTable();
        if(objTable == NULL )
        {
            cout<<"mem alloc fail"<<endl;
            return -1;
        }
        this->m_mapDataMap[strTableName] = objTable;

        return 0;
    }

    int DataOperator::InsertDataItem(const std::string strTableName, const unsigned int uRow,const unsigned int uCol, const float fDataItem)
    {	
        StoreDataMap::iterator it = m_mapDataMap.find(strTableName);	
        if(it == m_mapDataMap.end())
        {
            cout<<"map not found"<<endl;
            return -1;
        }

        DataTable *objTable = m_mapDataMap[strTableName];
        if(uRow >= Max_ROW_PER_TABLE )
        {
            cout<<"row or cells full, uRow:" <<uRow<<endl;
            return -1;
        }

        DataRow & objDataRow = objTable->m_objRows[uRow];
        DataCell objDataCell (uCol, fDataItem);
        if(objDataRow.uCuruentIndex < MAX_CELLS_PER_ROW - 1)
        {
            objDataRow.m_objCells[objDataRow.uCuruentIndex]= objDataCell;
            objDataRow.uCuruentIndex ++;
            return 0;
        }
        if(objDataRow.uCuruentIndex == MAX_CELLS_PER_ROW - 1)
        {
            objDataRow.m_objCells[objDataRow.uCuruentIndex]= objDataCell;
            objDataRow.uCuruentIndex ++;
            if (!std::is_heap(objDataRow.m_objCells, objDataRow.m_objCells+MAX_CELLS_PER_ROW, CompareDataCellReverse) )
                std::make_heap(objDataRow.m_objCells, objDataRow.m_objCells+MAX_CELLS_PER_ROW, CompareDataCellReverse);

            return 0; 
        }

        DataCell objMinDataCell = objDataRow.m_objCells[0];

        if(objDataCell < objMinDataCell)
        {
            cout<<"objDataCell:"<<uCol<<"|"<<fDataItem<<" is not top "<< MAX_CELLS_PER_ROW<<endl;
            cout<<"currmin:"<<objMinDataCell.uCol<<"|"<<objMinDataCell.fDataItem<<endl;
            return 0;
        }

        objDataRow.m_objCells[0] = objDataCell;

        if (!std::is_heap(objDataRow.m_objCells, objDataRow.m_objCells+MAX_CELLS_PER_ROW, CompareDataCellReverse) )
            std::make_heap(objDataRow.m_objCells, objDataRow.m_objCells+MAX_CELLS_PER_ROW, CompareDataCellReverse);

        return 0;
    }
    int DataOperator::GetDataItem(const std::string strTableName,const unsigned int uRow,const unsigned int uCol, float & fDataItem)
    {
        if(uRow >= Max_ROW_PER_TABLE)
        {
            cout<<"uRow >= "<< Max_ROW_PER_TABLE<<" fail"<<endl;
            return -1;
        }

        DataTable *objTable = m_mapDataMap[strTableName];
        DataRow & objDataRow = objTable->m_objRows[uRow];

        for(int i = 0; i < MAX_CELLS_PER_ROW; i++)
        {
            DataCell &objDataCell = objDataRow.m_objCells[i];
            if(objDataCell.uCol ==  uCol)
            {
                fDataItem = objDataCell.fDataItem;
                return 0;
            }
        }

        return -2;
    }

    int DataOperator::GetDataRow(const std::string strTableName, unsigned int uRow, DataRow & objDataRow)
    {
        if(uRow >= Max_ROW_PER_TABLE)
        {
            return -1;
        }

        if(m_mapDataMap.find(strTableName) == m_mapDataMap.end())
        {
            cout<<"table name not found"<<endl;
            return -1;
        }
        DataTable *objTable = m_mapDataMap[strTableName];
        objDataRow = objTable->m_objRows[uRow];

        return 0;
    }

    int DataOperator::AddDataTable(const std::string strTableName, DataTable * ptrDataTable)
    {
        m_mapDataMap[strTableName] = ptrDataTable;
        return 0;
    }


}
