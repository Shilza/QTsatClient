#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QListWidget>
#include <QKeyEvent>
#include <QLabel>
#include <QScrollBar>
#include <QPlainTextEdit>
#include <QFontMetrics>
#include <QClipboard>
#include <QApplication>
#include <QQueue>
#include <clickablelabel.h>
#include "distance_damerau_levenshtein.h"
#include "udpclient.h"

class GlobalTextEdit : public QTextEdit{
    Q_OBJECT
public:
    explicit GlobalTextEdit(QWidget *parent=0);
private:
    void keyPressEvent(QKeyEvent *e);
signals:
    void enter();
};

class PrivateTextEdit : public QTextEdit{
    Q_OBJECT
public:
    explicit PrivateTextEdit(QWidget *parent=0);
private:
    void keyPressEvent(QKeyEvent *e);
signals:
    void enter();
};

class WrapLabel : public QLabel{
    Q_OBJECT
public:
    explicit WrapLabel(QWidget* parent=0);
    ~WrapLabel();
private:
    void keyPressEvent(QKeyEvent *event);
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    UDPClient *client;
    QWidget *mainWidget;
    QGridLayout *mainLayout;
    QListWidget *listOfGlobalMessages;
    QPushButton *buttonSend;
    QPushButton *buttonPrivateMessages;
    QPushButton *buttonFriends;
    GlobalTextEdit *textMessage;
    ClickableLabel *labelFloodError;

    QQueue<QString> lastMessages;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void sendMessage();
    void printMessages();
public slots:
    void start(QByteArray sessionKey);
};


#endif // MAINWINDOW_H
