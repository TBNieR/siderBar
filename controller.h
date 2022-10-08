#ifndef CONTROLLER_H
#define CONTROLLER_H

#pragma once

#include <QObject>
#include <QThread>
#include "worker.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject* parent = nullptr);
    ~Controller();

    int getCpuUsage();

signals:
    void startRunning(); // 用于触发新线程中的耗时操作函数

public slots:
    void on_receivResult(const int dosage); // 接收新线程中的结果

private:
    QThread m_workThread;
    Worker* m_worker;

    int cpuDosage = 0;
};
#endif // CONTROLLER_H
