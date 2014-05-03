#include "paintwidget.h"
#include <QtCore/QLine>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QImage>
#include "fileopen.h"
#include "corefdksimple.h"
#include "imgsave.h"
#include <ctime>

PaintWidget::PaintWidget(QWidget *parent)
    : QWidget(parent)
{
    std::cout<<"Class PaintWidget"<<std::endl;
    setFixedSize(600, 600);
}

PaintWidget::~PaintWidget()
{std::cout<<"~PaintWidget"<<std::endl;
}

void PaintWidget::paintEvent(QPaintEvent *event)
{
     QRgb value;
/*
    FileOpen* file = new FileOpen();
    file->OpenFile();
    uint32 w=file->GetW();
    uint32 h=file->GetH();
    int I=290;

    double max=0;
    for (uint32 i = 0; i < w; i++){
        for (uint32 j = 0; j < h; j++){
            if(max<file->GetBuffer(I,j,i)){ max = file->GetBuffer(I,j,i);}
        }
    }

    QImage img(w, h, QImage::Format_RGB32);
    for (uint32 i = 0; i < w; i++){
        for (uint32 j = 0; j < h; j++){
            int  buffe = ((int)(((file->GetBuffer(I,j,i))/((double)(max))*256)));
            value = qRgb((buffe),(buffe), (buffe));
            img.setPixel(i, j, value);
        }

    }
*/
     clock_t totaltime;
     totaltime = clock();

     std::cout<<"Started FDK"<<std::endl;
    coreFDKsimple core = coreFDKsimple();
    core.Init();
    core.FDK();
    int x = core.getX();
    int y = core.getY();

    float **image;
    image = NULL;
    image = (float **)malloc(sizeof(float *) * x);
    for (int i = 0; i < x; i++)
      image[i] = (float *)malloc(sizeof(float) * y);

   double max=0;
   double min=0;

     for (int i = 0; i < y; i++){
        for (int j = 0; j < x; j++){

            if(max < core.output(j,i)){ max = core.output(j,i);}
            if(min > core.output(j,i)){ min = core.output(j,i);}
        }
    }

    std::cout<<"End FDK"<<std::endl;
    QImage img(x, y, QImage::Format_RGB32);
    std::cout<<"Starting setPixels"<<std::endl;

    int buffe;
    for (int i = 0; i < y; i++){
        for (int j = 0; j < x; j++){
            image[i][j] = core.output(i,j);
            buffe = (int)(((core.output(i,j)-min)/(max-min))*255);
            value = qRgb((buffe),(buffe),(buffe));
            img.setPixel(i, j, value);
        }
    }

    Imgsave (image, "test", x, y, 8, 1);
    QPainter painter(this);
    painter.drawImage(0, 0, img);
    totaltime = clock() - totaltime;
    std::cout << "Total time is " << ((float)totaltime)/CLOCKS_PER_SEC << std::endl;

    for (int i = 0; i < x; i++)
       free(image[i]);
    free(image);
}
