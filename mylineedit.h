#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QLineEdit>
#include <QKeyEvent>

class MyLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit MyLineEdit(QWidget *parent = nullptr);
    void mouseDoubleClickEvent(QMouseEvent *);
    void leaveEvent(QEvent *event);
    void keyPressEvent(QKeyEvent *evt);
signals:

};

#endif // MYLINEEDIT_H
