#ifndef MYDATA_H
#define MYDATA_H


#include <QObject>
#include "windows.h"
#include "mypoint.h"
#include "mypoint2.h"

class MyData : public QObject
{
    Q_OBJECT
public:
    explicit MyData(QObject *parent = nullptr);
    explicit MyData(QObject *parent, int y, int maxX, QColor col, HANDLE hObject);
    int y; //индивидуальная для потока координата Y
    QColor col; //индивидуальный цвет
    int maxX; // «докуда» рисовать - сколько точек можно нарисовать

    HANDLE hObject; //дескриптор синхронизирующего объекта

signals:
    void signalAddPoint(MyPoint2 myPoint); // добавлено новое данное и рисунок требует перерисовки:
public slots:
};


#endif // MYDATA_H
