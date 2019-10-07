#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include "mypoint.h"
#include "windows.h"
#include "mydata.h"
#include <QRadioButton>

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

    HANDLE arh[3];
    HANDLE firstThread;
    HANDLE secondThread;
    HANDLE thirdThread;

    MyData* t[3];

public:
    static int m_X;
    static int delay;
    static CRITICAL_SECTION cs;
    static HANDLE unnamedMutex;
    static HANDLE namedMutex;
    static HANDLE unnamedSemaphore;
    static HANDLE namedSemaphore;
    static HANDLE unnamedManualEvent;
    static HANDLE namedManualEvent;
    static HANDLE autoEvent;
private:
    void clearHandlers();
    void clearPoints();
    void resumeThreads();
    void createThreads(LPTHREAD_START_ROUTINE func);
    void setPriority(HANDLE thread, QRadioButton* highPrio, QRadioButton* midPrio, QRadioButton* lowPrio);

private slots:
    void on_samePriorityButton_clicked();
    void on_startThreadsWithCriticalSectionButton_clicked();

    void on_startThreadsWithMutexButton_clicked();
    void on_startProcessWithNamedMutexButton_clicked();

    void on_startProcessWithNamedSemaphoreButton_clicked();
    void on_startThreadsWithSemaphoreButton_clicked();

    void on_startManualEventsButton_clicked();
    void on_stopManualEventsButton_clicked();
    void on_continueManualEventsButton_clicked();
    void on_stopManualEventsButton2_clicked();
    void on_continueManualEventsButton2_clicked();
    void on_startProcessWithNamedEventButton_clicked();
    void on_startAutoEventsButton_clicked();
signals:
     void signalAddPoint(MyPoint2 myPoint);
};

#endif // MAINWINDOW_H
