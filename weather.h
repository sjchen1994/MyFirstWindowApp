#ifndef WEATHER_H
#define WEATHER_H
#include <QProcess>
#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QInputDialog>
class weather: public QThread
{
public:
    weather();
    void run();

public:

};

#endif // WEATHER_H
