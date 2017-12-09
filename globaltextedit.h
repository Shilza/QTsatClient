#ifndef GLOBALTEXTEDIT_H
#define GLOBALTEXTEDIT_H

#include <QTextEdit>
#include <QKeyEvent>
#include <QEvent>

class GlobalTextEdit : public QTextEdit{
    Q_OBJECT
public:
    explicit GlobalTextEdit(QWidget *parent=0);
private:
    void keyPressEvent(QKeyEvent *e);
signals:
    void enter();
};

#endif // GLOBALTEXTEDIT_H
