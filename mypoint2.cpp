#include "mypoint2.h"
#include "QDebug"

MyPoint2::MyPoint2(int x, int y, QColor col){
    this->x = x;
   this->y = y;
    this->col = col;
}


void MyPoint2::draw (QPainter &painter)
{

    QPen pen = QPen(QBrush(col), 8);
    painter.setPen(pen);
    painter.drawLine(x, y, x, y+4);
}

