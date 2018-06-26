#include"Interface.h"

/*
*parameter: cfgfilepath 文件的绝对路径名如 : / user / home / my.cfg
*           key         文本中的变量名
*           value       对应变量的值，用于保存
*
* /
*/
bool readConfigFile(char * cfgfilepath, const string & key, vector<string> & value)
{
    cout << "cfgfilepath" << cfgfilepath << endl;
    fstream cfgFile;
    cfgFile.open(cfgfilepath);//打开文件      
    if (!cfgFile.is_open())
    {
        cout << "can not open cfg file!" << endl;
        return false;
    }
    char tmp[100];
    while (!cfgFile.eof())//循环读取每一行  
    {
        cfgFile.getline(tmp, 100);//每行读取前100个字符
        string line(tmp);
        size_t pos = line.find('=');//找到每行的“=”号位置，之前是key之后是value  
        if (pos == string::npos) return false;
        string tmpKey = line.substr(0, pos);//取=号之前  
        if (key == tmpKey)
        {
            value.push_back(line.substr(pos + 1));//取=号之后  
        }
    }
    return false;
}

int setInfoToMark(mark & myMark, char* xmlpath)
{
    TiXmlDocument doc(xmlpath);    // 读入XML文件
    if (!doc.LoadFile()) return -1;  // 如果无法读取文件，则返回
    TiXmlHandle hDoc(&doc);         // hDoc是&doc指向的对象
    TiXmlElement* pElem;            // 指向元素的指针
    pElem = hDoc.FirstChildElement().Element(); //指向根节点
    TiXmlHandle hRoot(pElem);       // hRoot是根节点
    TiXmlElement* pathElem = hRoot.FirstChild("path").Element(); //当前指向了path节点
    const char* path = pathElem->GetText();
    int length = strlen(path);
    myMark.setPath(path, length);
    TiXmlElement* widthElem = hRoot.FirstChild("size").FirstChild("width").Element(); //当前指向了width节点
    myMark.width = atoi(widthElem->GetText());
    TiXmlElement* heightElem = hRoot.FirstChild("size").FirstChild("height").Element(); //当前指向了height节点
    myMark.height = atoi(heightElem->GetText());


    TiXmlElement* objElem = hRoot.FirstChild("object").Element();
    for (objElem; objElem; objElem = objElem->NextSiblingElement()) { // 挨个读取node节点的信息
        obj objTemp;
        TiXmlHandle node(objElem);  // objElem所指向的节点
        TiXmlElement* nameElem = node.FirstChild("name").Element();  // name节点
        memcpy(objTemp.label, nameElem->GetText(), strlen(nameElem->GetText()));

        TiXmlElement* xmin = node.FirstChild("bndbox").FirstChild("xmin").Element();  // xmin节点
        objTemp.xmin = atof(xmin->GetText());
        TiXmlElement* ymin = node.FirstChild("bndbox").FirstChild("ymin").Element();  // ymin节点
        objTemp.ymin = atof(ymin->GetText());
        TiXmlElement* xmax = node.FirstChild("bndbox").FirstChild("xmax").Element();  // xmax节点
        objTemp.xmax = atof(xmax->GetText());
        TiXmlElement* ymax = node.FirstChild("bndbox").FirstChild("ymax").Element();  // ymax节点
        objTemp.ymax = atof(ymax->GetText());
        myMark.objs.push_back(objTemp);
    }
    return 0;
}

static int setLocalTime(char* localTime,const char* savepath)
{
    int ret = 0;
    time_t timep;
    struct tm *p;
    time(&timep);
    p = localtime(&timep); //此函数获得的tm结构体的时间，是已经进行过时区转化为本地时间  
    var++;                       //p = gmtime(&timep); //把日期和时间转换为格林威治(GMT)时间的函数  
    sprintf(localTime, "%s\\%d@%d_%d_%d_%d_%d_%d.jpg",savepath,var, (1900 + p->tm_year), (p->tm_mon+1), (p->tm_mday), (p->tm_hour), (p->tm_min), (p->tm_sec));

    return ret;
}


