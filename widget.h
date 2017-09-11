#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QImage>
#include <QDebug>
#include <QDir>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <Qtsql/QSqlQuery>
#include <QProcess>
#include <QMessageBox>
#include <QMouseEvent>
#include <QTextEdit>
#include <windows.h>
#include <wechat.h>
#include "plantask.h"
#include "mem.h"
#include "trans.h"


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    QSqlDatabase initial_database();
    QSqlDatabase mem_database;
    QSqlQuery query;
    int isclosed;
    int getclosed();
    void get_table();
    wechat *w;
    mem *m;
    trans *t;
    plantask *p;
    void init_oper();
private slots:
    void frameHide();
    void reshow();
    void goto_mem();
    void closew();
    void on_memButton_clicked();
    void on_transButton_clicked();

    void on_plantask_clicked();

    void on_calc_clicked();

    void on_pushButton_clicked();

    void on_call_wechat_released();

signals:
    void sendsignal();



private:
    bool delete_ornot = true;
    QTextEdit *textedit;
    QPoint last;
    QMenu *tray_menu = new QMenu(this);
    QSystemTrayIcon *system_tray = new QSystemTrayIcon(this);
    Ui::Widget *ui;
protected:
    void dragEnterEvent(QDragEnterEvent *e);
    void dragMoveEvent(QDragMoveEvent *e);
    void dropEvent(QDropEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *event);
};

#endif // WIDGET_H
