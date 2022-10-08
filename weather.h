#ifndef WEATHER_H
#define WEATHER_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QHostInfo>
#include <QJsonObject>

namespace Ui {
class Weather;
}

class Weather : public QWidget
{
    Q_OBJECT

public:
    explicit Weather(QWidget *parent = nullptr);
    QString getLockIp();
    void getLoaclWeather();
    void setWeather(QJsonObject weather);
    ~Weather();

private:
    QNetworkAccessManager *manager ;
    Ui::Weather *ui;
};

#endif // WEATHER_H
