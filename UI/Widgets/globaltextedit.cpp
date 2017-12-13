#include "globaltextedit.h"

GlobalTextEdit::GlobalTextEdit(QWidget *parent) : QTextEdit(parent){
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(document()->documentLayout(), SIGNAL(documentSizeChanged(QSizeF)), this, SLOT(textEditSizeChange(QSizeF)));
    connect(this, SIGNAL(textChanged()), SLOT(validator()));
}

void GlobalTextEdit::keyPressEvent(QKeyEvent *event){
    bool previousSpace = false;

    if(this->toPlainText().length() > 0)
        previousSpace = this->toPlainText().at(this->toPlainText().length()-1) == " ";

    if((event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)){
        if(event->modifiers() != Qt::ControlModifier){
            emit enter();
            return;
        }
        else if(!previousSpace){
            if(this->toPlainText().indexOf('\n')==-1)
                QTextEdit::keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier));
            return;
        }
    }
    else if(event->key() == Qt::Key_Space && previousSpace)
        return;
    else if(event->matches(QKeySequence::Paste)){
        QString tempText = QApplication::clipboard()->text();
        quint8 maxSize = MAX_GLOBAL_MESSAGE_SIZE-this->toPlainText().length();
        if(this->toPlainText().indexOf('\n')!=-1)
            tempText = tempText.simplified();

        if(tempText.length()>maxSize)
            tempText = tempText.left(maxSize);

        QTextCursor cursor = this->textCursor();
        quint8 tempPos = cursor.position();
        this->setText(this->toPlainText().insert(cursor.position(), tempText));
        cursor.setPosition(tempPos+tempText.length());
        this->setTextCursor(cursor);
        return;
    }

    QTextEdit::keyPressEvent(event);
}

void GlobalTextEdit::textEditSizeChange(QSizeF changedSize)
{
    static QSizeF lastState=QSizeF(0.0,0.0);
    if(lastState.height()!=changedSize.height()){
        this->setFixedHeight(changedSize.height()>47 ? (changedSize.height()<138 ? changedSize.height()+1 : 139) : 48);
    }
    lastState=changedSize;
}

void GlobalTextEdit::validator()
{
    static QString lastTextState = this->toPlainText();
    if(this->toPlainText().length()>MAX_GLOBAL_MESSAGE_SIZE)
        this->setText(lastTextState);
    else
        lastTextState = this->toPlainText();
}
