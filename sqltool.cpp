#include "sqltool.h"

#include <QDebug>
#include <QtSql>

SqlTool::SqlTool()
{

}

void SqlTool::test()
{
    //创建连接
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");//第二个参数可以设置连接名字，这里为default

    db.setDatabaseName("./data.db");// 设置数据库名与路径, 此时是放在上一个目录
    //打开连接
    if(!db.open())
    {
        qDebug()<<"Failed to connect.";
        qDebug() << db.lastError();
    }else{
        qDebug()<<"open success";
    }

    //各种操作
    QSqlQuery qry;
    //创建table
    qry.prepare("CREATE TABLE IF NOT EXISTS config (key VARCHAR UNIQUE PRIMARY KEY, value VARCHAR)");
    if(!qry.exec()){
        qDebug() << qry.lastError();
    }
    else{
        qDebug() << "Table created!";
    }

    //增
    qry.prepare("INSERT INTO config (key, value) VALUES ('back_color_using','1234564')");
    if(!qry.exec()){
        qDebug() << qry.lastError();
    }
    else{
        qDebug("Inserted!");
    }

    //更新
    qry.prepare( "UPDATE config SET value = 'ff00ffff' WHERE key = 'back_color_using'" );
    if( !qry.exec() )
    {
        qDebug() << qry.lastError();
    }
    else
    {
        qDebug( "Updated!" );
    }

    //查
    qry.prepare("SELECT * FROM config");
    if(!qry.exec())
    {
        qDebug() << qry.lastError();
    }
    else
    {
        qDebug("Selected!");
        QSqlRecord rec = qry.record();
        int cols = rec.count();//字段数
        for(int c=0; c<cols; c++)
        {
            qDebug() << QString("Column %1: %2").arg(c).arg(rec.fieldName(c));
        }

        while (qry.next()) {
            qDebug()<<"key:"<<qry.value("key").toString()
                   <<"value:"<<qry.value("value").toString();
            //            qDebug()<<"key:"<<qry.value(0).toString()
            //                   <<"value:"<<qry.value(1).toString();
        }
    }

    //删除
    qry.prepare("DELETE FROM config WHERE key = 'back_color_using'" );
    if( !qry.exec() ){
        qDebug() << qry.lastError();
    }
    else{
        qDebug( "Deleted!" );
    }

    //关闭
    db.close();

}
