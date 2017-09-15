#ifndef WECHAT_H
#define WECHAT_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>

namespace Ui {
class wechat;
}

class wechat : public QWidget
{
    Q_OBJECT

public:
    explicit wechat(QWidget *parent = 0);
    QString we_code;
    ~wechat();
private slots:
    void reply_net(QNetworkReply*);
    void qrcode_reply(QNetworkReply*);
    void poll_reply(QNetworkReply*);
    void login_reply(QNetworkReply*);
    void init_reply(QNetworkReply*);
    void on_wechat_login_released();

private:
    QString wxuin;
    QString wxsid;
    QNetworkAccessManager* we_manager;
    QNetworkAccessManager* we_qrcode_manager;
    QNetworkAccessManager* we_poll_manager;
    QNetworkAccessManager* we_login_manager;
    QNetworkAccessManager* we_init_manager;
    Ui::wechat *ui;
};

#endif // WECHAT_H
