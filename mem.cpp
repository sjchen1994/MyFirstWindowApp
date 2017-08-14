#include "mem.h"
#include "ui_mem.h"


mem::mem(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mem)
{
    ui->setupUi(this);

    //初始化
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<""<<"content"<<"time"<<"id");
    ui->tableWidget->setColumnWidth(0,18);
    ui->tableWidget->setColumnWidth(1,150);
    ui->tableWidget->setColumnWidth(2,170);
    ui->tableWidget->setColumnWidth(3,150);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);
    this->setWindowFlags(Qt::FramelessWindowHint);
    //this->setWindowOpacity(0.8);
    tmp_id.clear();
    label = true;
    connect(ui->tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(double_click_table()));

    //数据库初始化
    mem_database = databaseinitial();
    QSqlQuery tmp_query(mem_database);
    query = tmp_query;
    get_table();
}

mem::~mem()
{

    delete ui;

}
void mem::get_table(){//从数据库中读取数据并绑定到tablewidget
    QString  get_sql = "select * from mem";
    int current_rowcount;
    if(!mem_database.open()){
        qDebug() << "Error: Failed to connect database." << mem_database.lastError();
    }
    else{
        query.prepare(get_sql);
        if(!query.exec()){
            qDebug()<< query.lastError();
        }
        else{
            if(!query.first()){
                emit this->noCord();
            }
            while(query.next()){
                current_rowcount = ui->tableWidget->rowCount();
                ui->tableWidget->setRowCount(current_rowcount+1);
                bool get_check = query.value(1).toBool();
                QString get_text = query.value(2).toString();
                QString get_time = query.value(3).toString();
                QString get_id = query.value(0).toString();
                QTableWidgetItem *tmp_checkbox = new QTableWidgetItem;
                //1
                if(get_check == true){
                    tmp_checkbox->setCheckState(Qt::Checked);
                }
                else{
                    tmp_checkbox->setCheckState(Qt::Unchecked);
                }

                ui->tableWidget->setItem(current_rowcount,0,tmp_checkbox);

                //2
                ui->tableWidget->setItem(current_rowcount,1,new QTableWidgetItem(get_text));

                //3
                ui->tableWidget->setItem(current_rowcount,2,new QTableWidgetItem(get_time));

                //4
                ui->tableWidget->setItem(current_rowcount,3,new QTableWidgetItem(get_id));
            }
        }
    }
    mem_database.close();
}

void mem::add_table_widget(){//tablewidget 添加项目  插入表数据  更新
    int current_rowcount = ui->tableWidget->rowCount();
    QString mem_data = ui->textEdit->toPlainText();
    QDateTime current_time = QDateTime::currentDateTime();
    QString current_date = current_time.toString("yyyy-MM-dd hh:mm:ss ddd");
    QString id = current_time.toString("yyyyMMddhhmmss");
    QString test_id;

    //tablewidget
    if(ui->textEdit->toPlainText() != ""){
        ui->tableWidget->setRowCount(current_rowcount+1);

        //1
        QTableWidgetItem *tmp_checkbox = new QTableWidgetItem;
        tmp_checkbox->setCheckState(Qt::Unchecked);
        ui->tableWidget->setItem(current_rowcount,0,tmp_checkbox);

        //2
        ui->tableWidget->setItem(current_rowcount,1,new QTableWidgetItem(mem_data));

        //3
        ui->tableWidget->setItem(current_rowcount,2,new QTableWidgetItem(current_date));

        //4
        if(tmp_id.isEmpty()){
            test_id = id;
            ui->tableWidget->setItem(current_rowcount,3,new QTableWidgetItem(id));
        }
        else{
            ui->tableWidget->setItem(current_rowcount,3,new QTableWidgetItem(tmp_id));
            test_id = tmp_id;
            tmp_id.clear();
        }
    }

    //数据表记录修改或增加操作
    if(!mem_database.open()){
        qDebug() << "Error: Failed to connect database." << mem_database.lastError();
    }
    else{
        QString testquerysql = "select id from mem where id = '" + test_id + "'";
        query.exec(testquerysql);
        query.next();
        QString get_test_id = query.value(0).toString();

        if(get_test_id.isEmpty()){
            QString insert_sql = "insert into mem values (?, ?, ?, ?)";
            query.prepare(insert_sql);
            query.addBindValue(id);
            query.addBindValue(false);
            query.addBindValue(mem_data);
            query.addBindValue(current_date);
            if(!query.exec()){
                qDebug()<<query.lastError();
            }
            else{
                qDebug()<<"insert successfully";
            }
        }
        else{
            QString update_sql = "update mem set me = '" + mem_data + "', memtime = '" + current_date + "' where id = " + test_id;
            query.prepare(update_sql);
            if(!query.exec()){
                qDebug()<< query.lastError();
            }
            else{
                qDebug()<< "update successfully";
            }
        }


    }
    mem_database.close();
}


