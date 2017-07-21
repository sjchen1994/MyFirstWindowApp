#include "plantask.h"
#include "ui_plantask.h"

plantask::plantask(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::plantask)
{
    ui->setupUi(this);
    havesetplan = false;
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::Tool);
    select_mode();
    QDateTime mindatetime = QDateTime::currentDateTime();
    ui->dateTimeEdit->setMinimumDateTime(mindatetime);

    ui->comboBox->addItem("关机");
    ui->comboBox->addItem("重启");
}

plantask::~plantask()
{
    delete ui;
}

void plantask::select_mode(){
    if(havesetplan == true){
        ui->setplan->setEnabled(false);
    }
    else{
        ui->setplan->setEnabled(true);
    }
    ui->setplan->setVisible(true);
    ui->cancelplan->setVisible(true);
    ui->planclose->setVisible(true);
    ui->dateTimeEdit->setVisible(false);
    ui->comboBox->setVisible(false);
    ui->setplanpush->setVisible(false);
}

void plantask::edit_mode(){
    ui->setplan->setVisible(false);
    ui->cancelplan->setVisible(false);
    //ui->planclose->setVisible(false);
    ui->comboBox->setVisible(true);
    ui->dateTimeEdit->setVisible(true);
    ui->setplanpush->setVisible(true);
}

void plantask::setplan(QDateTime set_time){
    QDateTime now = QDateTime::currentDateTime();
    QString secto = QString::number(now.secsTo(set_time),10);
    QString comm;
    if(ui->comboBox->currentText() == "关机"){
        comm = "shutdown -s -t " + secto;
    }
    else{
        comm = "shutdown -r -t " + secto;
    }

    QProcess p;
    p.start("C:\\Windows\\system32\\cmd.exe", QStringList()<<"/c"<< comm);
    p.waitForStarted();
    p.waitForFinished();
}

void plantask::on_setplan_released()
{
    edit_mode();
}

void plantask::on_planclose_released()
{
    this->hide();
    select_mode();
}

void plantask::on_setplanpush_released()
{
    QDateTime tmp_datetime = ui->dateTimeEdit->dateTime();
    setplan(tmp_datetime);
    havesetplan = true;
    select_mode();
}

void plantask::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);

    //画背景
    QPainter painter(this);
    QPixmap pix;
    pix.load("main.png");
    painter.drawPixmap(0,0,350,500,pix);
}
void plantask::on_cancelplan_released()
{
    ui->setplan->setEnabled(true);
    havesetplan = false;
    QProcess p;
    p.start("C:\\Windows\\system32\\cmd.exe", QStringList()<<"/c"<< "shutdown -a");
    p.waitForStarted();
    p.waitForFinished();
}

//移动窗口
void plantask::mousePressEvent(QMouseEvent *e)
{
    last = e->globalPos();
}
void plantask::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    move(x()+dx, y()+dy);
}
void plantask::mouseReleaseEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    move(x()+dx, y()+dy);
}
