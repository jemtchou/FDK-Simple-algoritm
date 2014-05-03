#ifndef FILEOPEN_H
#define FILEOPEN_H

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <tiffio.h>
#include <iostream>
#include <QDir>
#include <QString>
#include <QStringList>

class FileOpen
{

public:

    FileOpen();
    ~FileOpen();
    void OpenFile();
    void Init();
    int GetW() {return W;}
    int GetH() {return H;}
    float GetBuffer(int I, int j, int i) {return buffer[I][j][i];}
    int getMAX() {return MAX;}

    TIFF* tif;
    float ***buffer;
    uint32 W, H;
    int I;
    QDir* dir;

private:

    float *bulk;
    uint32 row;
    tdata_t buf;
    QString Filename;
    QStringList list;
    int MAX;

};

#endif // FILEOPEN_H
