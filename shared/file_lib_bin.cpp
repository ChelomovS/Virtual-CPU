#include "file_lib_bin.h"
#include "debug.h"

void FileDataBinDtor(FileDataBin* fileDataBin)
{
    ASSERT(fileDataBin != nullptr);

    fileDataBin->bufSize = 0;
    fileDataBin->textSize = 0;

    free(fileDataBin->buf);
    free(fileDataBin->text);
}