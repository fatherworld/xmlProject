#pragma once
#pragma warning(disable:4996)
#include <iostream>
#include <stdio.h>
extern "C" {
#include "jpeglib.h"
}
typedef enum _BIT_DATA_TYPE_ {
    BIT32 = 1,                      //读取或存储成32位
    BIT24 = 2,                      //读取或存储成24位
}BITDATATYPE;

typedef struct _JPG_ {
    int width;
    int height;
    int channels;
    unsigned char * data;
}JPG;

int LoadJpg(const char * file, JPG* out_img, BITDATATYPE bit_data_type);
int WriteJpg(const char * file, JPG * in_img, BITDATATYPE bit_data_type);
int FreeJpg(JPG* img);
