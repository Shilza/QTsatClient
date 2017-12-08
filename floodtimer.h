#ifndef FLOODTIMER_H
#define FLOODTIMER_H
#include <QWidget>
#include <QTimer>

class FloodTimer : public QObject{
    Q_OBJECT
public:
    FloodTimer(QWidget *parent=0);
    int remainingTime();
    void start();
private:
    QTimer *timerErrorHide;
    QTimer *timerShow;
    quint8 counter;
signals:
    void errorTimeout();
    void showTimeout();
private slots:
    void emitErrorTimeout();
    void emitShowTimeout();
};

#endif // FLOODTIMER_H
