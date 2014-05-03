#include "mainwindow.h"

#include <iostream>
#include <QApplication>
#include <QtGui>
#include <QLabel>
#include <QPainter>
#include <QImage>
#include <QColor>
#include <QLayout>
#include<QPixmap>
#include "paintwidget.h"
#include <unistd.h>

class Widget : public QWidget {
protected:
  void paintEvent(QPaintEvent*) {
    QPainter p(this);
    std::cout<<"Class Widget"<<std::endl;

  }
};



int main(int argc, char *argv[])
{
    std::cout<<"Started"<<std::endl;

    QApplication a(argc, argv);
   // MainWindow w;

    PaintWidget w1;
    w1.show();

    std::cout<<"RETURN A.EXEC"<<std::endl;
    return a.exec();

}
