#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H
#include <QLabel>

class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickableLabel(QWidget* parent=0, bool isUnderlined=true);
    ~ClickableLabel();
signals:
    void released();
private:
    bool isUnderlined;
    void mouseReleaseEvent(QMouseEvent* event);
    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);
};

#endif // CLICKABLELABEL_H
