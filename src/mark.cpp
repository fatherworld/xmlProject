#include"mark.h"

mark::mark()
{
    this->path = (const char*)malloc(sizeof(char)*50);
    memset((char*)(this->path), 0, 50);
}

void mark::setPath(const char* path, int length)
{
    if (!path)
    {
        return;
    }
    memcpy((char*)this->path, path, length);
}

void mark::freeMem()
{
    if (this->path)
    {
        free((void*)this->path);
        this->path = NULL;
    }
}