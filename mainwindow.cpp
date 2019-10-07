#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "windows.h"
#include "mydata.h"
#include "mypoint2.h"
#include <QPainter>
#include <QVectorIterator>
#include <QMessageBox>
#include <QDebug>


int MainWindow::m_X = 5;// define and initialize
int MainWindow::delay = 9999999;
CRITICAL_SECTION MainWindow::cs;
HANDLE MainWindow::unnamedMutex;
HANDLE MainWindow::namedMutex;
HANDLE MainWindow::unnamedSemaphore;
HANDLE MainWindow::namedSemaphore;
HANDLE MainWindow::unnamedManualEvent;
HANDLE MainWindow::namedManualEvent;
HANDLE MainWindow::autoEvent;

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    t[0] = new MyData(this, 20, 20, Qt::red, 0);
    t[1] = new MyData(this, 70, 20, Qt::blue, 0);
    t[2] = new MyData(this, 120, 20, Qt::green, 0);

    for(int i=0; i<3; i++)
    {
        QObject::connect(t[i], SIGNAL(signalAddPoint(MyPoint2)), ui->MyWidget, SLOT(slotAddPoint(MyPoint2)), Qt::QueuedConnection);
    }
}

MainWindow::~MainWindow()
{

   this->clearPoints();
    this->clearHandlers();
    delete t[0];
 delete t[1];
    delete t[2];


    DeleteCriticalSection(&MainWindow::cs);
    CloseHandle(unnamedMutex);
    CloseHandle(namedMutex);
    CloseHandle(unnamedSemaphore);
    CloseHandle(namedSemaphore);
    CloseHandle(unnamedManualEvent);
    CloseHandle(namedManualEvent);
    CloseHandle(autoEvent);
    delete ui;
}

void MainWindow::clearHandlers() {

    for(int i=0; i<3; i++)
    {
        if (arh[i] != 0) {
            CloseHandle(firstThread);
        }
    }
}

DWORD WINAPI ThreadFunc(LPVOID threadParam){

    MyData* myData = static_cast<MyData*> (threadParam);
 qDebug()<<myData->y;
    int drawnAlready = 0;
    while (drawnAlready < myData->maxX) {
        MyPoint2 m = MyPoint2(MainWindow::m_X, myData->y, myData->col);
        //програмная задержка
        for (int i = 0; i < MainWindow::delay; i++) {
            i+=2;
            i-=1;
        }

        emit myData->signalAddPoint(m);
        MainWindow::m_X +=10;
        drawnAlready++;
    }
}

void MainWindow::clearPoints() {
    ui->MyWidget->m_vector.clear();
    m_X = 5;
}

void MainWindow::resumeThreads() {
    for(int i=0; i<3; i++)
    {
     ResumeThread(arh[i]);
    }
}

void MainWindow::createThreads(LPTHREAD_START_ROUTINE func) {
    for(int i=0; i<3; i++)
    {
    arh[i] = CreateThread(0,0,func,t[i],CREATE_SUSPENDED,0);
    }
}

DWORD WINAPI ThreadFuncWithCriticalSection(LPVOID threadParam){
    EnterCriticalSection(& MainWindow::cs);
    ThreadFunc(threadParam);
    LeaveCriticalSection(& MainWindow::cs);
}

void MainWindow::on_samePriorityButton_clicked()
{

    SetProcessAffinityMask(GetCurrentProcess(), 1);

    this->clearPoints();
    this->clearHandlers();

    createThreads(ThreadFunc);

    setPriority(arh[0], ui->highPrioT1, ui->medPrioT1, ui->lowPrioT1);
    setPriority(arh[1], ui->highPrioT2, ui->medPrioT2, ui->lowPrioT2);
    setPriority(arh[2], ui->highPrioT3, ui->medPrioT3, ui->lowPrioT3);

    resumeThreads();
}

void MainWindow::setPriority(HANDLE thread, QRadioButton* highPrio, QRadioButton* midPrio, QRadioButton* lowPrio) {
    if (highPrio->isChecked()) {
        SetThreadPriority(thread, THREAD_PRIORITY_HIGHEST);
    } else if (midPrio->isChecked()) {
        SetThreadPriority(thread, THREAD_PRIORITY_NORMAL);
    } else if (lowPrio->isChecked()) {
        SetThreadPriority(thread, THREAD_PRIORITY_LOWEST);
    }
}


void MainWindow::on_startThreadsWithCriticalSectionButton_clicked()
{
  DeleteCriticalSection(& MainWindow::cs);
    InitializeCriticalSection(& MainWindow::cs);

    SetProcessAffinityMask(GetCurrentProcess(), 1);
this->clearHandlers();
    this->clearPoints();


     createThreads(ThreadFuncWithCriticalSection);
     resumeThreads();



}


DWORD WINAPI ThreadFuncWithMutex(LPVOID threadParam){
    WaitForSingleObject(MainWindow::unnamedMutex, INFINITE);
    ThreadFunc(threadParam);
    ReleaseMutex(MainWindow::unnamedMutex);
}

DWORD WINAPI ThreadFuncWithNamedMutex(LPVOID threadParam){
    WaitForSingleObject(MainWindow::namedMutex, INFINITE);
    ThreadFunc(threadParam);
    ReleaseMutex(MainWindow::namedMutex);
}


