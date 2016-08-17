#include <iostream>
#include <map>
#include <string>
#include "DataContent.h"
#include <algorithm>
#include <vector>
#include <unistd.h>
using namespace std;
using namespace dataContent;
int main()
{
	DataOperator dc;
    dc.CreateTable("temptable");
    int ret = dc.InsertDataItem("temptable",1,2,1.23);
    ret = dc.InsertDataItem("temptable",1,5,2.1);
    ret = dc.InsertDataItem("temptable",1,6,2.45);
    ret = dc.InsertDataItem("temptable",2,7,1.76);
    ret = dc.InsertDataItem("temptable",2,1,5);
    ret = dc.InsertDataItem("temptable",2,9,16);
    ret = dc.InsertDataItem("temptable",1,9,12);
    //ret = dc.InsertDataItem("temptable",1,1000,2000);
    cout<<ret<<endl;
    float data ;
    ret = dc.GetDataItem("temptable",1,2,data); 
    cout<<ret<<endl;
    cout<<"data:"<<data<<endl;
    ret = dc.GetDataItem("temptable",1,1,data); 
    cout<<ret<<endl;
    cout<<"data:"<<data<<endl;
    DataRow objDataRow;
    ret = dc.GetDataRow("temptable",1,objDataRow);
    cout<<ret<<endl;

    //sort(objDataRow.m_objCells, objDataRow.m_objCells+MAX_CELLS_PER_ROW, CompareDataCell);
    for_each(objDataRow.m_objCells, objDataRow.m_objCells+MAX_CELLS_PER_ROW, PrintDataCell);
    /*
    cout<<endl<<endl; 
    DataCell dcell(100,1000);
    objDataRow.m_objCells[MAX_CELLS_PER_ROW-1] = dcell;
    cout<<endl;
    for_each(objDataRow.m_objCells, objDataRow.m_objCells+MAX_CELLS_PER_ROW, PrintDataCell);
    if (!std::is_heap(objDataRow.m_objCells, objDataRow.m_objCells+MAX_CELLS_PER_ROW, CompareDataCellReverse) )
        std::make_heap(objDataRow.m_objCells, objDataRow.m_objCells+MAX_CELLS_PER_ROW, CompareDataCellReverse);
    cout<<endl;
    for_each(objDataRow.m_objCells, objDataRow.m_objCells+MAX_CELLS_PER_ROW, PrintDataCell);
    */
    cout<<endl;
    DataRow objDataRow2;
    ret = dc.GetDataRow("temptable",2,objDataRow2);
    for_each(objDataRow2.m_objCells, objDataRow2.m_objCells+MAX_CELLS_PER_ROW, PrintDataCell);
    cout<<endl;
    float *fSortBuffer = new float[1400000];
    float fSimilar = CalculateSimilarBetweenTowRow(objDataRow, objDataRow2, fSortBuffer);
    float fSimilar2 = CalculateSimilarBetweenTowRow(objDataRow, objDataRow2);

    cout<<fSimilar<<endl;
    cout<<fSimilar2<<endl;
    delete [] fSortBuffer;

    cout<<"hello,world"<<endl;
	return 0;
}
