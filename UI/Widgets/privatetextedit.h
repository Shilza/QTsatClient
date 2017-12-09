#ifndef PRIVATETEXTEDIT_H
#define PRIVATETEXTEDIT_H
#include <QTextEdit>
#include <QKeyEvent>

class PrivateTextEdit : public QTextEdit{
    Q_OBJECT
public:
    explicit PrivateTextEdit(QWidget *parent=0);
private:
    void keyPressEvent(QKeyEvent *e);
signals:
    void enter();
};

#endif // PRIVATETEXTEDIT_H
