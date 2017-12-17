#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H
#include <QLabel>
#include <QEvent>
#include <QPushButton>
#include "UI/Widgets/clickablelabel.h"
#include <QGridLayout>

class ImageView : public QWidget
{
    Q_OBJECT
public:
   explicit ImageView(QWidget *parent=0);
    ~ImageView();
private:
    QPushButton *picture;
    QPushButton *buttonClose;
    QGridLayout *mainLayout;
    ClickableLabel *background;
    bool eventFilter(QObject *target, QEvent *event);
public slots:
    void setPicture(QPixmap);
};

#endif // IMAGEVIEW_H
