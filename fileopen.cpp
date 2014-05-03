#include"fileopen.h"
#include <cmath>
#include <ctime>

FileOpen::FileOpen(){
    std::cout<<"FileOpen()"<<std::endl;

}

FileOpen::~FileOpen(){
    std::cout<<"~FileOpen()"<<std::endl;
    for (int i = 0; i < MAX; i++)
     {
       for(uint32 j = 0; j < H; j++)
         free(buffer[i][j]);
       free(buffer[i]);
     }
    free(buffer);

    free(bulk);
}

void FileOpen::Init(){
    std::cout<<"FileOpen Init()"<<std::endl;
    dir = new QDir("/home/andrew/Projections/calibrated_fantom/projections/SPos1/Energy1");
    list = dir->entryList(QStringList() << "*.tif", QDir::Files);
    MAX=list.size();

    buffer = NULL;
    bulk = NULL;
    buf = NULL;
}

void FileOpen::OpenFile(){
    std::cout<<"OpenFile()"<<std::endl;
    clock_t opentime;
    opentime= clock();

    /*Цикл по всем картинками в директории*/

    for (int I = 0; I<MAX; I++){

    /*Делаем полное имяфайла - имя файла + имя дирекории*/
    Filename = dir->absoluteFilePath(list[I]);
    tif = TIFFOpen (Filename.toStdString().c_str(), "r");

    /*Получаем размеры открытой картинки*/
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &W);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &H);

    /*Выделение памяти*/
    if (I==0){
    buffer = (float ***)malloc(sizeof(float *) * MAX);
    for (int i = 0; i < MAX; i++)
    {
      buffer[i] = (float **)malloc(sizeof(float *) * H);

      for(uint32 j = 0; j < H; j++)
        buffer[i][j] = (float *)calloc(W, sizeof(float));
    }

    bulk = (float *)malloc(sizeof(float) * W * H);
    } //if (I==0)

    buf = _TIFFmalloc(TIFFScanlineSize(tif));
    if (buf == NULL)
    {
      TIFFClose(tif);
    }

    /*Читаем строку картинки и кипируем её в память bulk*/
   for (row = 0; row < H; row++)
     {
       TIFFReadScanline(tif, buf, row);
       memcpy(&bulk[row * W], buf, W * 4);
     }
    _TIFFfree(buf);
    TIFFClose(tif);

    /*Заполнение тензора*/
    for(uint32 x = 0; x < W; x++){
      for(uint32 y = 0; y < H; y++)
        buffer[I][y][x] = (bulk[x + y * W]);
    }
    }
    opentime = clock()-opentime;
    std::cout << "Opentime is " << ((float)opentime)/CLOCKS_PER_SEC << std::endl;

}

