#include "weather.h"
#include "ui_weather.h"
#include <QJsonDocument>

#include <QJsonObject>
#include <QPixmap>
#include <QUrl>
Weather::Weather(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Weather)
{
    ui->setupUi(this);

    manager = new QNetworkAccessManager(this);
    getLoaclWeather();
    manager->get(QNetworkRequest(QUrl("http://whois.pconline.com.cn/ipJson.jsp?json=true")));
}

QString Weather::getLockIp()
{
    QString hostname = QHostInfo::localHostName();
        QHostInfo hostinfo = QHostInfo::fromName(hostname);
        QString localip = "";
        QList<QHostAddress> addList = hostinfo.addresses();
        if(!addList.isEmpty())
            for(int i = 0;i<addList.size();i++)
            {
                QHostAddress aHost = addList.at(i);
                if(QAbstractSocket::IPv4Protocol == aHost.protocol())
                {
                    localip = aHost.toString();
                    break;
                }
            }
        return localip;
}

void Weather::getLoaclWeather()
{

    connect(manager,&QNetworkAccessManager::finished,this,[&](QNetworkReply *reply){
        if(reply->error()!=QNetworkReply::NoError){
            ui->location->setText("获取地理位置失败");
        }else{
            //获取响应信息
             const QByteArray reply_data=reply->readAll();
             QString result = QString::fromLocal8Bit(reply_data);
             QJsonDocument doc = QJsonDocument::fromJson(result.toUtf8());
             if(doc.object().contains("city")){
                ui->location->setText(doc.object()["city"].toString());
                QString url = "https://autodev.openspeech.cn/csp/api/v2.1/weather?openId=aiuicus&clientType=android&sign=android&city="+doc.object()["city"].toString()+"&pageNo=1&pageSize=1";
                manager->get(QNetworkRequest(QUrl(url)));
             }else{
                QString weatherstr = QString::fromUtf8(reply_data);
                QJsonDocument wth = QJsonDocument::fromJson(weatherstr.toUtf8());
                QJsonObject weather = wth.object();
                if(weather["msg"]=="操作成功")
                    setWeather(weather["data"].toObject()["list"][0].toObject());
             }
       }
        reply->deleteLater();
    });

}

void Weather::setWeather(QJsonObject weather)
{
    qDebug()<<weather;

    ui->temp->setText(QString::number(weather["temp"].toDouble()).append("℃"));
    ui->weatherType->setText(weather["weather"].toString());
    //设置天气图标
    int weatherIcon = weather["weatherType"].toInt();
    QPixmap map;
    if(weatherIcon==0){
       map = QPixmap(":/weather/weather/sun.png");//晴天
    }else if(weatherIcon==1){
        map = QPixmap(":/weather/weather/cloudy.png");//多云
    }else if(weatherIcon==2){
        map = QPixmap(":/weather/weather/cloud.png");//阴天
    }else if(weatherIcon==3){
        map = QPixmap(":/weather/weather/");
    }else if(weatherIcon==7||weatherIcon==8||weatherIcon==3){
        map = QPixmap(":/weather/weather/rain.png");//雨
    }else if(weatherIcon==10||weatherIcon==9){
        map = QPixmap(":/weather/weather/bigrain.png");//大雨
    }else if(weatherIcon==14||weatherIcon==15){
        map = QPixmap(":/weather/weather/snow.png");//小中雪
    }else if(weatherIcon==16||weatherIcon==17){
        map = QPixmap(":/weather/weather/bigsnow.png");//大暴雪
    }else if(weatherIcon==18){
        map = QPixmap(":/weather/weather/fog.png");//雾天
    }else if(weatherIcon>18){
        map = QPixmap(":/weather/weather/dust.png");//沙尘暴
    }else{
        map = QPixmap(":/weather/weather/quest.png");
    }
    ui->weatherIcon->setPixmap(map);
}

Weather::~Weather()
{
    delete ui;
}
