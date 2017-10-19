#include "FileSearchThread.h"
#include <QDebug>

FileSearchThread::FileSearchThread()
{
    //stop = false;
    object_path_list.clear();
    default_path_list.clear();
    object_date_list.clear();
}


FileSearchThread::~FileSearchThread()
{

}


void FileSearchThread::run()
{
    //progress = 0;

    while(1)
    {
        QDate timestart_date = timestart.date();
        QDate timesend_date = timeend.date();

        //获取满足条件的日期列表
        if (timestart_date.toString() != "")
        {
            //qDebug() << "time not empty!" ;

            //qDebug()<<"fff2: " << str1 << str1.size();
            for(int i = 0; i < default_path_list.size(); i++)
            {
                searchDateFile(timestart_date, timesend_date, default_path_list.at(i));
                //qDebug() << "datelistqqq: "<<datelist;
            }
            break;
        }

    }
    //qDebug() << "datelist: "<<  object_date_list.size();

    //遍历日期列表,找出目的路径
    for(int i = 0; i < object_date_list.size(); i++)
    {
        QString search_date_path = static_cast <QString> (object_date_list.at(i));
        //qDebug() <<"search_date_path: "<< search_date_path;

        //起止日期相同
        if (timestart.date() == timeend.date())
        {
            QTime time_start = timestart.time();
            QTime time_end = timeend.time();
            //qDebug() << time_start << time_end;

            while(time_start <= time_end)
            {
                QString search_time_path = search_date_path + time_start.toString("/HH-mm-ss");
                //qDebug() << search_time_path;
                searchObjectPath(search_time_path);

                time_start = time_start.addSecs(1);
            }
        } else
        {
            //判断开始日期下的时间文件
            if(search_date_path.indexOf( timestart.date().toString("yyyy-MM-dd")) != -1)
            {
                QTime time_start = timestart.time();
                QTime time_temp(0,0,0);
                //qDebug() << time_start << time_temp;

                if(time_start == time_temp)
                {
                    QString search_time_path = search_date_path + time_start.toString("/HH-mm-ss");
                    searchObjectPath(search_time_path);

                    time_start = time_start.addSecs(1);

                    while(time_start != time_temp)
                    {
                        QString search_time_path = search_date_path + time_start.toString("/HH-mm-ss");
                        //qDebug() <<"search_time_path: "<< search_time_path;
                        searchObjectPath(search_time_path);

                        time_start = time_start.addSecs(1);
                    }

                } else
                {
                    while(time_start != time_temp)
                    {
                        QString search_time_path = search_date_path + time_start.toString("/HH-mm-ss");
                        //qDebug() <<"search_time_path: "<< search_time_path;
                        searchObjectPath(search_time_path);

                        time_start = time_start.addSecs(1);
                    }
                }
            }
            //判断结束日期下的时间文件
            else if(search_date_path.indexOf( timeend.date().toString("yyyy-MM-dd")) != -1)
            {
                QTime time_end = timeend.time();
                qDebug() << "ttttt" <<time_end;

                QTime time_temp(0,0,0);
                while(time_temp <= time_end)
                {
                    QString search_time_path = search_date_path + time_temp.toString("/HH-mm-ss");
                    //qDebug() << file2;
                    searchObjectPath(search_time_path);

                    time_temp = time_temp.addSecs(1);
                }
            }else//中间日期直接判断
            {
                //qDebug() << file;
                searchObjectPath(search_date_path);
            }
        }
    }

    emit returnPath(object_path_list);
    //qDebug() << object_path_list;
}


//接收数据并赋值给全局变量
void FileSearchThread::receiveData(QDateTime oldtime, QDateTime newtime, QStringList default_path, QString keyword)
{
    timestart = oldtime;
    timeend = newtime;
    default_path_list = default_path;
    key_word = keyword;
    //qDebug() << "date: " << str1;
}


//目标路径搜索
int FileSearchThread::searchObjectPath(QString find_file_path)
{
    QString path_temp = "";

    QDir dir(find_file_path);

    if (!dir.exists())
    {
        return 0;
    }

    QStringList listFliter = QStringList() ;//<< "*.png" << "*.jpg" << "*.bmp";
    QFileInfoList fileList = dir.entryInfoList(listFliter);

    for(int i = 0; i < fileList.size(); i++)
    {
        QFileInfo info = fileList.at(i);

        if(info.isDir())
        {
            //getFile(info.filePath());
        }

        if(info.isFile())
        {
            path_temp = info.absoluteFilePath();
            //qDebug() << str3;
            //object_path_list.append(path_temp);

            if (key_word != "")
            {
                if(path_temp.indexOf(key_word) != -1)
                {
                    object_path_list.append(path_temp);
                    emit returnProgress(path_temp);
                    //sleep(1);
                }
            } else
            {
                object_path_list.append(path_temp);
                emit returnProgress(path_temp);
                //sleep(1);
            }

            //emit returnProgress(path_temp);
        } else
        {
            if(info.fileName() == "." || info.fileName() == "..") continue;
            //qDebug() << "path2: " << info.absoluteFilePath();
            searchObjectPath(info.absoluteFilePath());
        }
    }

    return 0;
}


//日期搜索
void FileSearchThread::searchDateFile(QDate date_start, QDate ddate_end2, QString default_path)
{
    while(date_start <= ddate_end2)
    {
        QString path_temp = default_path;
        path_temp += date_start.toString("/yyyy-MM-dd");
        QDir dir(path_temp);

        if (dir.exists())
        {
            object_date_list.append(path_temp);
            //qDebug() << "t1: "<<object_date_list;
        }

        date_start = date_start.addDays(1);
    }
}
