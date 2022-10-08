#include "mylineedit.h"


MyLineEdit::MyLineEdit(QWidget *parent)
    : QLineEdit{parent}
{
    this->setFocusPolicy(Qt::StrongFocus);
}

void MyLineEdit::mouseDoubleClickEvent(QMouseEvent *)
{
    this->setReadOnly(false);

    this->selectAll();
}

void MyLineEdit::leaveEvent(QEvent *event)
{
    this->setSelection(this->text().length(),0);
}

void MyLineEdit::keyPressEvent(QKeyEvent *evt)
{

    if(evt->key()==16777220){//enter？

        this->setReadOnly(true);
    }
    QLineEdit::keyPressEvent(evt);
}



