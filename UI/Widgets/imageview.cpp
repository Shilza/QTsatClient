#include "imageview.h"

ImageView::ImageView(QWidget *parent) : QWidget(parent){
    mainLayout = new QGridLayout(this);

    background = new ClickableLabel(this, false);

    picture = new QPushButton(this);
    buttonClose = new QPushButton(this);

    picture->setFixedSize(parent->width()*3/4, parent->height()*3/4);
    picture->setIconSize(picture->maximumSize());
    picture->setStyleSheet("background: rgba(0,0,0,180);"
                           "border: 1px solid black;");

    buttonClose->setFixedSize(40, 25);

    buttonClose->installEventFilter(this);
    buttonClose->setStyleSheet("QPushButton {"
                               "border: 0px;"
                               "background: transparent;"
                               "}");
    buttonClose->setIcon(QIcon(":/images/close1.png"));
    buttonClose->setIconSize(QSize(buttonClose->width(), buttonClose->height()-10));

    background->setStyleSheet("background: rgba(0,0,0,140);");

    mainLayout->setMargin(0);

    mainLayout->addWidget(background, 0,0,1,1);
    mainLayout->addWidget(picture, 0, 0, 1, 1, Qt::AlignHCenter);
    mainLayout->addWidget(buttonClose, 0, 0, 1, 1, Qt::AlignTop | Qt::AlignRight);

    setLayout(mainLayout);
    setContentsMargins(0, 0, 0, 0);
    setFixedSize(parent->size());
    close();

    connect(buttonClose, SIGNAL(released()), this, SLOT(close()));
    connect(background, SIGNAL(released()), this, SLOT(close()));
}

void ImageView::setPicture(QPixmap affixImage){
    picture->setIcon(QIcon(affixImage));
    background->setFixedSize(size());
    show();
}

bool ImageView::eventFilter(QObject *target, QEvent *event){
    if (target == buttonClose){
        if (event->type() == QEvent::HoverEnter)
            buttonClose->setIcon(QIcon(":images/close2.png"));
        else if(event->type() == QEvent::HoverLeave)
            buttonClose->setIcon(QIcon(":images/close1.png"));
    }

    return QWidget::eventFilter(target, event);
}

ImageView::~ImageView(){
    delete picture;
    delete buttonClose;
    delete mainLayout;
}
