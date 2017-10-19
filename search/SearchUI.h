#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QString>
#include <FileSearchThread.h>
#include <QMessageBox>
#include <QDateTime>
#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include <windows.h>
#include <shellapi.h>
#include <QListWidget>

namespace Ui {
class Widget;
}

class SearchUI : public QWidget
{
    Q_OBJECT

public:
    QString keyWord = "";//关键字
    QStringList object_path_list;//目的路径列表
    QStringList default_Path_list;//默认路径列表
    QString show_path;//图片显示路径

    explicit SearchUI(QWidget *parent = 0);
    ~SearchUI();

private slots:
    void on_pushButton_Search_clicked();

    //void on_listView_doubleClicked(const QModelIndex &index);

private slots:
    void labelShowObjectPath(QStringList);//接收并显示路径信息
    void labelShowPath(QString);//进度显示

    void on_pushButton_SsysShow_clicked();

    void on_pushButton_SoftShow_clicked();

    void on_pushButton_FileShow_clicked();

    void on_listView_SearchResult_clicked(const QModelIndex &index);

    void on_listWidget_SearchPath_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButton_AddDefaultPath_clicked();

    void on_pushButton_delDefaultPath_clicked();

signals:
    void sendData(QDateTime, QDateTime, QStringList, QString);//传递时间信息

private:
    Ui::Widget *ui;

    QStandardItemModel *standarditemModel;

};

#endif // WIDGET_H
