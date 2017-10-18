#ifndef TEXTEDIT_H
#define TEXTEDIT_H
#include <QTextEdit>
#include <QKeyEvent>

class TextEdit : public QTextEdit{
    Q_OBJECT
public:
    explicit TextEdit(QWidget *parent=0);
private:
    void keyPressEvent(QKeyEvent *e);
signals:
    void enter();
};

#endif // TEXTEDIT_H
