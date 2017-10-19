#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QString>
#include <QDateTime>
#include <SearchUI.h>
#include <QDir>

class FileSearchThread : public QThread
{
    Q_OBJECT
public:
    FileSearchThread();
    ~FileSearchThread();

    //QString str3 = "";//遍历路径
    QStringList object_path_list;//存储路径信息
    QStringList object_date_list;//存储日期路径
    QDateTime timestart;//开始时间
    QDateTime timeend;//终止时间
    QStringList default_path_list;//默认地址
    QString key_word;
    QString file_path_temp;

    //int progress = 0;//进度条参数

    int searchObjectPath(QString);//路径搜索
    void searchDateFile(QDate, QDate, QString);

protected:
    void run();//线程函数

signals:
    void returnPath(QStringList);//信号返回路径信息

    void returnProgress(QString);//返回进度条数值

private slots:
    void receiveData(QDateTime ,QDateTime , QStringList, QString);//槽接收时间信息

};

#endif // MYTHREAD_H
