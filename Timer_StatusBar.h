#ifndef TIMER_STATUSBAR_H
#define TIMER_STATUSBAR_H

#include <QThread>
#include <QLabel>
#include <QColor>
#include <QDebug>
#include <QMutex>
#include <QMutexLocker>


constexpr int DefaultPeriodNum=20;//默认周期数
constexpr int DefaultInterval=250;//默认周期(毫秒数)

class Timer_StatusBar:public QThread
{
    Q_OBJECT
public:
    enum Status
    {
        Showing,//显示状态
        Pause,//停止(暂停)状态
        Over,//结束
        End_Status//哨兵
    };
    static constexpr int DEFAULT=0;


public:
//    QLabel *label_target;//目标label
    int interval{DefaultInterval};//两次轮询之间的时间间隔(毫秒单位)
    int count{0};
    int defaultPeriodNum{DefaultPeriodNum};//默认周期数
    int tempPeriodNum{0};
    Status status;//当前状态

    QMutex mutex;//互斥量

public:
    //构造函数
    Timer_StatusBar(const int &interval=DefaultInterval,const int &defaultPeriodNum=DefaultPeriodNum);


public:

    void run();//线程运行(重载)

    //开始倒计时
    void startCountdown(const int &unit=DEFAULT);
    void stopTimer();
    void resetTimer();

signals:
    void finish();//倒计时结束

};

#endif // TIMER_STATUSBAR_H
