#ifndef ICONWIDGET_H
#define ICONWIDGET_H

#include <QWidget>

class IconWidget : public QWidget
{
    Q_OBJECT
public:
    explicit IconWidget(QWidget *parent = nullptr);
    void mouseDoubleClickEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void setPortrait(QString &path);
    QString getPortrait();
    QPixmap PixmapToRound(const QPixmap& src, int diameter/*直径*/);
private:
    QString filePath=":/head.png";
signals:

};

#endif // ICONWIDGET_H
