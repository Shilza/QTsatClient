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
    QWidget *sendWidget;
    QWidget *affixWidget;
    QWidget *subAffixWidget;
    QHBoxLayout *affixLayout;
    QLabel *labelBicycle;
    QGridLayout *mainLayout;
    QListWidget *listOfGlobalMessages;
    QPushButton *buttonSend;
    QPushButton *buttonPrivateMessages;
    QPushButton *buttonFriends;
    QPushButton *buttonAffix;
    GlobalTextEdit *textMessage;
    ClickableLabel *labelFloodError;
    ClickableLabel *labelBan;
    QLabel *labelTimerShow;
    QLabel *labelSymbolsCount;
    QGridLayout *sendLayout;

    QPushButton *buttonPhotos;
    QPushButton *buttonVideos;
    QPushButton *buttonAudios;
    QPushButton *buttonDocuments;

    FloodTimer *floodTimer;

    QQueue<QString> lastMessages;
public:
    explicit MainWindow(QWidget *parent = 0);
     bool eventFilter(QObject *target, QEvent *event);
    ~MainWindow();
private slots:
    void sendMessage();
    void printMessages();
    void floodErrorHide();
    void updateTime();
public slots:
    void start(QByteArray sessionKey);
    void showSymbolsCount();
};

#endif // MAINWINDOW_H
