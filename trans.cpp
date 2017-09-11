#include "trans.h"
#include "ui_trans.h"

trans::trans(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::trans)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::Tool);
    manager = new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(Reply_net(QNetworkReply*)));
    ui->comboBox->addItem("ch->en");
    ui->comboBox->addItem("en->ch");
}

trans::~trans()
{
    delete ui;
}


void trans::on_close_trans_clicked()
{
    this->close();
}

void trans::Reply_net(QNetworkReply *reply){ //读取网页信息
    if(reply->error() == QNetworkReply::NoError){
        QString all = reply->readAll();
        QRegExp rx("dst\"\:\"(.*)\"");
        int pos = all.indexOf(rx);
        if(pos >= 0){
            QString str = rx.cap(1);
            while(true){
                if(ui->comboBox->currentText() == "en->ch"){
                    int idx = str.indexOf("\\u");
                    if(idx == -1){
                        break;
                    }
                    QString strhex = str.mid(idx, 6);
                    strhex = strhex.replace("\\u", QString());
                    int nhex = strhex.toInt(0, 16);
                    str.replace(idx, 6, QChar(nhex));
                    ui->textBrowser->setText(str);
                }
                else{
                    ui->textBrowser->setText(str);
                    break;
                }

            }
        }
        reply->deleteLater();
    }

}

void trans::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);

    //画背景
    QPainter painter(this);
    QPixmap pix;
    pix.load("main.png");
    painter.drawPixmap(0, 0, 350, 500, pix);
}

void trans::on_translate_clicked(){//提交翻译请求
    if(ui->textEdit->toPlainText() != ""){
        QString wait_to_trans = ui->textEdit->toPlainText();

        QByteArray query =  wait_to_trans.toUtf8();
        qDebug()<< query;
        QString lasturl = "http://api.fanyi.baidu.com/api/trans/vip/translate?";
        QString appid = "20170809000071446";

        QString from_to = ui->comboBox->currentText();
        QString from;
        if(from_to[0] == 'c'){
            from.append("z");
        }
        else{
            from.append(from_to[0]);
        }
        from.append(from_to[1]);
        from.append("&");
        QString to;
        if(from_to[4] == 'c'){
            to.append("z");
        }
        else{
            to.append(from_to[4]);
        }
        to.append(from_to[5]);
        to.append("&");

        int randnum = qrand();
        //QByteArray salt = QByteArray::number(randnum);
        QString salt = QString::number(randnum);
        QString secret_key = "EHy_V503R6K1gOaEBaWa";
        QString bfmd5 = appid + query + salt + secret_key;
        //bfmd5 = bfmd5.toUtf8();
        //QByteArray bfmd5 = appid.toUtf8() + query + "41" + secret_key.toUtf8();

        QCryptographicHash md(QCryptographicHash::Md5);
        QByteArray ba;
        ba.append(bfmd5);
        md.addData(ba);

        //QByteArray bb = QCryptographicHash::hash(bfmd5_to_ascii, QCryptographicHash::Md5);
        QByteArray bb;
        bb = md.result();
        QString afmd5;
        afmd5.append(bb.toHex());

        appid = appid +"&";
        salt = salt + "&";
        query = query.toPercentEncoding();
        query = query + "&";
        lasturl = lasturl + "q=" + query + "from=" + from + "to=" + to + "appid=" + appid + "salt=" + salt + "sign=" + afmd5;
        qDebug()<<lasturl;
        manager->get(QNetworkRequest(lasturl));
    }
}



//移动窗口
void trans::mousePressEvent(QMouseEvent *e)
{
    last = e->globalPos();
}
void trans::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    move(x()+dx, y()+dy);
}
void trans::mouseReleaseEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    move(x()+dx, y()+dy);
}
