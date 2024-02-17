#ifndef FILE_LIB_BIN_H
#define FILE_LIB_BIN_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "debug.h"

typedef uint8_t byte_t;

struct FileDataBin
{
    byte_t* buf;
    size_t bufSize;

    char** text;
    size_t textSize;
};

size_t GetFileSize(FILE* file);
void FileDataBinDtor(FileDataBin* fileDataBin);

#endif // FILE_LIB_H
