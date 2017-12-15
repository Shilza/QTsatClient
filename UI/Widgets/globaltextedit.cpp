#include "globaltextedit.h"

GlobalTextEdit::GlobalTextEdit(QWidget *parent) : QTextEdit(parent){
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(textChanged()), SLOT(validator()));
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), SLOT(showMenu(QPoint)));
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

        const QMimeData* mime = QApplication::clipboard()->mimeData();
        if(mime->hasImage()){
            emit imageReceived(mime->imageData().value<QPixmap>());
        }

        quint8 maxSize = MAX_GLOBAL_MESSAGE_SIZE-toPlainText().length();

        if(toPlainText().indexOf('\n') == -1){
            if(tempText.indexOf('\n')!=-1)
                tempText = tempText.simplified().insert(tempText.indexOf('\n'), '\n');
            else
                tempText = tempText.simplified();
        }
        else if(toPlainText().indexOf('\n')!=-1)
            tempText = tempText.simplified();

        if(tempText.length()>maxSize)
            tempText = tempText.left(maxSize);

        QTextCursor cursor = this->textCursor();
        quint8 tempPos = cursor.position();
        setText(this->toPlainText().insert(cursor.position(), tempText));
        cursor.setPosition(tempPos+tempText.length());
        setTextCursor(cursor);
        return;
    }

    QTextEdit::keyPressEvent(event);
}

void GlobalTextEdit::validator()
{
    static QString lastTextState = this->toPlainText();
    if(toPlainText().length()>MAX_GLOBAL_MESSAGE_SIZE){
        QTextCursor cursor = this->textCursor();
        quint8 tempPos = cursor.position();
        setText(lastTextState);
        cursor.setPosition(tempPos-1);
        setTextCursor(cursor);
    }
    else
        lastTextState = this->toPlainText();
}
