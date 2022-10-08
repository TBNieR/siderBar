#include "Controller.h"

#include <QThread>
#include <QDebug>

Controller::Controller(QObject* parent) : QObject(parent)
{
    m_worker = new Worker();
    m_worker->moveToThread(&m_workThread);

    connect(this, &Controller::startRunning, m_worker, &Worker::calCpuUsage);
    connect(&m_workThread, &QThread::finished, m_worker, &QObject::deleteLater);
    connect(m_worker, &Worker::resultReady, this, &Controller::on_receivResult);
}

Controller::~Controller()
{
    m_workThread.quit();
    m_workThread.wait();
}

int Controller::getCpuUsage()
{
    m_workThread.start();
    emit startRunning();
    return cpuDosage;
}

void Controller::on_receivResult(const int dosage)
{
    cpuDosage = dosage;
}
