#include "mydata.h"

MyData::MyData(QObject *parent) : QObject(parent)
{

}

MyData::MyData(QObject *parent, int y, int maxX, QColor col, HANDLE hObject): QObject(parent) {
    this->y = y;
    this->maxX = maxX;
    this->col = col;
    this->hObject = hObject;
}
