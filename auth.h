#ifndef AUTH_H
#define AUTH_H
#include <QMainWindow>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

namespace Ui {
class Auth;
}

class Auth : public QMainWindow
{
    Q_OBJECT

private:
    Ui::Auth *ui;

    QLineEdit *log;
    QLineEdit *pass;
    QPushButton *signIn;

public:
    explicit Auth(QWidget *parent = 0);
    ~Auth();

private slots:
    void signIn_clicked();
};


#endif // AUTH_H
