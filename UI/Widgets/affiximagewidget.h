#ifndef AFFIXIMAGEWIDGET_H
#define AFFIXIMAGEWIDGET_H
#include <QPushButton>
#include <QEvent>
#include <QHBoxLayout>

class AffixImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AffixImageWidget(QWidget *parent=0);
    QWidget *getSendedImage();
    ~AffixImageWidget();
private:
    QWidget *mainWidget;
    QHBoxLayout *mainAffixLayout;
    QPushButton *sendedImage;
    QPushButton *buttonCloseAffixedPicture;
    QPushButton *toolTipAffixClose;
    QPushButton *originalSize;
    QPixmap affixImage;

    quint8 sendedImageSize = 60;
    quint8 closeButtonSize = 12;

    bool eventFilter(QObject *target, QEvent *event);
signals:
    void originalSizeReleased(QPixmap);
    void detachmentImage();
public slots:
    void receivedImageTreatment(QPixmap);
private slots:
    void originalSize_released();
    void buttonCloseAffixedPicture_released();
};

#endif // AFFIXIMAGEWIDGET_H
