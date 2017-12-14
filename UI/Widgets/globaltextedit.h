#ifndef GLOBALTEXTEDIT_H
#define GLOBALTEXTEDIT_H
#include <QTextEdit>
#include <QKeyEvent>
#include <QApplication>
#include <QClipboard>
#include <QAbstractTextDocumentLayout>
#include <QMimeData>
#include <QImage>
#include "Config/def.h"

class GlobalTextEdit : public QTextEdit{
    Q_OBJECT
public:
    explicit GlobalTextEdit(QWidget *parent=0);
private:
    void keyPressEvent(QKeyEvent *e);
signals:
    void enter();
    void imageReceived(QImage);
private slots:
    void validator();
    void showMenu(QPoint){}
};

#endif // GLOBALTEXTEDIT_H
