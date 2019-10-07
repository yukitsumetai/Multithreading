#ifndef MYWIDGET_H
#define MYWIDGET_H

#include "mypoint2.h"

#include <QPaintEvent>
#include <QWidget>



class MyDemoWidget: public QWidget
{
    Q_OBJECT
private:


public:
   explicit MyDemoWidget(QWidget* parent);
    ~MyDemoWidget() override;
  QVector<MyPoint2> m_vector;
    void clearPoints();

public slots:
    void slotAddPoint(MyPoint2 myPoint);

protected:
    virtual void paintEvent(QPaintEvent * event) override;
};
#endif // MYWIDGET_H
