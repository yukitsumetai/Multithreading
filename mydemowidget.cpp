#include "mydemowidget.h"
#include "QDebug"

#include <QPaintEvent>

MyDemoWidget::MyDemoWidget(QWidget *parent) : QWidget(parent)
{
qRegisterMetaType<MyPoint2>("MyPoint2");
m_vector = QVector<MyPoint2>();
}

MyDemoWidget::~MyDemoWidget()
{
    this->clearPoints();
}

void MyDemoWidget::slotAddPoint(MyPoint2 m){
    m_vector.push_back(m);
  repaint();
}

void MyDemoWidget::paintEvent(QPaintEvent* event){

    QPainter painter(this);
    auto itb = m_vector.begin();
    auto ite = m_vector.end();

    while(itb!=ite){
        MyPoint2 tmp= *itb;
        tmp.draw(painter);
        ++itb;
    }

}

void MyDemoWidget::clearPoints() {
   m_vector.clear();
}
