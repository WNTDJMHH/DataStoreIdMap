#include <iostream>
#include <map>
#include "comm.h"
#include <string>
#include <cstdio>
#include <cstdlib>
using namespace std;
int main(int argc, char *argv[])
{
    if(argc <2)
    {
        cout<<"testcomm <filename>"<<endl;;
        return -1;
    }
    std::string tempf= "1.1233\n";
    std::string tempu= "1233";
    cout<<StrToUInt(tempu)<<endl;
    cout<<StrToFloat(tempf)<<endl;

    std::string filename =argv[1];

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
        unsigned int uDbId;
        cout<<"strline"<< strLine <<endl;
        //ParseLine(strLine, uDbId, objCellMap);
        ParseLine2(strLine, uDbId, objCellMap);
        cout<<"UDbId:"<< uDbId <<endl;
    
        for(CellMap::iterator it  = objCellMap.begin(); it != objCellMap.end(); it ++)
        {
            cout<<it->first<<":"<<it->second<<" ";
        }
        objCellMap.clear();
    }


    free(line);
    exit(EXIT_SUCCESS);

    cout<<"hello,world"<<endl;
    return 0;
}
