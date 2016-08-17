#ifndef _COMM_H_ 
#define _COMM_H_ 

#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <cstring>
#include <iostream>

typedef std::map<unsigned int,float> CellMap;

unsigned int StrToUInt(const std::string strPart)
{
    std::stringstream oss;
    oss << strPart;
    unsigned int res;
    oss >> res;
    return res;
};

float StrToFloat(const std::string strPart)
{
    std::stringstream oss;
    oss << strPart;
    float res;
    oss >> res;
    return res;
}

int ParseLine(const std::string strLine, unsigned int & uDbId, CellMap & objResMap)
{
    char delims[] = ",";
    char *result = NULL;
    char buffer[10240];

    strcpy(buffer, strLine.c_str());

    result = strtok( buffer, delims );
    
    int i = 0;
    while( result != NULL ) 
    {
        std::string strData = result;
        if(i == 0)
        {
            uDbId = StrToUInt(strData);
            std::cout<<"strdata:"<<strData<<std::endl;
            i++;
            continue;
        }

        result = strtok( NULL, delims );
        
        if(i == 1) 
        {
            i++;
            continue;
        }
        float fData = StrToFloat(strData);
        if(fData != 0)
            {
                objResMap[i-2] = fData; 
            }
        i++;
    }
    std::cout<<"i"<<i<<std::endl;
    
    return 0;

};

int ParseLine2(const std::string strLine, unsigned int & uDbId, CellMap & objResMap)
{
    char delims[] = ",";
    char *result = NULL;
    //char buffer[10240];
    const char *buffer = strLine.c_str();
    
    //strcpy(buffer, strLine.c_str());
    
    char bufferOut[10240];
    int count=0;
    int begin=0;
    for(int i=0; i< strLine.size(); i++ )
    {
        if(count > 4000)
        {
            cout<<"count > 4000"<<endl;
            break;
        }
        if(buffer[i] == '\n' || buffer[i] == '\0' || buffer[i] == ',')
        {
            if(i == (begin+1) && buffer[begin] == '0')
            {
                begin = i+1;
                count++;
                continue;
            }
            if(i > begin)
            {   
                int len = i - begin;
                memcpy(bufferOut, buffer+begin, len);
                bufferOut[len]= '\0';
                begin = i+1;
                if(count == 0)
                {
                    uDbId = StrToUInt(bufferOut);
                    count ++;
                }
                else
                {
                    float fData = StrToFloat(bufferOut);
                    if(fData != 0)
                    {
                        objResMap[count-1] = fData; 
                    }
                    count ++;
                }
            }
        }
        //nothing
    }
    std::cout<<"count"<<count<<std::endl;; 
    return 0;

};



















#endif

