#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QScrollBar>
#include <QStackedWidget>
#include <QPixmap>
#include "UI/Widgets/sendwidget.h"
#include "UI/Widgets/affiximagewidget.h"
#include "UI/Widgets/clickablelabel.h"
#include "UI/Widgets/globaltextedit.h"
#include "UI/Widgets/privatetextedit.h"
#include "UI/Widgets/wraplabel.h"
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

    SendWidget *sendWidget;
    AffixImageWidget *affixImageWidget;
    QLabel *picture;

    QListWidget *listOfGlobalMessages;

    QPushButton *buttonUserPage;
    QPushButton *buttonPrivateMessages;
    QPushButton *buttonFriends;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void sendMessage();
    void printMessages();
    void showAffixedPicture(QPixmap);
public slots:
    void start(QByteArray sessionKey);
};

#endif // MAINWINDOW_H
