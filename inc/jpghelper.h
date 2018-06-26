#pragma once
#pragma warning(disable:4996)
#include <iostream>
#include <stdio.h>
extern "C" {
#include "jpeglib.h"
}
typedef enum _BIT_DATA_TYPE_ {
    BIT32 = 1,                      //��ȡ��洢��32λ
    BIT24 = 2,                      //��ȡ��洢��24λ
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
