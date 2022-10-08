#ifndef OPENFILEBTN_H
#define OPENFILEBTN_H

#include <QContextMenuEvent>
#include <QMenu>
#include <QToolButton>

class openfileBtn : public QToolButton
{
    Q_OBJECT
public:
    explicit openfileBtn(QWidget *parent = nullptr);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
signals:
       void openfile(QString path);
       void deleteSelf(QString &path);
private:
       QMenu *rightMenu;
};

#endif // OPENFILEBTN_H