QSqlDatabase mem::databaseinitial(){//数据库连接初始化
    QSqlDatabase tmp_database;
    if(QSqlDatabase::contains("qt_sql_default_connection")){
        tmp_database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else{
        tmp_database = QSqlDatabase::addDatabase("QSQLITE");
        tmp_database.setDatabaseName("men_database.db");
    }
    return tmp_database;
}

void mem::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);

    //画背景
    QPainter painter(this);
    QPixmap pix;
    pix.load("main.png");
    painter.drawPixmap(0,0,350,500,pix);
}
void mem::on_backtomain_clicked()//返回按钮
{
    emit sendsignal();
    this->hide();
}

void mem::on_save_mem_clicked()//保存按钮
{
    if(ui->textEdit->toPlainText() != ""){
        add_table_widget();
        ui->textEdit->clear();
        label = true;
    }

}

void mem::on_delete_row_clicked()//删除按钮
{
    if(ui->tableWidget->currentRow() != -1){
        if(!mem_database.open()){
            qDebug() << "Error: Failed to connect database." << mem_database.lastError();
        }
        else{//删除数据表记录
            QString delete_sql = "delete from mem where id = ?";
            QTableWidgetItem *delete_item = new QTableWidgetItem;
            delete_item = ui->tableWidget->item(ui->tableWidget->currentRow(),3);
            QString delete_id = delete_item->text();
            query.prepare(delete_sql);
            query.addBindValue(delete_id);
            if(!query.exec()){
                qDebug()<< query.lastError();
            }
            else{
                qDebug()<< "delete successfully";
            }
            mem_database.close();
        }
        ui->tableWidget->removeRow(ui->tableWidget->currentRow());
    }
    int rowcount = ui->tableWidget->rowCount();
    QTableWidgetItem *d_item = new QTableWidgetItem;
    QVector<int> v_d_rowcount;
    for(int i = 0; i < rowcount; i++){
        d_item = ui->tableWidget->item(i,0);
        if(d_item->checkState() == Qt::Checked){
            v_d_rowcount.push_back(i);
            d_item = ui->tableWidget->item(i,3);
            QString d_id = d_item->text();
            if(!mem_database.open()){
                qDebug() << "Error: Failed to connect database." << mem_database.lastError();
            }
            else{
                QString d_sql = "delete from mem where id = ?";
                query.prepare(d_sql);
                query.addBindValue(d_id);
                if(!query.exec()){
                    qDebug()<< query.lastError();
                }
                else{
                    qDebug()<< "delete successfully";
                }
                mem_database.close();
            }
        }
    }
    if(!v_d_rowcount.empty()){

        QVector<int>::reverse_iterator d_iterator = v_d_rowcount.rbegin();
        for(;d_iterator != v_d_rowcount.rend(); d_iterator++){
            ui->tableWidget->removeRow(*d_iterator);
        }
    }

}

void mem::on_revise_mem_clicked()//修改按钮
{
    if(ui->tableWidget->currentRow() != -1 && label == true){
        QTableWidgetItem *tmp_item = new QTableWidgetItem;

        tmp_item = ui->tableWidget->item(ui->tableWidget->currentRow(),1);
        ui->textEdit->setPlainText(tmp_item->text());
        tmp_item = ui->tableWidget->item(ui->tableWidget->currentRow(),3);
        tmp_id = tmp_item->text();

        ui->tableWidget->removeRow(ui->tableWidget->currentRow());

        //移动光标至末尾
        ui->textEdit->setFocus();
        QTextCursor cur(ui->textEdit->textCursor());
        cur.movePosition(QTextCursor::End,QTextCursor::MoveAnchor,1);
        ui->textEdit->setTextCursor(cur);
        label = false;
    }
}

void mem::double_click_table(){//双击tablewidget
    if(ui->tableWidget->currentRow() != -1){
        QTableWidgetItem *tmp_item = new QTableWidgetItem;
        tmp_item = ui->tableWidget->item(ui->tableWidget->currentRow(),1);
        ui->textEdit->setPlainText(tmp_item->text());
        tmp_item = ui->tableWidget->item(ui->tableWidget->currentRow(),3);
        tmp_id = tmp_item->text();
        ui->tableWidget->removeRow(ui->tableWidget->currentRow());
    }
}

//移动窗口
void mem::mousePressEvent(QMouseEvent *e)
{
    last = e->globalPos();
}
void mem::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    move(x()+dx, y()+dy);
}
void mem::mouseReleaseEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    move(x()+dx, y()+dy);
}

