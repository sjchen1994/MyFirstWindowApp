#ifndef PLANTASK_H
#define PLANTASK_H
#include <QPainter>
#include <QPixmap>
#include <QImage>
#include <QDialog>
#include <QDebug>
#include <QProcess>
#include <QMouseEvent>
namespace Ui {
class plantask;
}

class plantask : public QDialog
{
    Q_OBJECT

public:
    explicit plantask(QWidget *parent = 0);
    void select_mode();
    void edit_mode();
    void setplan(QDateTime set_time);
    void cancelplan();
    ~plantask();

private slots:
    void on_setplan_released();

    void on_planclose_released();

    void on_setplanpush_released();

    void on_cancelplan_released();

private:
    QPoint last;
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    bool havesetplan;
    void paintEvent(QPaintEvent *event);
    Ui::plantask *ui;
};

#endif // PLANTASK_H
