#include <iostream>
#include "IdService.h"
#include <vector>
#include <algorithm>
using namespace std;

template <class T>
void printElem(T &elem)
{
    cout << elem << " " << endl;
}
int main()
{

    uInt32 uDbId, uFid;
    IdService is;
    vector<int> v;
    for(int i=0;i< 20;i++)
    {
        uDbId = 100 -i;
        is.FidAlloc(uDbId, uFid);
        v.push_back(uFid);
    }
    is.GetDbId(uDbId, 40);
    cout<<"get dbid"<<uDbId<<endl; 
    for_each(v.begin(), v.end(), printElem<int>);
//    is.LoadDbIdToFidMapMem();
 //   is.GetFid(89, uFid);
    cout<<"get fid"<<uFid<<endl; 
    return 0;
}
