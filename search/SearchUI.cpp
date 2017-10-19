#include "SearchUI.h"
#include "ui_Search.h"
#include "QDesktopServices"


SearchUI::SearchUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //初始化时间控件
    ui -> dateTimeEdit_StartTime ->setDisplayFormat("yyyy-MM-dd HH-mm-ss");
    ui -> dateTimeEdit_StartTime ->setDateTime(QDateTime::fromString(QDate::currentDate().toString("yyyy-MM-dd 00-00-00"), "yyyy-MM-dd HH-mm-ss"));
    ui -> dateTimeEdit_EndTime -> setDisplayFormat("yyyy-MM-dd HH-mm-ss");
    ui -> dateTimeEdit_EndTime ->setDateTime(QDateTime::currentDateTime());

    standarditemModel = new QStandardItemModel(this);
    ui -> listView_SearchResult -> setModel(standarditemModel);

    ui -> pushButton_SsysShow -> setEnabled(false);
    ui -> pushButton_SoftShow -> setEnabled(false);
    ui -> pushButton_FileShow -> setEnabled(false);

    ui-> listWidget_SearchPath ->addItem("D:/SanguDetect/Vision/FaultImage");
    ui-> listWidget_SearchPath ->addItem("D:/SanguDetect/Vision/InspectImage");
}

SearchUI::~SearchUI()
{
    delete ui;
}

//按钮消息
void SearchUI::on_pushButton_Search_clicked()
{
    ui -> pushButton_Search -> setEnabled(false);
    ui -> pushButton_FileShow -> setEnabled(false);
    ui -> pushButton_SoftShow -> setEnabled(false);
    ui -> pushButton_SsysShow -> setEnabled(false);
    ui -> label_PhotoShow -> setScaledContents(false);

    ui ->label_TempPath -> setText("");

    FileSearchThread *mythread = new FileSearchThread;

    //将列表中路径加入default_Path_list参数中
    int row = 0;
    default_Path_list.clear();
    while(row < ui->listWidget_SearchPath->count())
    {
        default_Path_list.append( ui->listWidget_SearchPath->item(row)->text() );
        row++;
    }
    //qDebug() << mylist1 ;

    //连接信号与槽，启动工作线程，并发送信号
    if (default_Path_list.empty())
    {
        QMessageBox::information(this, tr("Warning"), tr("默认路径不能为空!"), QMessageBox::Ok);
        ui -> pushButton_Search -> setEnabled(true);
    }else
    {
        //QString afile = ui -> lineEdit -> text();//默认路径
        keyWord = ui -> lineEdit_KeyWord -> text();//关键字

        QDateTime oldtime = ui -> dateTimeEdit_StartTime ->dateTime();
        QDateTime newtime = ui -> dateTimeEdit_EndTime -> dateTime();

        if (oldtime >= newtime) {
            QMessageBox::information(this, tr("Warning"), tr("时间输入错误"), QMessageBox::Ok);
            ui -> pushButton_Search -> setEnabled(true);
            //return ;
        }else
        {
            connect(mythread, SIGNAL(returnPath(QStringList)), this, SLOT(labelShowObjectPath(QStringList)), Qt::QueuedConnection);
            connect(this, SIGNAL(sendData(QDateTime, QDateTime, QStringList, QString)), mythread, SLOT(receiveData(QDateTime, QDateTime, QStringList, QString)), Qt::QueuedConnection);
            connect(mythread, SIGNAL(returnProgress(QString)), this, SLOT(labelShowPath(QString)), Qt::QueuedConnection);

            mythread->start();

            standarditemModel ->removeRows(0, standarditemModel->rowCount());

            emit sendData(oldtime, newtime, default_Path_list, keyWord);
        }
    }
}

//接收路径信息并打印到结果列表中
void SearchUI::labelShowObjectPath(QStringList objectpathlist)
{
    object_path_list = objectpathlist;
    //qDebug() << mylist;

    //列表显示
    if(object_path_list.empty())
    {
        QMessageBox::information(this, tr("Warning"), tr("路径下没有文件！"), QMessageBox::Ok);
    }else
    {
        for(int i = 0; i < object_path_list.size(); i++)
        {
            QString string = static_cast <QString> (object_path_list.at(i));
            QStandardItem *item = new QStandardItem(string);
            standarditemModel -> appendRow(item);
        }
    }

    ui->listView_SearchResult -> setModel(standarditemModel);

    ui -> pushButton_Search -> setEnabled(true);
}

//进度显示
void SearchUI::labelShowPath(QString temp_path)
{
    //qDebug() << "t: " <<temp_path;
    ui ->label_TempPath -> setText(temp_path);
}

//系统显示图片
void SearchUI::on_pushButton_SsysShow_clicked()
{
    ui -> pushButton_SsysShow -> setEnabled(false);

    LPCWSTR s = LPCWSTR(show_path.utf16());
    ShellExecute(NULL, (LPCWSTR)L"open", s, (LPCWSTR)L"", (LPCWSTR)L"", SW_SHOWNORMAL);
}

//软件显示图片
void SearchUI::on_pushButton_SoftShow_clicked()
{
    ui -> pushButton_SoftShow -> setEnabled(false);

    QPixmap pixmap(show_path);
    pixmap.scaled(ui -> label_PhotoShow ->size(), Qt::KeepAspectRatio);
    ui -> label_PhotoShow -> setScaledContents(true);
    ui -> label_PhotoShow -> setPixmap(pixmap);
    ui -> label_PhotoShow -> show();
}

//显示图片所在文件夹
void SearchUI::on_pushButton_FileShow_clicked()
{
    ui -> pushButton_FileShow -> setEnabled(false);

    int n_temp = 0;
    for(int k = 0; k < show_path.size(); k++)
    {
        if(show_path.at(k) == "/")
        {
            n_temp = k;
        }
    }

    //qDebug() <<pStr.left(n);
    QDesktopServices::openUrl(QUrl(show_path.left(n_temp),QUrl::TolerantMode));
}

//单击结果列表显示按钮
void SearchUI::on_listView_SearchResult_clicked(const QModelIndex &index)
{
    ui -> pushButton_FileShow -> setEnabled(true);
    ui -> pushButton_SoftShow -> setEnabled(true);
    ui -> pushButton_SsysShow -> setEnabled(true);

    show_path = index.data().toString();
    ui ->label_TempPath -> setText(show_path);
}

//双击路径列表修改路径
void SearchUI::on_listWidget_SearchPath_itemDoubleClicked(QListWidgetItem *item)
{
    item->setFlags(item->flags() | Qt::ItemIsEditable);
}

//添加路径信息
void SearchUI::on_pushButton_AddDefaultPath_clicked()
{
    QString inputpath = ui -> lineEdit_inputPath ->text();
    if(inputpath.isEmpty())
    {
        QMessageBox::information(this, tr("Warning"), tr("默认路径不能为空!"), QMessageBox::Ok);
    }else
    {
        ui-> listWidget_SearchPath ->addItem(inputpath);
    }
}

//删除路径信息
void SearchUI::on_pushButton_delDefaultPath_clicked()
{
    if(ui->listWidget_SearchPath -> currentItem()!=Q_NULLPTR)
    {
        QListWidgetItem *item = ui ->listWidget_SearchPath -> takeItem(ui -> listWidget_SearchPath -> currentRow());
        delete item;
    }
}
