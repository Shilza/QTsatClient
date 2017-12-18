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
    void dropEvent(QDropEvent *e);
    void dragEnterEvent(QDragEnterEvent *e);
signals:
    void enter();
    void imageReceived(QPixmap);
private slots:
    void validator();
};

#endif // GLOBALTEXTEDIT_H
