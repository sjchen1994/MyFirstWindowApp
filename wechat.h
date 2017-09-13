#ifndef WECHAT_H
#define WECHAT_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

namespace Ui {
class wechat;
}

class wechat : public QWidget
{
    Q_OBJECT

public:
    explicit wechat(QWidget *parent = 0);
    ~wechat();
private slots:
    void reply_net(QNetworkReply*);
    void qrcode_reply(QNetworkReply*);
    void on_wechat_login_released();

private:
    QNetworkAccessManager* we_manager;
    QNetworkAccessManager* we_qrcode_manager;
    Ui::wechat *ui;
};

#endif // WECHAT_H
