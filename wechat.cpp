#include "wechat.h"
#include "ui_wechat.h"

wechat::wechat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wechat)
{
    ui->setupUi(this);
    we_manager = new QNetworkAccessManager(this);
    we_qrcode_manager = new QNetworkAccessManager(this);
    //QSslSocket::addDefaultCaCertificates("cert.pem");
    connect(we_qrcode_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(qrcode_reply(QNetworkReply*)));
    connect(we_manager,SIGNAL(finished(QNetworkReply*)), this, SLOT(reply_net(QNetworkReply*)));
}

wechat::~wechat()
{
    delete ui;
}

void wechat::qrcode_reply(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError){
        QByteArray all = reply->readAll();
        QPixmap pix;
        pix.loadFromData(all);
        pix = pix.scaled(100, 100, Qt::KeepAspectRatio);
        ui->qrcode->setPixmap(pix);
        ui->qrcode->show();
    }
    else{
        qDebug()<<reply->error();
    }
}

void wechat::reply_net(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError){
        QString all = reply->readAll();
        QRegExp rx("\"(.*)\"");
        int pos = all.indexOf(rx);
        if(pos > 0){
            QString str = rx.cap(1);
            str = "http://login.weixin.qq.com/qrcode/" + str;
            qDebug()<<str;
            we_qrcode_manager->get(QNetworkRequest(str));
        }

    }
    else{
        qDebug()<<reply->error();
    }
}

void wechat::on_wechat_login_released()
{
    QString url = "http://login.weixin.qq.com/jslogin?appid=wx782c26e4c19acffb&redirect_uri=https%3A%2F%2Fwx.qq.com%2Fcgi-bin%2Fmmwebwx-bin%2Fwebwxnewloginpage&fun=new&lang=en_US&_=1453725386008";
    we_manager->get(QNetworkRequest(url));
    //QByteArray postdata;
    //QNetworkReply* reply = we_manager->post(QNetworkRequest(url), postdata);
}
