#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QKeyEvent>
#include <QScrollBar>
#include "udpclient.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    MyUDP *client;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_sendButton_clicked();
    void printMessages();
    void start(QByteArray sessionKey);
};


#endif // MAINWINDOW_H
