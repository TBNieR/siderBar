#ifndef WORKER_H
#define WORKER_H
#include "windows.h"
#include <QObject>
#include <QThread>


class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject* parent = nullptr) { };

protected:
    //时间转换
    __int64 Filetime2Int64(const FILETIME* ftime)
    {
        LARGE_INTEGER li;
        li.LowPart = ftime->dwLowDateTime;
        li.HighPart = ftime->dwHighDateTime;
        return li.QuadPart;
    }
    //两个时间相减运算
    __int64 CompareFileTime(FILETIME preTime, FILETIME nowTime)
    {
        return this->Filetime2Int64(&nowTime) - this->Filetime2Int64(&preTime);
    }

signals:
    void resultReady(const int dosage); // 向外界发送结果

public slots:
    void calCpuUsage()// 耗时操作
    {
        HANDLE hEvent;
        bool res;
        static FILETIME preIdleTime;
        static FILETIME preKernelTime;
        static FILETIME preUserTime;

        FILETIME idleTime;
        FILETIME kernelTime;
        FILETIME userTime;

        res = GetSystemTimes(&idleTime, &kernelTime, &userTime);

        preIdleTime = idleTime;
        preKernelTime = kernelTime;
        preUserTime = userTime;

        hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

        WaitForSingleObject(hEvent, 500);	//等待500毫秒

        res = GetSystemTimes(&idleTime, &kernelTime, &userTime);

        int idle = CompareFileTime(preIdleTime, idleTime);
        int kernel = CompareFileTime(preKernelTime, kernelTime);
        int user = CompareFileTime(preUserTime, userTime);

        auto nCpuRate = (int)ceil(100.0 * (kernel + user - idle) / (kernel + user));

        emit resultReady(nCpuRate);
    }
};
#endif // WORKER_H
