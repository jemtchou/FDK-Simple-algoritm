#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPicture>
#include <QImage>
#include <QPainter>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

protected:
   void paintEvent(QPaintEvent *){


       QImage img("img.jpg");


       QPainter painter(this);


       painter.drawImage(0,0, img.scaled(this->size())); // рисуем наше изображение от 0,0 и растягиваем по всему виджету


   };
};





#endif // MAINWINDOW_H
