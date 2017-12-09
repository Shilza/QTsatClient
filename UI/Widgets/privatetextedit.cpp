#include "privatetextedit.h"

PrivateTextEdit::PrivateTextEdit(QWidget *parent) : QTextEdit(parent){}

void PrivateTextEdit::keyPressEvent(QKeyEvent *e)
{
    bool previousSpace = false;
    bool previousEndl = false;
    bool prePreviousEndl = false;

    if(this->toPlainText().length() > 0){
        previousSpace = this->toPlainText().at(this->toPlainText().length()-1) == " ";
        previousEndl = this->toPlainText().at(this->toPlainText().length()-1) == "\n";
    }

    if(this->toPlainText().length() > 1)
        prePreviousEndl = this->toPlainText().at(this->toPlainText().length()-2) == "\n";

    if((e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)){
        if(e->modifiers() != Qt::ControlModifier){
            emit enter();
            return;
        }
        else if((!previousSpace && !previousEndl) || !prePreviousEndl){
            QTextEdit::keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter,Qt::NoModifier));
            return;
        }
    }
    else if(e->key() == Qt::Key_Space && (previousSpace || previousEndl))
        return;

    QTextEdit::keyPressEvent(e);
}
