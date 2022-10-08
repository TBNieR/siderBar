#ifndef NETINFO_H
#define NETINFO_H

#include <QObject>

class NetInfo : public QObject
{
    Q_OBJECT
public:
    explicit NetInfo(QObject *parent = nullptr);

    bool isExit;
    void exitThread();
public slots:
    void doWork();
signals:
    void sendNewNetSpeed(const QString &speed);

};

#endif // NETINFO_H
