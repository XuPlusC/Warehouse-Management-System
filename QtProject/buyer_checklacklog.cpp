#include "buyer_checklacklog.h"
#include "ui_buyer_checklacklog.h"
#include "dbhelper.h"
#include <QDateTime>
#include <QSqlQuery>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QString>

Buyer_CheckLackLog::Buyer_CheckLackLog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Buyer_CheckLackLog)
{
    ui->setupUi(this);
    setWindowFlag(Qt::Window);
    ui->tableView_display->setEditTriggers(QAbstractItemView::NoEditTriggers);
    updateTable();
}

Buyer_CheckLackLog::~Buyer_CheckLackLog()
{
    delete ui;
}

void Buyer_CheckLackLog::on_btn_back_clicked()
{
    emit(backToMain());
}

void Buyer_CheckLackLog::updateTable()
{
    DBHelper *helper = DBHelper::getInstance();
    helper->connectDatabase();

    if(currentRowCount != 0 && model != NULL)
    {
        model->removeRows(0, model->rowCount());
        currentRowCount = 0;
    }

    if(model != NULL)
        delete model;

    model = new QStandardItemModel();
    model->setColumnCount(7);
    model->setHeaderData(0, Qt::Horizontal, "缺货记录编号");
    model->setHeaderData(1, Qt::Horizontal, "隶属出货单");
    model->setHeaderData(2, Qt::Horizontal, "货品编号");
    model->setHeaderData(3, Qt::Horizontal, "货品名称");
    model->setHeaderData(4, Qt::Horizontal, "缺货个数");
    model->setHeaderData(5, Qt::Horizontal, "满足情况");
    model->setHeaderData(6, Qt::Horizontal, "提交时间");


    QString str_query;
    str_query = "select * from LACK_LOG ORDER BY time DESC;";
    QSqlQuery query;
    query.prepare(str_query);
    query.exec();
    while(query.next())
    {
        // if the user wants to show satisfied lacklog, then show it; else ignore it.
        if(!ui->checkBox_seeSat->isChecked() && query.value(5).toInt() == 2)
            continue;

        QString str_lackLogID = query.value(0).toString();
        model->setItem(currentRowCount, 0, new QStandardItem(str_lackLogID));
        QString str_shipmentID = query.value(1).toString();
        model->setItem(currentRowCount, 1, new QStandardItem(str_shipmentID));
        QString str_goodsID = query.value(2).toString();
        model->setItem(currentRowCount, 2, new QStandardItem(str_goodsID));
        QString str_goodsName = query.value(3).toString();
        model->setItem(currentRowCount, 3, new QStandardItem(str_goodsName));
        QString str_lackNum = query.value(4).toString();
        model->setItem(currentRowCount, 4, new QStandardItem(str_lackNum));
        int int_status = query.value(5).toInt();
        QString str_status;
        if(int_status == 0)
            str_status = "不可满足";
        else if(int_status == 1)
            str_status = "可满足";
        else
            str_status = "已满足";
        model->setItem(currentRowCount, 5, new QStandardItem(str_status));
        QDateTime datetime = query.value(6).toDateTime();
        QString str_datetime = datetime.toString("yyyy-MM-dd hh:mm:ss");
        model->setItem(currentRowCount, 6, new QStandardItem(str_datetime));

        currentRowCount++;
    }

    helper->disconnectDatabase();
    ui->tableView_display->setModel(model);
    ui->tableView_display->resizeColumnsToContents();
}

void Buyer_CheckLackLog::on_btn_refresh_clicked()
{
    updateTable();
}
