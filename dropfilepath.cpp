#include "dropfilepath.h"

#include <QToolButton>
#include <QDesktopServices>
#include <QLayout>

DropFilePath::DropFilePath(QWidget *parent)
    : QWidget{parent}
{
    setAcceptDrops(true);
    layout = new QVBoxLayout(this);
    layout->setSpacing(10);
    item = new QSpacerItem(20,20,QSizePolicy::Preferred,QSizePolicy::Expanding);
    layout->addItem(item);
    btnList = new QList<openfileBtn *>;
    this->setLayout(layout);
}

void DropFilePath::dropEvent(QDropEvent *event)
{

    if(event->mimeData()->hasUrls()){
        QList<QUrl> u=event->mimeData()->urls();
        if(u.isEmpty())return;
           setPaths(u);
    }
}

void DropFilePath::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls()){
        event->acceptProposedAction();
    }
}

QList<QUrl> DropFilePath::getList()
{
    return urls;
}

void DropFilePath::SetList(QList<QUrl> localeUrl)
{
    setPaths(localeUrl);
}

void DropFilePath::setPaths(QList<QUrl> u)
{
    layout->removeItem(item);
    for(QUrl url: u){
        if(url.isLocalFile()&&btnList->size()<6){
               openfileBtn*btn = new openfileBtn(this);
               btn->setText(url.fileName());
               btn->setProperty("path",url.toString());
               connect(btn,&openfileBtn::openfile,this,&DropFilePath::openFileInExplore);
               connect(btn,&openfileBtn::deleteSelf,this,&DropFilePath::deletePathRequest);
               this->layout->addWidget(btn);
               this->btnList->push_back(btn);
               urls.push_back(url);
        }
    }//添加控件
   u.clear();
   this->layout->addItem(item);
}

DropFilePath::~DropFilePath()
{
    delete btnList;
}
//打开文件
void DropFilePath::openFileInExplore(QString path)
{
    QDesktopServices::openUrl(QUrl(path));
}
//删除文件路径
void DropFilePath::deletePathRequest(QString &path)
{
    openfileBtn *btn = qobject_cast<openfileBtn *>(sender());
    disconnect(btn,&openfileBtn::openfile,this,&DropFilePath::openFileInExplore);
    disconnect(btn,&openfileBtn::deleteSelf,this,&DropFilePath::deletePathRequest);
    btn->setParent(nullptr);
    this->layout->removeWidget(btn);
    btnList->removeAll(btn);
    urls.removeAll(btn->property("path").toString());
    update();
}
