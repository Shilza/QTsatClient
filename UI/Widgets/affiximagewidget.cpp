#include "affiximagewidget.h"
#include <QDebug>
#include <QBuffer>

AffixImageWidget::AffixImageWidget(QWidget *parent) : QWidget(parent){
    mainWidget = new QWidget(parent);
    mainAffixLayout = new QHBoxLayout(mainWidget);
    mainAffixLayout->setMargin(5);
    mainWidget->setLayout(mainAffixLayout);

    sendedImage = new QPushButton(mainWidget);
    sendedImage->installEventFilter(this);

    toolTipAffixClose = new QPushButton(parent);

    originalSize = new QPushButton(sendedImage);

    buttonCloseAffixedPicture = new QPushButton(sendedImage);

    sendedImage->setFixedSize(sendedImageSize, sendedImageSize);
    sendedImage->setStyleSheet("border: 1px solid black;");
    sendedImage->close();

    originalSize->resize(sendedImage->width(), sendedImage->width());
    originalSize->setIcon(QIcon(":/images/originalSize.png"));
    originalSize->setIconSize(QSize(sendedImage->width(), sendedImage->width()));
    originalSize->setStyleSheet("background: transparent;"
                                "border: 0px;");
    originalSize->close();

    buttonCloseAffixedPicture->setGeometry(originalSize->width()-closeButtonSize-2, 2, closeButtonSize, closeButtonSize);
    buttonCloseAffixedPicture->setIcon(QIcon(":/images/affixClose.png"));
    buttonCloseAffixedPicture->setIconSize(QSize(closeButtonSize, closeButtonSize));
    buttonCloseAffixedPicture->setStyleSheet("background: white;"
                                             "border-radius: 6px;"
                                             "border: 0px;");
    buttonCloseAffixedPicture->installEventFilter(this);

    toolTipAffixClose->setStyleSheet("background: transparent;"
                                     "border: 0px;");
    toolTipAffixClose->setGeometry(0, 0, 100, 26);
    toolTipAffixClose->setIcon(QIcon(":/images/toolTipAffixClose.png"));
    toolTipAffixClose->setIconSize(QSize(100, 26));
    //toolTipAffixClose->setPixmap(QPixmap(":/images/toolTipAffixClose.png").scaled(toolTipAffixClose->width(), toolTipAffixClose->height()));
    toolTipAffixClose->close();

    connect(originalSize, SIGNAL(released()), this, SLOT(originalSize_released()));
    connect(buttonCloseAffixedPicture, SIGNAL(released()), this, SLOT(buttonCloseAffixedPicture_released()));
}

QWidget *AffixImageWidget::getSendedImage(){
    return mainWidget;
}

void AffixImageWidget::receivedImageTreatment(QPixmap image){
    affixImage = image;
/*
    QByteArray b;
    QBuffer buf(&b);
    buf.open(QIODevice::WriteOnly);
    image.save(&buf, "JPG");

    buf.close();

    affixImage.loadFromData(b);
*/
    if(image.height() > image.width()){
        int leftTop = image.height()/2-image.width()/2;

        QRect rect(0, leftTop, image.width(), image.width());
        image = image.copy(rect);
    }
    else if(image.height() < image.width()){
        int leftTop = image.width()/2-image.height()/2;

        QRect rect(leftTop, 0, image.height(), image.height());
        image = image.copy(rect);
    }

    if(image.width()<sendedImageSize-2)
        image = image.scaled(sendedImageSize-2,sendedImageSize-2);

    sendedImage->setIcon(QIcon(image));
    sendedImage->setIconSize(QSize(sendedImageSize-2,sendedImageSize-2));
    mainAffixLayout->addWidget(sendedImage, 1, Qt::AlignLeft | Qt::AlignBottom);
    sendedImage->show();
}

void AffixImageWidget::buttonCloseAffixedPicture_released(){
    emit detachmentImage();
    sendedImage->close();
}

AffixImageWidget::~AffixImageWidget(){
    delete sendedImage;
    delete buttonCloseAffixedPicture;
    delete toolTipAffixClose;
    delete originalSize;
}

bool AffixImageWidget::eventFilter(QObject *target, QEvent *event)
{
    if(target == buttonCloseAffixedPicture){
        if(event->type() == QEvent::HoverEnter){
            QPoint point = parentWidget()->mapFromGlobal(QCursor::pos());
            toolTipAffixClose->move(point.x()-20, point.y()-toolTipAffixClose->height()-toolTipAffixClose->height()/5);
            toolTipAffixClose->show();
        }
        else if(event->type() == QEvent::HoverLeave)
            toolTipAffixClose->close();
    }
    else if(target==sendedImage){
        if(event->type() == QEvent::HoverEnter)
            originalSize->show();
        else if(event->type() == QEvent::HoverLeave)
            originalSize->close();
    }

    return QObject::eventFilter(target, event);
}

void AffixImageWidget::originalSize_released(){
    emit originalSizeReleased(affixImage);
}
