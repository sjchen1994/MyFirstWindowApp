#include "widget.h"
#include "ui_widget.h"



Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->recycle_clear_show->clear();
    ui->pushButton->setVisible(false);
    this->setAcceptDrops(true);

    //设置背景
    this->setAutoFillBackground(true);
    QPalette pal;
    pal.setBrush(QPalette::Background, QBrush(QPixmap("://image/main.png")));
    this->setPalette(pal);

    isclosed = 0;
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::Tool);
    ui->calc->setIcon(QIcon(":/image/calc.png"));
    ui->plantask->setIcon(QIcon(":/image/plan.png"));
    ui->transButton->setIcon(QIcon(":/image/fanyi.jpg"));
    ui->memButton->setIcon(QIcon(":/image/mem.jpg"));
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
    init_oper();
}

void Widget::init_oper(){
    int higher30 = 0;
    QDateTime dt = QDateTime::currentDateTime();
    QFile file("time.txt");
    if(file.open(QIODevice::ReadOnly)){
        QTextStream stream(&file);
        int year = stream.readLine().toInt();
        int month = stream.readLine().toInt();
        int day = stream.readLine().toInt();
        QDate pre(year, month, day);

        if(pre.daysTo(dt.date()) > 3){
            //清空回收站
            higher30 = 1;
            /*ui->pushButton->setVisible(true);

            QTime current_time = QTime::currentTime();
            QString show_label;
            while(true){
                show_label = "即将清空回收站，还有";

                show_label += (QString::number(15 - current_time.secsTo(QTime::currentTime())) + "...  如若取消，请点击" );
                ui->recycle_clear_show->setText(show_label);
                if(current_time.secsTo(QTime::currentTime()) == 16){
                    break;
                }
            }

            if(this->delete_ornot == true){
                SHEmptyRecycleBin(NULL, NULL, SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI |SHERB_NOSOUND);
                ui->recycle_clear_show->clear();
            }*/
            QMessageBox::StandardButton rb = QMessageBox::question(NULL, QString::fromLocal8Bit("警告！"), QString::fromLocal8Bit("即将清空回收站，是否确定？"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            if(rb == QMessageBox::Yes){
                SHEmptyRecycleBin(NULL, NULL, SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI |SHERB_NOSOUND);
            }
        }
        file.close();
    }
    else{
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream stream(&file);
            stream<<dt.date().year()<<endl<<dt.date().month()<<endl<<dt.date().day();
        }
        file.close();
    }
    if(higher30 == 1){
        higher30 = 0;
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream stream(&file);
            stream<<dt.date().year()<<endl<<dt.date().month()<<endl<<dt.date().day();
        }
        file.close();
    }



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

void Widget::dragEnterEvent(QDragEnterEvent *e){
    e->acceptProposedAction();
}
void Widget::dragMoveEvent(QDragMoveEvent *e){

}

void Widget::dropEvent(QDropEvent *e){
    QList<QUrl> urls = e->mimeData()->urls();
    if(urls.empty()){
        return;
    }
    QMessageBox::StandardButton rb = QMessageBox::question(NULL, QString::fromLocal8Bit("警告！"), QString::fromLocal8Bit("确定要删除选定文件吗？"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes){
        for(QUrl url: urls){
            QString file_name = url.toLocalFile();
            QFileInfo file_info(file_name);
            if(file_info.isFile()){//如果选中的有文件，就直接删除
                QFile::remove(file_name);
            }
            else if(file_info.isDir()){//是文件夹
                QDir dir(file_name);
                QFileInfo cur_file;
                QFileInfoList file_listtmp;
                int info_num;
                //获取文件夹下的所有文件（包括文件夹）
                QFileInfoList file_list = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::Readable | QDir::Writable | QDir::Hidden | QDir::NoDotAndDotDot, QDir::Name);
                while(file_list.size() > 0){
                    info_num = file_list.size();
                    //遍历，如果有文件就删除，有文件夹的话读取
                    for(int i = info_num -1; i >= 0; i--){
                        cur_file = file_list[i];

                        if(cur_file.isFile()){
                            QFile::remove(cur_file.filePath());
                            file_list.removeAt(i);
                            continue;
                        }

                        if(cur_file.isDir()){
                            QDir dirtmp(cur_file.filePath());
                            file_listtmp = dirtmp.entryInfoList(QDir::Dirs | QDir::Files | QDir::Readable | QDir::Writable | QDir::Hidden | QDir::NoDotAndDotDot, QDir::Name);

                            if(file_listtmp.size() == 0){//空文件夹 ，删除
                                dirtmp.rmdir(".");
                                file_list.removeAt(i);
                            }
                            else{//不是空文件夹，将目录加入待删除的列表中
                                for(int j = 0; j < file_listtmp.size(); j++){
                                    if(!(file_list.contains(file_listtmp[j]))){
                                        file_list.append(file_listtmp[j]);
                                    }
                                }
                            }
                        }
                    }
                }
                dir.rmdir(".");
            }
        }
    }
}

void Widget::on_pushButton_clicked()
{
    this->delete_ornot = false;
    ui->pushButton->setVisible(false);
}

void Widget::on_call_wechat_released()
{
    w = new wechat;
    w->show();
    w->setGeometry(this->x(), this->y()+400, 180, 350);
}
