#include "iconwidget.h"
#include <QFileDialog>
#include <QPainter>
#include <QPixmap>
IconWidget::IconWidget(QWidget *parent)
    : QWidget{parent}
{

}

void IconWidget::mouseDoubleClickEvent(QMouseEvent *)
{
     filePath= QFileDialog::getOpenFileName(this,"选择头像","C:\\","Image Files (*.png *.jpg *jpeg)");
     update();
}

void IconWidget::paintEvent(QPaintEvent *)
{
   QPainter painter(this);
   QPixmap pixmap;
   pixmap.load(filePath);
   pixmap = pixmap.scaled(100,100,Qt::KeepAspectRatio,Qt::SmoothTransformation);
   pixmap=PixmapToRound(pixmap,100);
   int x=(this->width()-100)/2;
   painter.drawPixmap(x,30,pixmap);
}

void IconWidget::setPortrait(QString &path)
{
    filePath=path;
    update();
}

QString IconWidget::getPortrait()
{
    return filePath;
}
//将图片设置为圆形
QPixmap IconWidget::PixmapToRound(const QPixmap& src, int diameter/*直径*/)
{
    if (src.isNull())
    {
        return QPixmap();
    }
    QSize size(diameter, diameter);
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::LosslessImageRendering);
    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(0, 0, size.width(), size.height(), 99, 99);
    QPixmap image = src.scaled(size);
    image.setMask(mask);
    return image;
}
