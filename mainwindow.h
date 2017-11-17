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
#include "udpclient.h"

namespace Ui {
class MainWindow;
}

class TextEdit : public QTextEdit{
    Q_OBJECT
public:
    explicit TextEdit(QWidget *parent=0);
private:
    void keyPressEvent(QKeyEvent *e);
signals:
    void enter();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    UDPClient *client;
    QWidget *mainWidget;
    QGridLayout *mainLayout;
    QListWidget *listOfGlobalMessages;
    QPushButton *buttonSend;
    QTextEdit *textMessage;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_sendButton_clicked();
    void printMessages();
public slots:
    void start(QByteArray sessionKey);
};


#endif // MAINWINDOW_H
