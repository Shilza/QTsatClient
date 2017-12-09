#include "clickablelabel.h"

ClickableLabel::ClickableLabel(QWidget* parent, bool isUnderlined) : QLabel(parent){
    this->isUnderlined = isUnderlined;
}

void ClickableLabel::enterEvent(QEvent*){
    if(isUnderlined){
        QFont f = font();
        f.setUnderline(true);
        setFont(f);
    }
}

void ClickableLabel::leaveEvent(QEvent*){
    if(isUnderlined){
        QFont f = font();
        f.setUnderline(false);
        setFont(f);
    }
}

void ClickableLabel::mouseReleaseEvent(QMouseEvent *){
    emit released();
}

ClickableLabel::~ClickableLabel(){}
