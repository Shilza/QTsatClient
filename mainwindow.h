#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QKeyEvent>
#include <QScrollBar>
#include <QLineEdit>
#include <QWidget>
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
    QLineEdit *log;
    QLineEdit *pass;
    QPushButton *signIn;
    QWidget *auth;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_sendButton_clicked();
    void printMessages();
    void signIn_clicked();
};


#endif // MAINWINDOW_H
