#include "authlineedit.h"

AuthLineEdit::AuthLineEdit(QWidget *parent, bool isDefault) : QLineEdit(parent){
    defaultFontSize = ((QApplication::desktop()->width()/100)*25/260)*11;
    setAcceptDrops(false);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setDefaultStyleSheet();
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), SLOT(showMenu(QPoint)));
    if(isDefault)
        connect(this, SIGNAL(textChanged(QString)), SLOT(setDefaultStyleSheet()));
}

void AuthLineEdit::setDefaultStyleSheet(){
    this->setStyleSheet(QString("AuthLineEdit{"
                                "font-family: Century Gothic;"
                                "font-size: %1px;"
                                "background: transparent;"
                                "border: 1px solid gray;"
                                "color: #B5EBEE;"
                                "}"
                                "AuthLineEdit:hover{"
                                "border: 1px solid black;"
                                "}"
                                "AuthLineEdit:focus{"
                                "border: 1px solid #0078d7;"
                                "}").arg(defaultFontSize));
}

void AuthLineEdit::setErrorStyleSheet(){
    this->setStyleSheet(QString("font-family: Century Gothic;"
                                "font-size: %1px;"
                                "background: transparent;"
                                "border: 1px solid red;"
                                "color: #B5EBEE;").arg(defaultFontSize));
}

void AuthLineEdit::keyPressEvent(QKeyEvent *event){
    if(event->matches(QKeySequence::Copy) || event->matches(QKeySequence::Cut) || event->matches(QKeySequence::Paste))
        event->ignore();
    else return QLineEdit::keyPressEvent(event);
}

void AuthLineEdit::setDisabledOverride(){
    setDisabled(true);
    setStyleSheet(QString("AuthLineEdit{"
                          "font-family: Century Gothic;"
                          "font-size: %1px;"
                          "background: transparent;"
                          "border: 1px solid #cccccc;"
                          "color: #B5EBEE;"
                          "}").arg(defaultFontSize));
}

void AuthLineEdit::setEnabledOverride(){
    setEnabled(true);
    setDefaultStyleSheet();
}
