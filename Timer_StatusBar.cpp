#include "Timer_StatusBar.h"

Timer_StatusBar::Timer_StatusBar(const int &_interval,const int &_defaultPeriodNum)
    :interval(_interval)
    ,defaultPeriodNum(_defaultPeriodNum)
{

}

void Timer_StatusBar::run()
{
    count=0;

    while(status!=Over)
    {
        if(status==Pause)//当前处于停止状态
        {
            ;//donothing
        }
        else if(status==Showing)//当前处于显示状态
        {
//            qDebug()<<"Showing Count:"<<count;
            if(count<=0)//count超过周期数
            {
//                count=0;//重置计数
                status=Pause;//设为停止状态
                tempPeriodNum=0;//重置
                emit finish();//发送结束信号
                qDebug()<<"Countdown Finish";
                continue;
            }
            count--;
        }
        msleep(static_cast<unsigned long>( interval));//睡眠一个周期
    }

    return;
}

void Timer_StatusBar::startCountdown(const int &unit)
{
    this->status=Showing;//设置状态为显示
    if(unit>0)
        count=tempPeriodNum=unit;//设置临时周期数
    else
        count=defaultPeriodNum;//设置默认周期数
    return;
}

void Timer_StatusBar::stopTimer()
{
//    QMutexLocker locker(&mutex);//没必要加互斥锁
    status=Status::Over;
}

void Timer_StatusBar::resetTimer()
{
    status=Status::Pause;//暂停
    count=0;//重置
}









