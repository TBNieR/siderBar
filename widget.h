#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QColor>
#include <Windows.h>
#include <QFile>
#include <QThread>
#include "NetInfo.h"
#include "weather.h"
#include "controller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    QColor GlobalColor;
    Controller *cpu;
    QTimer* timer;
    NetInfo *netinfo;
    QThread *speed;
    QJsonObject config;
    void InitUI();
    void InitSignalAndSlot();
    void readSetting();
    void getStorageInfo();
private slots:
      void shutdown();
      void updateNetInfo(QString netinfo);
};
#endif // WIDGET_H
