// main.cpp
#pragma warning(disable:4996)
#pragma warning(disable:4819)
#include "tinystr.h"
#include "tinyxml.h"
#include "interface.h"
#include"mark.h"
#include<iostream>
#include<vector>
#include<string>
#include<stdlib.h>
#include<iomanip>
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>
#include <WINDOWS.H> 
using namespace cv;

using namespace std;


int main(int argc, char* argv[]) {

    int ret = 0;

    char tempPath[40] = { 0 };
    char goodPath[50] = { 0 };
    char badPath[50] = { 0 };
    char backPath[50] = { 0 };
    char towerPath[50] = { 0 };
    char tempfile[10] = { 0 };
    char tempCurPath[50] = { 0 };
    char curdir[30] = { 0 };
    char completefile[50] = { 0 };
#if 0
    char* filepath = argv[1];
    char* cfgpath = argv[2];
#else
    //char* filepath = "E:\\testXml\\xml";
   char* filepath = "E:\\ysy\\testXml\\xml_test";
   char* cfgpath = "E:\\ysy\\cfg.txt";
#endif
    cout << filepath << endl;

    sprintf(tempPath, "%s%s", filepath, "\\*.xml");

    char* dotpoint = (char*)strchr(tempPath, '.');
   
    memcpy(curdir, tempPath,dotpoint - tempPath - 1);

    vector<string> curdirs;
    
    vector<string> filenames;
    ret = getfiles(tempPath, filenames);
    vector<string> values;
    cout << "cfg file is " << cfgpath << endl;
    bool res = readConfigFile(cfgpath,"name" , values);

    for (int i = 0; i < values.size(); i++)
    {
        sprintf(tempCurPath, "%s\%s", curdir, values[i]);
        createDirectory(tempCurPath);
        curdirs.push_back(tempCurPath);
    }
/*
    sprintf(goodPath, "%s\%s", curdir, "good");
    sprintf(badPath, "%s%s", curdir, "bad");
    sprintf(backPath, "%s%s", curdir, "back");
    sprintf(goodPath, "%s\%s", curdir, "tower");
    createDirectory(goodPath);
    createDirectory(badPath);
    createDirectory(backPath);
    */
    for (int i = 0; i < filenames.size(); i++)
    {
        
        mark markTemp;
        char* dotpoint = (char*) strchr(filenames[i].c_str(), '.');
        memcpy(tempfile, filenames[i].c_str(), dotpoint - filenames[i].c_str()-1);
//      sprintf(completedir, "%s%s", curdir, tempfile);
        sprintf(completefile, "%s%s", curdir, filenames[i].c_str());
//      createDirectory(completedir);
        
        setInfoToMark(markTemp, completefile);
               
      

        cutImage(markTemp, values, curdirs);
    }

    system("pause");
    return 0;
}