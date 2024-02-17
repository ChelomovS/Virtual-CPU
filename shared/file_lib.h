#ifndef FILE_LIB_H
#define FILE_LIB_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "debug.h"

typedef uint8_t byte_t;

struct FileData 
{
    char* buf;
    size_t bufSize;

    char** text;
    size_t textSize;
};

size_t GetFileSize(FILE* file);

void FileRead(FILE* file, FileData* fileData);

void ParseFile(FILE* file, FileData* fileData);

size_t CountLines(FileData* fileData);

void SwapChars(FileData* fileData);

void ReadBufToText(FileData* fileData);

void FileDataDtor(FileData* fileData);

//void PrintBuf(FileData* fileData);

void WriteToFile(FileData* data, FILE* fileout);

#endif // FILE_LIB_H