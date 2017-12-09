#ifndef WRAPLABEL_H
#define WRAPLABEL_H
#include <QLabel>
#include <QKeyEvent>
#include <QApplication>
#include <QClipboard>

class WrapLabel : public QLabel{
    Q_OBJECT
public:
    explicit WrapLabel(QWidget* parent=0);
    void wrapText(QString text);
    ~WrapLabel();
private:
    void keyPressEvent(QKeyEvent *event);
};
#endif // WRAPLABEL_H
