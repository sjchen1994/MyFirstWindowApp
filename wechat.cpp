#include "wechat.h"
#include "ui_wechat.h"

wechat::wechat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wechat)
{
    ui->setupUi(this);
    we_manager = new QNetworkAccessManager(this);
    connect(we_manager,SIGNAL(finished(QNetworkReply*)), this, SLOT(reply_net(QNetworkReply*)));
}

wechat::~wechat()
{
    delete ui;
}

void wechat::reply_net(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError){
        QString all = reply->readAll();
        qDebug()<<all;
    }
    else{
        qDebug()<<reply->error();
    }
}

void wechat::on_wechat_login_released()
{
    QString url = "https://login.weixin.qq.com/jslogin?appid=wx782c26e4c19acffb&redirect_uri=https%3A%2F%2Fwx.qq.com%2Fcgi-bin%2Fmmwebwx-bin%2Fwebwxnewloginpage&fun=new&lang=en_US&_=1453725386008";
    QString test_url = "http://api.fanyi.baidu.com/api/trans/vip/translate?q=1&from=zh&to=en&appid=20170809000071446&salt=41&sign=9ef6559e825dd2551bed729478562239";
    QString test1_url = "https://login.wx.qq.com/jslogin?appid=wx782c26e4c19acffb&redirect_uri=https%3A%2F%2Flogin.weixin.qq.com%2Fcgi-bin%2Fmmwebwx-bin%2Fwebwxnewloginpage&fun=new&lang=zh_CN&_=1505208345578";
    we_manager->get(QNetworkRequest(url));
    //QByteArray postdata;
    //QNetworkReply* reply = we_manager->post(QNetworkRequest(url), postdata);
}
