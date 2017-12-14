#ifndef PRIVATETEXTEDIT_H
#define PRIVATETEXTEDIT_H
#include <QTextEdit>
#include <QAbstractTextDocumentLayout>
#include <QKeyEvent>

class PrivateTextEdit : public QTextEdit{
    Q_OBJECT
public:
    explicit PrivateTextEdit(QWidget *parent=0);
private:
    void keyPressEvent(QKeyEvent *e);
signals:
    void enter();
private slots:
    void textEditSizeChange(QSizeF changedSize);
};

#endif // PRIVATETEXTEDIT_H
