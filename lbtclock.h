#ifndef LBTCLOCK_H
#define LBTCLOCK_H

#include <QtCore>
#include <QtGui>
#include <QWidget>
#include <QCloseEvent>
#include <QTimer>
#include <QPoint>
#include <QTime>
#include <QPainter>
#include <QPalette>
#include <QDebug>

class lbtClock : public QWidget
{
    Q_OBJECT

public:
    lbtClock(QWidget *parent = 0):
    QWidget(parent)
    {
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(update()));
        timer->start(1000);

        this->setWindowFlags(Qt::FramelessWindowHint);
        this->setWindowTitle(tr("Libratone Clock"));
        int side = qMin(width(), height());
        this->resize(QSize(side, side));
        this->setAttribute(Qt::WA_TranslucentBackground, true);
    }
    ~lbtClock(){
        delete timer;
    };
protected:
    void mousePressEvent(QMouseEvent *event){
        moving = true;
        position = event->globalPos() - pos();
        return QWidget::mouseMoveEvent(event);
    }

    void mouseMoveEvent(QMouseEvent *event){
        if(moving && (event->buttons() & Qt::LeftButton)){
            move(event->globalPos() - position);
            position = event->globalPos() - pos();
        }
        return QWidget::mouseMoveEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent *event){
        moving = false;
    }

    void paintEvent(QPaintEvent *){
        static const QPoint hourHand[3] = {
            QPoint(1, 1),
            QPoint(-1, 1),
            QPoint(0, -40)
        };
        static const QPoint minuteHand[3] = {
            QPoint(1, 1),
            QPoint(-1, 1),
            QPoint(0, -70)
        };

        QColor color(0, 0x71, 0x79);

        int side = qMin(width(), height());
        QTime time = QTime::currentTime();

        QPainter painter(this);
        QPen pen = painter.pen();
//        drawLogo(&painter)
        painter.setRenderHint(QPainter::Antialiasing);
        painter.translate(width() / 2, height() / 2);
        painter.scale(side / 200.0, side / 200.0);

        pen.setColor(color);
        pen.setWidth(0.3);
        painter.setPen(pen);
        painter.setBrush(Qt::white);
        painter.drawEllipse(-98, -98, 198, 198);

        pen.setStyle(Qt::SolidLine);
        pen.setWidth(1);
        painter.setPen(pen);
        painter.save();

        pen.setWidth(2);
        painter.setPen(pen);
        painter.rotate(30.0 * ((time.hour() + time.minute() / 60.0)) - 90.0);
        qDebug() << time;
//        painter.drawConvexPolygon(hourHand, 3);
        painter.drawLine(0, 0, 55.0, 0);
        painter.restore();

        for(int i = 0; i < 12; ++i){
            painter.drawLine(88, 0, 96, 0);
            painter.rotate(30.0);
        }

        pen.setWidth(0.5);
        painter.setPen(pen);
        painter.setBrush(color);
        painter.save();

        pen.setWidth(1);
        painter.setPen(pen);
        painter.rotate(6.0 * (time.minute() + time.second() / 60.0) - 90.0);
        painter.drawLine(0, 0, 75.0, 0);
        painter.restore();

        for(int j = 0; j < 60; ++j){
            if(j % 5 != 0)
                painter.drawLine(92, 0, 96, 0);
            painter.rotate(6.0);
        }

        painter.save();

        pen.setWidth(0.5);
        painter.setPen(pen);
        painter.rotate(6.0 * time.second() - 90.0);
        painter.drawLine(0, 0, 85.0, 0);
        painter.restore();
    }
    void keyPressEvent(QKeyEvent *event){
        if(event->key() == Qt::Key_Escape){
            this->close();
        }
    }

private:
    QTimer *timer;
    bool    moving;
    QPoint  position;
};

#endif // LBTCLOCK_H
