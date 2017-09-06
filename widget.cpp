#include "widget.h"
#include "ui_widget.h"



Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);



    isclosed = 0;
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::Tool);

    //this->setWindowOpacity(0.8);

    //托盘图标设置
    system_tray->setToolTip("EASY");
    system_tray->setIcon(QIcon("trayicon.ico"));
    system_tray->setVisible(true);
    system_tray->show();
    QAction *mclose = new QAction(tray_menu);
    tray_menu->addAction(mclose);
    mclose->setText(QString("Quit"));
    system_tray->setContextMenu(tray_menu);
    connect(mclose,SIGNAL(triggered(bool)),this,SLOT(closew()));


    connect(ui->listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(goto_mem()));
    //()<<connect(m, SIGNAL(noCord()), this, SLOT(frameHide()));
    //数据库
    mem_database = initial_database();
    QSqlQuery tmp_query(mem_database);
    query = tmp_query;
    get_table();
}

void Widget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    //画背景
    QPainter painter(this);
    QPixmap pix;
    pix.load("main.png");
    painter.drawPixmap(0,0,350,500,pix);
}

QSqlDatabase Widget::initial_database(){
    QSqlDatabase tmp_database;
    if(QSqlDatabase::contains("qt_sql_default_connection")){
        tmp_database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else{
        tmp_database = QSqlDatabase::addDatabase("QSQLITE");
        tmp_database.setDatabaseName("men_database.db");
    }
    if(!tmp_database.open()){
        qDebug() << "Error: Failed to connect database." << tmp_database.lastError();
    }
    else{//数据连接成功
        QSqlQuery query(tmp_database);
        bool isTableExist;
        isTableExist = query.exec(QString("select remindornot from mem where remindornot='true'"));
        if(!isTableExist){
            //表不存在，创建表
            QString create_sql = "create table mem (id TEXT, remindornot bool, me TEXT, memtime TEXT)";
            query.prepare(create_sql);
            if(!query.exec()){
                qDebug()<<"Error:Fail to create talbe." << query.lastError();
            }
            else{
                qDebug()<<"Table created.";
            }
        }
        else{//表存在  还未写

        }
    }
    tmp_database.close();
    return tmp_database;
}

Widget::~Widget()
{
    delete ui;
}


void Widget::closew(){
    isclosed = 1;
    this->close();
}

int Widget::getclosed(){
    return isclosed;
}

void Widget::frameHide(){
    ui->listWidget->setVisible(false);
}

void Widget::reshow(){
    ui->listWidget->clear();
    get_table();
    this->show();
}

void Widget::get_table(){

    QString  get_sql = "select * from mem";
    if(!mem_database.open()){
        qDebug() << "Error: Failed to connect database." << mem_database.lastError();
    }
    else{
        query.prepare(get_sql);
        if(!query.exec()){
            qDebug()<< query.lastError();
        }
        else{
            while(query.next()){
                QString get_text = query.value(2).toString();
                ui->listWidget->addItem(get_text);
            }
        }
    }
    mem_database.close();
    if(ui->listWidget->count() == 0){
        ui->listWidget->setVisible(false);
    }
    else{
        ui->listWidget->setVisible(true);
    }
}



void Widget::goto_mem(){
    this->hide();
    m->show();
    m->setGeometry(this->x(),this->y(),180,350);
}

void Widget::on_memButton_clicked()//备忘录按钮
{
    this->hide();
    m = new mem;
    connect(m, SIGNAL(sendsignal()), this, SLOT(reshow()));
    m->show();
    m->setGeometry(this->x(),this->y(),180,350);
}

void Widget::on_transButton_clicked()//翻译按钮
{
    t = new trans;
    t->show();
    t->setGeometry(this->x(),this->y(),180,350);
}


void Widget::on_plantask_clicked()//定时关机按钮
{
    p = new plantask;
    p->show();
    p->setGeometry(this->x(),this->y(),180,350);
}

void Widget::on_calc_clicked()//打开计算器
{

    QProcess pro;
    pro.startDetached("C:\\Windows\\system32\\calc.exe",QStringList());
}

//移动窗口
void Widget::mousePressEvent(QMouseEvent *e)
{
    last = e->globalPos();
}
void Widget::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    move(x()+dx, y()+dy);
}
void Widget::mouseReleaseEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    move(x()+dx, y()+dy);
}
