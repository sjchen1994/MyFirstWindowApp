#include "widget.h"
#include <QApplication>
#include <QFrame>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    QDesktopWidget *d = QApplication::desktop();
    int width = d->width();
    w.move(width-250,100);
    w.show();
    while(true){
        a.processEvents();
        if(w.getclosed() == 1){
            break;
        }
    }
    return 0;
}


