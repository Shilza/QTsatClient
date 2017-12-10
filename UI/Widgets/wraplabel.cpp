#include "wraplabel.h"

WrapLabel::WrapLabel(QWidget *parent): QLabel(parent){}

//Crazy bicycle, please don't touch it, it's dangerous...
void WrapLabel::wrapText(QString text){
    QFontMetrics *tempFontSize = new QFontMetrics(font());
    for(int i=text.indexOf("  ");i!=-1;i=text.indexOf("  "))
        text.remove(i,1);
    QString final = text;

    int tempCountOfPixels = 0;
    bool isSpace = false;

    if(tempFontSize->width(text)>445){
        final="";
        for(int i=0;i<text.length();i++){
            tempCountOfPixels+=tempFontSize->width(text[i]);
            isSpace = false;
            if(text[i]=="\n")
                tempCountOfPixels=0;
            if(tempCountOfPixels>=445){
                for(int j=i;j>final.length();j--){
                    if(text[j]==" "){
                        i=j;
                        isSpace=true;
                        final = text.mid(0,j);
                        break;
                    }
                }
                if(!isSpace){
                    final+="  ";
                    final+=text[i];
                }
                tempCountOfPixels=0;
            }
            else if(!isSpace)
                final+=text[i];
        }
    }

    setText(final);
}

void WrapLabel::keyPressEvent(QKeyEvent *event){
    QLabel::keyPressEvent(event);
    if(event->matches(QKeySequence::Copy) || event->matches(QKeySequence::Cut)){
        QString tempText = QApplication::clipboard()->text();
        for(int i=tempText.indexOf("  ");i!=-1;i=tempText.indexOf("  "))
            tempText.remove(i,2);
        QApplication::clipboard()->setText(tempText);
    }
}

WrapLabel::~WrapLabel(){}
