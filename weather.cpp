#include "weather.h"

weather::weather()
{

}

void weather::run(){


    QNetworkAccessManager* manager = new QNetworkAccessManager;

    QProcess* p = new QProcess;;
    p->start("cmd", QStringList()<<"/c"<<"ping www.baidu.com");
    p->waitForStarted();
    p->waitForFinished();
    QString all = QString::fromLocal8Bit(p->readAll());
    if(!all.contains("请求超时")){
        QFile
        QString city =
    }
}
