#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QScrollBar>
#include <QQueue>
#include <QPropertyAnimation>
#include <QStackedWidget>
#include <QPixmap>
#include "UI/Widgets/clickablelabel.h"
#include "UI/Widgets/globaltextedit.h"
#include "UI/Widgets/privatetextedit.h"
#include "UI/Widgets/wraplabel.h"
#include "Util/floodtimer.h"
#include "Util/distance_damerau_levenshtein.h"
#include "Util/udpclient.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    UDPClient *client;

    QStackedWidget *stackOfWidgets;
    QWidget *mainWidget;
    QHBoxLayout *mainLayout;
    QWidget *globalChatWidget;
    QGridLayout *globalChatLayout;
    QWidget *menuListWidget;
    QVBoxLayout *menuListLayout;

    QWidget *sendWidget;
    QPushButton *affixWidget;
    QWidget *subAffixWidget;
    QHBoxLayout *affixLayout;
    QLabel *labelBicycle;
    QPushButton *buttonAffix;
    GlobalTextEdit *textMessage;
    QPushButton *buttonSend;
    ClickableLabel *labelFloodError;
    QLabel *labelBan;
    QLabel *labelTimerShow;
    QLabel *labelSymbolsCount;
    QGridLayout *sendLayout;

    QPushButton *buttonPhotos;
    QPushButton *buttonVideos;
    QPushButton *buttonAudios;
    QPushButton *buttonDocuments;

    QPushButton *sendedImage;
    QPushButton *buttonCloseAffixedPicture;
    QLabel *toolTipAffixClose;
    QLabel *originalSize;
    QLabel *picture;
    QPixmap affixImage;

    FloodTimer *floodTimer;

    QListWidget *listOfGlobalMessages;

    QPushButton *buttonUserPage;
    QPushButton *buttonPrivateMessages;
    QPushButton *buttonFriends;

    QQueue<QString> lastMessages;
    void initSendWidget();
public:
    explicit MainWindow(QWidget *parent = 0);
     bool eventFilter(QObject *target, QEvent *event);
    ~MainWindow();
private slots:
    void sendMessage();
    void printMessages();
    void floodErrorHide();
    void updateTime();
    void showSymbolsCount();
    void receivedImageTreatment(QPixmap);
    void buttonCloseAffixedPicture_released();
    void showAffixedPicture();
public slots:
    void start(QByteArray sessionKey);
};

#endif // MAINWINDOW_H
