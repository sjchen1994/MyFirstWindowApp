#ifndef MEM_H
#define MEM_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <Qtsql/QSqlQuery>
#include <QCheckBox>
#include <QString>
#include <QTime>
#include <QPainter>
#include <QPixmap>
#include <QImage>
#include <QTableWidgetItem>
#include <QVector>
#include <QMouseEvent>
namespace Ui {
class mem;
}

class mem : public QMainWindow
{
    Q_OBJECT

public:
    explicit mem(QWidget *parent = 0);
    void add_table_widget();
    QSqlDatabase databaseinitial();
    QSqlDatabase mem_database;
    QSqlQuery query;
    void delete_table();
    void clear_table();
    void get_table();
    bool label;
    ~mem();
signals:
    void sendsignal();
private slots:
    void on_backtomain_clicked();

    void on_save_mem_clicked();

    void on_delete_row_clicked();

    void on_revise_mem_clicked();

    void double_click_table();

private:
    QPoint last;
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    QString tmp_id;
    void paintEvent(QPaintEvent *event);
    QStandardItemModel *mem_model;
    Ui::mem *ui;
};

#endif // MEM_H
