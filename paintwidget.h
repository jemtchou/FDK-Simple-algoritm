#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QList>
#include <QPoint>

class PaintWidget : public QWidget
{
    Q_OBJECT

public:
    PaintWidget(QWidget *parent = 0);
    ~PaintWidget();

protected:

    virtual void paintEvent(QPaintEvent *event);
private:


};

#endif // PAINTWIDGET_H
