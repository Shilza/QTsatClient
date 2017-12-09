#ifndef AUTHLINEEDIT_H
#define AUTHLINEEDIT_H
#include <QApplication>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QLineEdit>

class AuthLineEdit : public QLineEdit{
    Q_OBJECT

public:
    AuthLineEdit(QWidget *parent=0, bool isDefault = true);
    void setDisabledOverride();
    void setEnabledOverride();
private:
    int defaultFontSize;
    void keyPressEvent(QKeyEvent *event);
private slots:
    void showMenu(QPoint){}

public slots:
    void setDefaultStyleSheet();
    void setErrorStyleSheet();
};

#endif // AUTHLINEEDIT_H