#if 0
int cutImage(mark& myMark,const char* goodPath,const char* badPath,const char* backPath,const char* towerPath)
{
    int ret = 0;
    Mat src = imread(myMark.path);

    if (!src.data)
    {
        ret = -1;
        return ret;
    }
    char* localTime = (char*)malloc(sizeof(char)*100);
    for (int i = 0; i < myMark.objs.size(); i++)
    {
        memset(localTime, 0, sizeof(char) * 100);
        Mat roi_img;
        //Rect rectTemp(myMark.objs[i].xmin, myMark.objs[i].ymin, myMark.objs[i].xmax, myMark.objs[i].ymax);

        Range R1, R2;
        R1.start = myMark.objs[i].xmin;
        R1.end = myMark.objs[i].xmax + 1;
        R2.start = myMark.objs[i].ymin;
        R2.end = myMark.objs[i].ymax + 1;
        //roi_img = src(Range(myMark.objs[i].xmin, myMark.objs[i].xmax+1),Range(myMark.objs[i].ymin, myMark.objs[i].ymax+1));
        roi_img = Mat::Mat(src,R2, R1);

        if (!strncmp(myMark.objs[i].label, "good", 4))
        {
            ret = setLocalTime(localTime, goodPath);
        }
        else if (!strncmp(myMark.objs[i].label, "bad", 3))
        {
            ret = setLocalTime(localTime, badPath);
        }
        else if(!strncmp(myMark.objs[i].label, "back", 4))
        {
            ret = setLocalTime(localTime, backPath);
        }
//       ret = setLocalTime(localTime,savepath); 

        imwrite(localTime, roi_img);
    }
    if (localTime)
    {
        free(localTime);
        localTime = NULL;
    }

    return ret;
}
#else 1
int cutImage(mark& myMark,vector<string> values,vector<string> curdirs)
{
    int ret = 0;
    Mat src = imread(myMark.path);

    if (!src.data)
    {
        ret = -1;
        return ret;
    }
    char* localTime = (char*)malloc(sizeof(char) * 100);
    for (int i = 0; i < myMark.objs.size(); i++)
    {
        memset(localTime, 0, sizeof(char) * 100);
        Mat roi_img;
        //Rect rectTemp(myMark.objs[i].xmin, myMark.objs[i].ymin, myMark.objs[i].xmax, myMark.objs[i].ymax);

        Range R1, R2;
        R1.start = myMark.objs[i].xmin;
        R1.end = myMark.objs[i].xmax + 1;
        R2.start = myMark.objs[i].ymin;
        R2.end = myMark.objs[i].ymax + 1;
        //roi_img = src(Range(myMark.objs[i].xmin, myMark.objs[i].xmax+1),Range(myMark.objs[i].ymin, myMark.objs[i].ymax+1));
        roi_img = Mat::Mat(src, R2, R1);
        cout << "src->width is " << src.cols << endl;
        cout << "src->height is " << src.rows << endl;
        cout << "roi_img->xmin" << myMark.objs[i].xmin << endl;
        cout << "roi_img->xmax" << myMark.objs[i].xmax << endl;
        cout << "roi_img->ymin" << myMark.objs[i].ymin << endl;
        cout << "roi_img->ymax" << myMark.objs[i].ymax << endl;
        cout << "该图片的路径为" << myMark.path << endl;
        if (R1.start < 0 || R1.end > src.cols || R2.start < 0 || R2.end >src.cols)
        {
            cout << "该图片" << myMark.path << "剪裁有错误" << endl;
            ret = -2;
            return ret;
        }

        for (int j = 0; j < values.size(); j++)
        {
           
            if (!strncmp(myMark.objs[i].label, values[j].c_str(), values[j].length()))
            {
                cout << "values[i].leng" << values[j].length() << endl;
                cout << values[j].c_str() << "fadsfas" << endl;
                cout << myMark.objs[i].label << "------" << endl;
                ret = setLocalTime(localTime, curdirs[j].c_str());
                break;
            }
                    
        }
        /*
        if (!strncmp(myMark.objs[i].label, "good", 4))
        {
            ret = setLocalTime(localTime, goodPath);
        }
        else if (!strncmp(myMark.objs[i].label, "bad", 3))
        {
            ret = setLocalTime(localTime, badPath);
        }
        else if (!strncmp(myMark.objs[i].label, "back", 4))
        {
            ret = setLocalTime(localTime, backPath);
        }
        else if (!strncmp(myMark.objs[i].label, "tower", 4))
        {
            ret = setLocalTime(localTime, towerPath);
        }
        //       ret = setLocalTime(localTime,savepath); 
        */
//        imshow("img is", roi_img);
//        waitKey(21);
        imwrite(localTime, roi_img);
    }
    if (localTime)
    {
        free(localTime);
        localTime = NULL;
    }

    return ret;
}
#endif

int getfiles(string szPath, vector<string>& filenames)
{
    WIN32_FIND_DATA wfd;

    HANDLE hFind;//HANDLE句柄  
    hFind = FindFirstFile(szPath.c_str(), &wfd);

    //以下表示FindFirstFile调用失败  
    if (hFind == INVALID_HANDLE_VALUE)
    {
        cout << "This Handle is not rightly used";
        return 0;
    }
    int i = 0;
    do
    {
        filenames.push_back(wfd.cFileName);
    } while (FindNextFile(hFind, &wfd));
    FindClose(hFind);
}



bool createDirectory(const std::string folder) {
    std::string folder_builder;
    std::string sub;
    sub.reserve(folder.size());
    for (auto it = folder.begin(); it != folder.end(); ++it)
    {
        //cout << *(folder.end()-1) << endl;  
        const char c = *it;
        sub.push_back(c);
        if (c == PATH_DELIMITER || it == folder.end() - 1)
        {
            folder_builder.append(sub);
            if (0 != ::_access(folder_builder.c_str(), 0))
            {
                // this folder not exist  
                if (0 != ::_mkdir(folder_builder.c_str()))
                {
                    // create failed  
                    return false;
                }
            }
            sub.clear();
        }
    }
    return true;
}
