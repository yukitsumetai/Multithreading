#ifndef MYPOINT2_H
#define MYPOINT2_H

#include <QColor>
#include <QPainter>



class MyPoint2
{
public:
  MyPoint2(int x=0, int y=0, QColor col=Qt::black);
  void draw(QPainter &painter);
private:
   QColor col;
   int x;
   int y;
};

#endif // MYPOINT2_H
