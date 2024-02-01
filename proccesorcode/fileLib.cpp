#include "fileLib.h"
#include "debug.h"

size_t GetFileSize(FILE* file) 
{
    ASSERT(file != nullptr);

    fseek(file, 0, SEEK_END);

    size_t size = ftell(file);

    fseek(file, 0, SEEK_SET);

    return size;
}

void ReadBufToText(FileData* fileData)
{
    ASSERT(fileData != nullptr);

    fileData->text[0] = fileData->buf;
    for (size_t i = 0, j = 1; i < fileData->bufSize && j < fileData->textSize; i++)
    {//NOTE - j invalid size
        if (fileData->buf[i] == '\0')
        {
            ASSERT(j!=fileData->textSize);
            fileData->text[j] = fileData->buf + i + 1;
            j++;
        }
    }

}

size_t CountLines(FileData* fileData) // чекать последние символы
{
    ASSERT (fileData != nullptr);

    size_t counter = 0;

    for (size_t i = 0; i < fileData->bufSize; i++)
    {
        if (fileData->buf[i] == '\n')
            counter++;
    }
    
    return counter;
}

void SwapChars(FileData* fileData) // обьединитт 
{
    ASSERT(fileData != nullptr);
    for (size_t i = 0; i < fileData->bufSize; i++)
    {
        if (fileData->buf[i] == '\n') 
        {
            fileData->buf[i] = '\0';
        }
    }
}


void FileRead(FILE* file, FileData* fileData)
{
    ASSERT(file != nullptr);
    ASSERT(fileData != nullptr);

    fread(fileData->buf, sizeof(char), fileData->bufSize, file);
    *(fileData->buf + fileData->bufSize - 1) = '\0';
}

void Freedom(FileData* fileData)// занулить 
{
    ASSERT(fileData != nullptr);

    free(fileData->buf);
    free(fileData->text);
}

// void PrintfBuf(FileData* fileData)
// {
//     ASSERT(fileData != nullptr);
//     printf("#  bufSize: %lu\n", fileData->bufSize);
//     printf("#  textSize: %lu\n", fileData->textSize);
//     for (size_t i = 0; i < fileData->textSize; i++)
//        printf("%s", fileData->text[i]);
// }

void ParseFile(FILE* file, FileData* fileData)  // перенести вверх проверка на калок 
{
    ASSERT(file != nullptr);
    ASSERT(fileData != nullptr);

    fileData->bufSize = GetFileSize(file) + 1;

    fileData->buf = (char*)calloc(fileData->bufSize, sizeof(char)); 
    
    FileRead(file, fileData); 

    fileData->textSize = CountLines(fileData);

    fileData->text = (char**)calloc(fileData->textSize, sizeof(char*));

    SwapChars(fileData);
    
    ReadBufToText(fileData);
    
    //PrintfBuf(fileData);  
}

void WriteToFile(FileData* data, FILE* fileout)
{
    ASSERT(data != nullptr);
    ASSERT(fileout != nullptr);

    for (size_t i = 0; i < data->textSize; i++)
    {
        fputs(data->text[i], fileout);
        fputc('\n', fileout);
    }
}
