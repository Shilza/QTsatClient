#ifndef SENDWIDGET_H
#define SENDWIDGET_H
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QQueue>
#include <QScrollBar>
#include <QFileDialog>
#include <QPropertyAnimation>
#include "Util/floodtimer.h"
#include "Util/distance_damerau_levenshtein.h"
#include "UI/Widgets/clickablelabel.h"
#include "UI/Widgets/globaltextedit.h"

class SendWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SendWidget(QWidget *parent=0);
    QWidget *getMainWidget();
    GlobalTextEdit *getTextMessage();
    ~SendWidget();
private:
    QWidget *mainWidget;
    QPushButton *affixWidgetContainer;
    QWidget *subAffixWidget;
    QHBoxLayout *affixLayout;
    QLabel *labelBicycle;
    QPushButton *buttonAffix;
    QPushButton *buttonSend;
    ClickableLabel *labelFloodError;
    QLabel *labelBan;
    QLabel *labelTimerShow;
    QLabel *labelSymbolsCount;
    QGridLayout *sendLayout;

    GlobalTextEdit *textMessage;

    QPushButton *buttonPhotos;
    QPushButton *buttonVideos;
    QPushButton *buttonAudios;
    QPushButton *buttonDocuments;

    FloodTimer *floodTimer;

    QQueue<QString> lastMessages;

    quint8 countOfAttachment=0;

    bool eventFilter(QObject *target, QEvent *event);
signals:
    void messageSended();
    void imageReceived(QPixmap);
private slots:
    void floodErrorHide();
    void updateTime();
    void showSymbolsCount();
    void send();
    void imageReceivedRedirect(QPixmap);
    void selectImage();
public slots:
    void decrementing();
};

#endif // SENDWIDGET_H
