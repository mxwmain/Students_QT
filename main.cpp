#include "mainwindow.h"
#include "login.h"
#include <QApplication>
#include <QtSql>
#include <QMessageBox>
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");//本地主机IP和"127.0.0.1"一样
    db.setPort(3306);//默认端口
    db.setUserName("root");//用户名
    db.setPassword("200109meng");//数据库密码
    //注意:需要在MySQL中先创建一个数据库才可以
    db.setDatabaseName("students");//连接的数据库
    bool ok = db.open();
    if(!ok){
        QMessageBox::critical(0, QObject::tr("连接数据库失败！"), db.lastError().text());
        return 1;
    }else{
         qDebug()<<QObject::tr("连接到数据库。");
    }
    qApp->setWindowIcon(QIcon(":/images/windowIcon.png"));
    Login login;
    login.show();
    return a.exec();
}
