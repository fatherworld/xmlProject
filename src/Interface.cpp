#include"Interface.h"

/*
*parameter: cfgfilepath �ļ��ľ���·������ : / user / home / my.cfg
*           key         �ı��еı�����
*           value       ��Ӧ������ֵ�����ڱ���
*
* /
*/
bool readConfigFile(char * cfgfilepath, const string & key, vector<string> & value)
{
    cout << "cfgfilepath" << cfgfilepath << endl;
    fstream cfgFile;
    cfgFile.open(cfgfilepath);//���ļ�      
    if (!cfgFile.is_open())
    {
        cout << "can not open cfg file!" << endl;
        return false;
    }
    char tmp[100];
    while (!cfgFile.eof())//ѭ����ȡÿһ��  
    {
        cfgFile.getline(tmp, 100);//ÿ�ж�ȡǰ100���ַ�
        string line(tmp);
        size_t pos = line.find('=');//�ҵ�ÿ�еġ�=����λ�ã�֮ǰ��key֮����value  
        if (pos == string::npos) return false;
        string tmpKey = line.substr(0, pos);//ȡ=��֮ǰ  
        if (key == tmpKey)
        {
            value.push_back(line.substr(pos + 1));//ȡ=��֮��  
        }
    }
    return false;
}

int setInfoToMark(mark & myMark, char* xmlpath)
{
    TiXmlDocument doc(xmlpath);    // ����XML�ļ�
    if (!doc.LoadFile()) return -1;  // ����޷���ȡ�ļ����򷵻�
    TiXmlHandle hDoc(&doc);         // hDoc��&docָ��Ķ���
    TiXmlElement* pElem;            // ָ��Ԫ�ص�ָ��
    pElem = hDoc.FirstChildElement().Element(); //ָ����ڵ�
    TiXmlHandle hRoot(pElem);       // hRoot�Ǹ��ڵ�
    TiXmlElement* pathElem = hRoot.FirstChild("path").Element(); //��ǰָ����path�ڵ�
    const char* path = pathElem->GetText();
    int length = strlen(path);
    myMark.setPath(path, length);
    TiXmlElement* widthElem = hRoot.FirstChild("size").FirstChild("width").Element(); //��ǰָ����width�ڵ�
    myMark.width = atoi(widthElem->GetText());
    TiXmlElement* heightElem = hRoot.FirstChild("size").FirstChild("height").Element(); //��ǰָ����height�ڵ�
    myMark.height = atoi(heightElem->GetText());


    TiXmlElement* objElem = hRoot.FirstChild("object").Element();
    for (objElem; objElem; objElem = objElem->NextSiblingElement()) { // ������ȡnode�ڵ����Ϣ
        obj objTemp;
        TiXmlHandle node(objElem);  // objElem��ָ��Ľڵ�
        TiXmlElement* nameElem = node.FirstChild("name").Element();  // name�ڵ�
        memcpy(objTemp.label, nameElem->GetText(), strlen(nameElem->GetText()));

        TiXmlElement* xmin = node.FirstChild("bndbox").FirstChild("xmin").Element();  // xmin�ڵ�
        objTemp.xmin = atof(xmin->GetText());
        TiXmlElement* ymin = node.FirstChild("bndbox").FirstChild("ymin").Element();  // ymin�ڵ�
        objTemp.ymin = atof(ymin->GetText());
        TiXmlElement* xmax = node.FirstChild("bndbox").FirstChild("xmax").Element();  // xmax�ڵ�
        objTemp.xmax = atof(xmax->GetText());
        TiXmlElement* ymax = node.FirstChild("bndbox").FirstChild("ymax").Element();  // ymax�ڵ�
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
    p = localtime(&timep); //�˺�����õ�tm�ṹ���ʱ�䣬���Ѿ����й�ʱ��ת��Ϊ����ʱ��  
    var++;                       //p = gmtime(&timep); //�����ں�ʱ��ת��Ϊ��������(GMT)ʱ��ĺ���  
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
        cout << "��ͼƬ��·��Ϊ" << myMark.path << endl;
        if (R1.start < 0 || R1.end > src.cols || R2.start < 0 || R2.end >src.cols)
        {
            cout << "��ͼƬ" << myMark.path << "�����д���" << endl;
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

    HANDLE hFind;//HANDLE���  
    hFind = FindFirstFile(szPath.c_str(), &wfd);

    //���±�ʾFindFirstFile����ʧ��  
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
