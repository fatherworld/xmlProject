#pragma once
#pragma warning(disable:4996)
#include "tinystr.h"
#include "tinyxml.h"
#include"mark.h"
#include<iostream>
#include<time.h>
#include<vector>
#include <io.h>  
#include <direct.h>  
#include<string>
#include<stdlib.h>
#include<iomanip>
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <Windows.h>
#include <fstream> 
#include<vector>
#define PATH_DELIMITER '\\'  
static int var = 0;
using namespace cv;
using namespace std;



int getfiles(string szPath, vector<string>& filenames);

int cutImage(mark& myMark, vector<string> values, vector<string> curdirs);

int setInfoToMark(mark & myMark,char* xmlpath);

bool createDirectory(const std::string folder);

bool readConfigFile(char * cfgfilepath, const string & key, vector<string> & value);