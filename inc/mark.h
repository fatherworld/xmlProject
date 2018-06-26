
#pragma once
#pragma warning(disable:4244)
#pragma warning(disable:4267)
#include<iostream>
#include<iomanip>
#include<vector>

using namespace std;
typedef struct _obj
{
    char label[20] = { 0 };
    float xmin;
    float xmax;
    float ymin;
    float ymax;
    
}obj;
class mark {
public:
    mark();
    void setPath(const char* path,int length);
    void freeMem();
    int width;
    int height;
    vector<obj> objs;
    const char* path;
};
