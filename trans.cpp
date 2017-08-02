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
    qDebug()<< reply->error();
    if(reply->error() == QNetworkReply::NoError){
        QString all = reply->readAll();
        QString selected_all;
        auto it = all.begin();
        qDebug()<< all;
        while(*it != "\""){
            it++;
        }
        it++;
        while(*it != "\""){
            selected_all += *it;
            it++;
        }
        ui->textBrowser->setText(selected_all);
        reply->deleteLater();
    }

}

void trans::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);

    //画背景
    QPainter painter(this);
    QPixmap pix;
    pix.load("main.png");
    painter.drawPixmap(0,0,350,500,pix);
}
void trans::on_translate_clicked(){//提交翻译请求
    if(ui->textEdit->toPlainText() != ""){
        QString waittotrans = ui->textEdit->toPlainText();
        QByteArray byte = waittotrans.toUtf8();
        byte = byte.toPercentEncoding();
        QUrl lasturl;
        if(ui->comboBox->currentText() == "ch->en"){
             lasturl = "http://translate.google.cn/translate_a/single?client=t&sl=zh-CN&tl=en&hl=zh-CN&dt=at&dt=bd&dt=ex&dt=ld&dt=md&dt=qca&dt=rw&dt=rm&dt=ss&dt=t&ie=UTF-8&oe=UTF-8&source=btn&ssel=0&tsel=0&kc=0&tk=743696.853111&q=" + waittotrans;
        }
        else{
            lasturl = "https://link.zhihu.com/?target=http%3A//translate.google.cn/translate_a/single%3Fclient%3Dat%26sl%3Den%26tl%3Dzh-CN%26dt%3Dt%26q%3D" + waittotrans;
        }
        qDebug()<< lasturl;
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
