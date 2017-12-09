#include "globaltextedit.h"

GlobalTextEdit::GlobalTextEdit(QWidget *parent) : QTextEdit(parent){
}

void GlobalTextEdit::keyPressEvent(QKeyEvent *e){
    bool previousSpace = false;

    if(this->toPlainText().length() > 0)
        previousSpace = this->toPlainText().at(this->toPlainText().length()-1) == " ";

    if((e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)){
        if(e->modifiers() != Qt::ControlModifier){
            emit enter();
            return;
        }
        else if(!previousSpace){
            if(this->toPlainText().indexOf('\n')==-1)
                QTextEdit::keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier));
            return;
        }
    }
    else if(e->key() == Qt::Key_Space && previousSpace)
        return;

    QTextEdit::keyPressEvent(e);
}
