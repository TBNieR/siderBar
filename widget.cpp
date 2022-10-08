#include "widget.h"
#include "ui_widget.h"

#include <QStorageInfo>
#include <QDesktopServices>
#include <QTimer>
#include <QDateTime>
#include <QTime>
#include <QScreen>
#include <QJsonDocument>
#include <QJsonArray>
#include <QUrl>
#include <QJsonArray>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    cpu = new Controller(this);
    timer = new QTimer(this);
    speed = new QThread(this);
    netinfo = new NetInfo;
    netinfo->moveToThread(speed);
    InitUI();
    InitSignalAndSlot();
    getStorageInfo();
    speed->start();
    Weather ther;
    readSetting();
}

Widget::~Widget()
{
    config["portrait"]=ui->widget_2->getPortrait();
    config["nickName"]=ui->lineEdit->text();
    QList<QUrl> list=ui->fileDropWidget->getList();
    QJsonArray array;
     for(QUrl url: list){
        array.append(url.toString());
     }
     config["list"]=array;
    QFile setting("./config.ini");
    if(setting.open(QFile::WriteOnly)){
        QJsonDocument doc(config);
        QByteArray arry = QString(doc.toJson(QJsonDocument::Indented)).toUtf8();
        setting.write(arry);
    }
    setting.close();
    delete ui;
    netinfo->exitThread();
    speed->quit();
    speed->wait();

}

void Widget::InitUI()
{
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnBottomHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    QPalette palette = QPalette();
    palette.setColor(QPalette::Window,QColor(0x00,0xFF,0x00,0x00));
    setPalette(palette);
    //将窗口背景设为透明
    this->setStyleSheet("QToolButton,QWidget{"
                        "background-color:rgb(48,45,65);"
                        "border-radius:10px;"
                        "}");//toolbutton color
//    设置背景色
    this->move(qApp->primaryScreen()->size().width()-this->width()-20,60);
//    设置窗口位置
    GlobalColor = QColor(48,45,65);
}

void Widget::InitSignalAndSlot()
{
    connect(timer,&QTimer::timeout,this,[=]{
        int cpupre = cpu->getCpuUsage();
        ui->cpuprogressBar->setValue(cpupre>20? cpu->getCpuUsage():0);
        ui->cpuprogressBar->setToolTip(QString::number(cpupre));
        QDateTime date = QDateTime::currentDateTime();
        QTime time = date.time();
        QDate Date =  date.date();
        ui->pra->setText(time.toString("a"));
        ui->week->setText(Date.toString("dddd"));
        ui->hour->setText(time.toString("hh"));
        ui->min->setText(time.toString("mm"));
    });
    timer->start(1000);

    connect(speed,&QThread::finished,netinfo,&NetInfo::deleteLater);
    connect(speed,&QThread::started,netinfo,&NetInfo::doWork);
    connect(netinfo,&NetInfo::sendNewNetSpeed,this,&Widget::updateNetInfo);
    connect(ui->closeBtn,&QToolButton::clicked,this,&Widget::shutdown);//关机
    connect(ui->restartBtn,&QToolButton::clicked,this,[]{
        system("shutdown -r -t 00");
    });//重启
    connect(ui->bilibili,&QToolButton::clicked,this,[]{
        QDesktopServices::openUrl(QUrl("www.bilibili.com",QUrl::TolerantMode));
    });
    connect(ui->github,&QToolButton::clicked,this,[]{
        QDesktopServices::openUrl(QUrl("https://github.com/",QUrl::TolerantMode));
    });
    connect(ui->music,&QToolButton::clicked,this,[]{
        QUrl music;
        if(QFile::exists("C:/ProgramData/Microsoft/Windows/Start Menu/Programs/网易云音乐.lnk")){
            music = QUrl("file:///C:/ProgramData/Microsoft/Windows/Start Menu/Programs/网易云音乐.lnk");
            QDesktopServices::openUrl(music);
        }
    });
    connect(ui->computer,&QToolButton::clicked,this,[]{
        WinExec("calc",SW_HIDE);
    });
    connect(ui->terminal,&QToolButton::clicked,this,[]{
        system("start cmd /K ");
    });
}

void Widget::readSetting()
{
    QFile file("./config.ini");
    if(QFile::exists("./config.ini")){

        if(file.open(QFile::ReadOnly)){
            QByteArray settings = file.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(settings);
            config = doc.object();
            if(!doc.isEmpty()){
                //头像路径
                QString port = config["portrait"].toString();
                ui->widget_2->setPortrait(port);
//                名称
                QString nickname = (config["nickName"].toString()=="")? "Administrator":config["nickName"].toString();
                ui->lineEdit->setText(nickname);
//                    快捷文件链接
                if(!config["list"].isNull()){
                    QJsonArray linkArray = doc["list"].toArray();
                    QList<QUrl> linkList;
                    for (int var = 0; var < linkArray.size(); ++var) {
                        linkList.push_back(QUrl(linkArray[var].toString()));
                    }
                    ui->fileDropWidget->SetList(linkList);
                }
            }//读取设定
        }
    }else{
        file.open(QFile::WriteOnly);
    }
    file.close();
}

void Widget::getStorageInfo()
{
    QList<QStorageInfo> list = QStorageInfo::mountedVolumes();
    //C:
    QStorageInfo diskInfo = list.at(0);
    int freeSize = diskInfo.bytesFree()/1024/1024/1024;
    int totalSize = diskInfo.bytesTotal()/1024/1024/1024;
    QString C = diskInfo.displayName();
    ui->cprogressBar->setMaximum(totalSize);
    ui->cprogressBar->setValue(totalSize-freeSize);
    int pres = (double)freeSize/(double)totalSize*100;
    ui->cprogressBar->setToolTip(C.append("    ").append(QString::number(pres)).append("%"));
    ui->cLabel->setText(QString::number(100-pres).append("%"));
    //D:
    QStorageInfo ddiskInfo = list.at(1);
    qint64 dfreeSize = ddiskInfo.bytesFree()/1024/1024/1024;
    qint64 dtotalSize = ddiskInfo.bytesTotal()/1024/1024/1024;
    QString D = ddiskInfo.displayName();
    ui->dprogressBar->setMaximum(dtotalSize);
    ui->dprogressBar->setValue(dtotalSize-dfreeSize);
    int dpres = (double)dfreeSize/(double)dtotalSize*100;
    ui->dprogressBar->setToolTip(D.append("    ").append(QString::number(dpres)).append("%"));
}

void Widget::shutdown()
{
    qDebug()<<"shutdown";
    system("shutdown -s -t 00");
}

void Widget::updateNetInfo(QString netinfo)
{
    ui->speed->setText(netinfo);
}