void MainWindow::on_startThreadsWithMutexButton_clicked()
{
    CloseHandle(unnamedMutex);
    clearHandlers();
    clearPoints();
    unnamedMutex = CreateMutex(0, false, 0);

    SetProcessAffinityMask(GetCurrentProcess(), 1);



    createThreads(ThreadFuncWithMutex);
    resumeThreads();
    //WaitForMultipleObjects(3,arh,TRUE,INFINITE);

}

void MainWindow::on_startProcessWithNamedMutexButton_clicked()
{
    CloseHandle(namedMutex);
    namedMutex = CreateMutex(0, false, L"myMutex");
    SetProcessAffinityMask(GetCurrentProcess(), 1);

    this->clearPoints();
    this->clearHandlers();

    createThreads(ThreadFuncWithNamedMutex);
    resumeThreads();
}

DWORD WINAPI ThreadFuncWithSemaphore(LPVOID threadParam){
    WaitForSingleObject(MainWindow::unnamedSemaphore, INFINITE);
    ThreadFunc(threadParam);
    ReleaseSemaphore(MainWindow::unnamedSemaphore, 1, 0);
}

DWORD WINAPI ThreadFuncWithNamedSemaphore(LPVOID threadParam){
    WaitForSingleObject(MainWindow::namedSemaphore, INFINITE);
    ThreadFunc(threadParam);
    ReleaseSemaphore(MainWindow::namedSemaphore, 1, 0);
}



void MainWindow::on_startThreadsWithSemaphoreButton_clicked()
{
CloseHandle(unnamedSemaphore);
    unnamedSemaphore = CreateSemaphore(0, 3, 3, 0);

    SetProcessAffinityMask(GetCurrentProcess(), 1);

    this->clearPoints();
    this->clearHandlers();

    createThreads(ThreadFuncWithSemaphore);
    resumeThreads();
}

void MainWindow::on_startProcessWithNamedSemaphoreButton_clicked()
{
 CloseHandle(namedSemaphore);
    namedSemaphore = CreateSemaphore(0, 1, 1, L"mySemaphore");

    SetProcessAffinityMask(GetCurrentProcess(), 1);

    this->clearPoints();
    this->clearHandlers();

    createThreads(ThreadFuncWithNamedSemaphore);
    resumeThreads();
}

DWORD WINAPI ThreadFuncWithManualEvent(LPVOID threadParam){
    MyData* myData = static_cast<MyData*> (threadParam);
    int drawnAlready = 0;
    while (drawnAlready < myData->maxX) {
        WaitForSingleObject(MainWindow::unnamedManualEvent, INFINITE);
        MyPoint2 m = MyPoint2(MainWindow::m_X, myData->y, myData->col);
        for (int i = 0; i < MainWindow::delay; i++) {
            i+=2;
            i-=1;
        }
        emit myData->signalAddPoint(m);
        MainWindow::m_X += 10;
        drawnAlready++;
    }
}

void MainWindow::on_startManualEventsButton_clicked()
{
    CloseHandle(unnamedManualEvent);

    unnamedManualEvent = CreateEvent(0, true, true, 0);

    SetProcessAffinityMask(GetCurrentProcess(), 1);

    this->clearPoints();
    this->clearHandlers();

    createThreads(ThreadFuncWithManualEvent);
    resumeThreads();
   }

void MainWindow::on_stopManualEventsButton_clicked()
{
    ResetEvent(unnamedManualEvent);
}

void MainWindow::on_continueManualEventsButton_clicked()
{
    SetEvent(unnamedManualEvent);
}


DWORD WINAPI ThreadFuncWithNamedManualEvent(LPVOID threadParam){
    MyData* myData = static_cast<MyData*> (threadParam);
    int drawnAlready = 0;
    while (drawnAlready < myData->maxX) {
        WaitForSingleObject(MainWindow::namedManualEvent, INFINITE);
        MyPoint2 m = MyPoint2(MainWindow::m_X, myData->y, myData->col);
        for (int i = 0; i < MainWindow::delay; i++) {
            i+=2;
            i-=1;
        }
        emit myData->signalAddPoint(m);
        MainWindow::m_X += 10;
        drawnAlready++;
    }
}

void MainWindow::on_startProcessWithNamedEventButton_clicked()
{
    CloseHandle(namedManualEvent);
    namedManualEvent = CreateEvent(0, true, true, L"myEvent");

    SetProcessAffinityMask(GetCurrentProcess(), 1);

    this->clearPoints();
    this->clearHandlers();

    createThreads(ThreadFuncWithNamedManualEvent);
    resumeThreads();
}


void MainWindow::on_stopManualEventsButton2_clicked()
{
    ResetEvent(namedManualEvent);
}

void MainWindow::on_continueManualEventsButton2_clicked()
{
    SetEvent(namedManualEvent);
}


DWORD WINAPI ThreadFuncWithAutoEvent(LPVOID threadParam){
    WaitForSingleObject(MainWindow::autoEvent, INFINITE);
    ThreadFunc(threadParam);
    SetEvent(MainWindow::autoEvent);
}


void MainWindow::on_startAutoEventsButton_clicked()
{

 CloseHandle(autoEvent);
    autoEvent = CreateEvent(0, false, true, 0);

    SetProcessAffinityMask(GetCurrentProcess(), 1);

    this->clearPoints();
    this->clearHandlers();

    createThreads(ThreadFuncWithAutoEvent);
    resumeThreads();

}
