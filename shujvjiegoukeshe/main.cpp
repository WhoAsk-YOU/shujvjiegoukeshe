#include "start_widget.h"
#include <QApplication>
#include <QSqlDatabase>  //数据库

int main(int argc, char *argv[]){
    QApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");  //连接数据库
    db.setHostName("localhost");
    db.setUserName("root");
    db.setDatabaseName("shujvjiegou");
    db.setPassword("1016");
    db.open();

    Start_widget w;
    w.show();
    QObject::connect(&a, &QApplication::aboutToQuit, [&db](){  //若关闭应用程序，则断开与数据库的连接
        db.close();
    });
    return a.exec();
}
