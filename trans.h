#ifndef TRANS_H
#define TRANS_H
#include <QPainter>
#include <QPixmap>
#include <QImage>
#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QMouseEvent>
#include <QtCore>
#include <QRegExp>
#include <QCryptographicHash>

namespace Ui {
class trans;
}

class trans : public QMainWindow
{
    Q_OBJECT

public:
    explicit trans(QWidget *parent = 0);
    ~trans();

private slots:

    void on_close_trans_clicked();

    void on_translate_clicked();

    void Reply_net(QNetworkReply*);

private:
    QPoint last;
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    QNetworkAccessManager *manager;
    Ui::trans *ui;
    void paintEvent(QPaintEvent *event);
};

#endif // TRANS_H
