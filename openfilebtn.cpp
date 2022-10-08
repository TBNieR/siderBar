#include "openfilebtn.h"

#include <QAction>
#include <QVariant>
openfileBtn::openfileBtn(QWidget *parent)
    : QToolButton{parent}
{
    setContextMenuPolicy(Qt::DefaultContextMenu);
    rightMenu = new QMenu(this);
    QAction *dlt= new QAction(tr("删除"),this);
    this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->setFont(QFont("Sitka Small Semibold",10));
    this->setIcon(QIcon(":/folder.png"));
    connect(dlt,&QAction::triggered,this,[&]{
        QString path = this->property("path").toString();
        emit deleteSelf(path);
    });
    rightMenu->addAction(dlt);
    this->setMenu(rightMenu);
    this->setStyleSheet("QMenu{background-color:rgb(240, 236, 232);}");
}

void openfileBtn::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(this->property("path").toString()!=""){
        emit openfile(this->property("path").toString());
    }
}

void openfileBtn::contextMenuEvent(QContextMenuEvent *event)
{
    rightMenu->exec(event->globalPos());
}
