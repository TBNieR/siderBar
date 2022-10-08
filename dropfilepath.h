#ifndef DROPFILEPATH_H
#define DROPFILEPATH_H

#include "openfilebtn.h"

#include <QDropEvent>
#include <QWidget>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QList>
#include <QSpacerItem>
#include <QVBoxLayout>

class DropFilePath : public QWidget
{
    Q_OBJECT
public:
    explicit DropFilePath(QWidget *parent = nullptr);
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    QList<QUrl> getList();
    void SetList(QList<QUrl> localeUrl);
    void setPaths(QList<QUrl>);
    QList<openfileBtn *> *btnList;
    ~DropFilePath();

private:
   QList<QUrl> urls;
   QVBoxLayout *layout;
   QSpacerItem *item;

signals:

private slots:
   void openFileInExplore(QString path);
   void deletePathRequest(QString &path);

};

#endif // DROPFILEPATH_H
