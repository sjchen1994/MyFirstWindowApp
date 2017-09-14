#include "wechat.h"
#include "ui_wechat.h"

wechat::wechat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wechat)
{
    ui->setupUi(this);
    we_manager = new QNetworkAccessManager(this);
    we_qrcode_manager = new QNetworkAccessManager(this);
    we_poll_manager = new QNetworkAccessManager(this);
    we_login_manager = new QNetworkAccessManager(this);
    we_init_manager = new QNetworkAccessManager(this);
    //QSslSocket::addDefaultCaCertificates("cert.pem");

    connect(we_init_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(init_reply(QNetworkReply*)));
    connect(we_login_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(login_reply(QNetworkReply*)));
    connect(we_poll_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(poll_reply(QNetworkReply*)));
    connect(we_qrcode_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(qrcode_reply(QNetworkReply*)));
    connect(we_manager,SIGNAL(finished(QNetworkReply*)), this, SLOT(reply_net(QNetworkReply*)));
}

wechat::~wechat()
{
    delete ui;
}

void wechat::init_reply(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError){
        QString all = reply->readAll();
        qDebug()<<all;
    }
    else{
        qDebug()<<reply->error();
    }
}

void wechat::login_reply(QNetworkReply *reply){//登录
    if(reply->error() == QNetworkReply::NoError){
        QString all = reply->readAll();
        QRegExp rx1("<wxsid>(.*)</wxsid>");
        QRegExp rx2("<wxuin>(.*)</wxuin>");
        int pos1 = all.indexOf(rx1);
        int pos2 = all.indexOf(rx2);
        if(pos1 > 0 && pos2 > 0){
            wxsid = rx1.cap(1);
            wxuin = rx2.cap(1);
            QJsonObject post_json;
            //post_json.insert("BaseRequest", "");
            post_json.insert("Uin", wxuin);
            post_json.insert("Sid", wxsid);
            post_json.insert("Skey", "");
            post_json.insert("DeviceID", "e1615250492");
            QByteArray post_array = QJsonDocument(post_json).toJson();
            QDateTime current_time = QDateTime::currentDateTime();
            QString url = "http://wx.qq.com/cgi-bin/mmwebwx-bin/webwxinit?r=" + current_time.toMSecsSinceEpoch();
            QNetworkRequest *request = new QNetworkRequest;
            request->setUrl(QUrl(url));
            QNetworkReply* reply = we_init_manager->post(*request, post_array);
        }
    }
    else{
        qDebug()<<reply->error();
    }
}

void wechat::poll_reply(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError){//如果返回window.code = 201; 说明已经扫描二维码
        QString all = reply->readAll();
        if(all == "window.code=201;"){
            QDateTime current_time = QDateTime::currentDateTime();
            QString str = "http://login.weixin.qq.com/cgi-bin/mmwebwx-bin/login?uuid=" + we_code  + "&tip=1&_=" + QString::number(current_time.toMSecsSinceEpoch());
            we_poll_manager->get(QNetworkRequest(str));
        }
        else{
            qDebug()<<all;
            QRegExp rx("https://(.*)\"");
            int pos = all.indexOf(rx);
            if(pos > 0){
                QString str = rx.cap(1);
                we_login_manager->get(QNetworkRequest("http://" + rx.cap(1)));
            }

        }
    }
    else{
        qDebug()<<reply->error();
    }
}

void wechat::qrcode_reply(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError){
        QByteArray all = reply->readAll();
        QPixmap pix;
        pix.loadFromData(all);
        pix = pix.scaled(100, 100, Qt::KeepAspectRatio);
        ui->qrcode->setPixmap(pix);
        ui->qrcode->show();
        QDateTime current_time = QDateTime::currentDateTime();
        QString str = "http://login.weixin.qq.com/cgi-bin/mmwebwx-bin/login?uuid=" + we_code  + "&tip=1&_=" + QString::number(current_time.toMSecsSinceEpoch());
        we_poll_manager->get(QNetworkRequest(str));
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
            we_code = str;
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
    QDateTime current_time = QDateTime::currentDateTime();

    QString url = "http://login.weixin.qq.com/jslogin?appid=wx782c26e4c19acffb&redirect_uri=https%3A%2F%2Fwx.qq.com%2Fcgi-bin%2Fmmwebwx-bin%2Fwebwxnewloginpage&fun=new&lang=en_US&_=" + QString::number(current_time.toMSecsSinceEpoch());
    we_manager->get(QNetworkRequest(url));
    //QByteArray postdata;
    //QNetworkReply* reply = we_manager->post(QNetworkRequest(url), postdata);
}
