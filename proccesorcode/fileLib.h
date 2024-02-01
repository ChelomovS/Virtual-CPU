#ifndef FILELIB_H
#define FILELIB_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "debug.h"

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

void Freedom(FileData* fileData);

void PrintBuf(FileData* fileData);

void WriteToFile(FileData* data, FILE* fileout);

#endif // FILELIB_H
