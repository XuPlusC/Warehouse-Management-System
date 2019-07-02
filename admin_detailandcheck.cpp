#include "admin_detailandcheck.h"
#include "ui_admin_detailandcheck.h"
#include "dbhelper.h"
#include <QDateTime>
#include <QDebug>
#include <QSqlQuery>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QString>
#define TYPE_PURCHASE 0
#define TYPE_SHIPMENT 1

admin_detailAndCheck::admin_detailAndCheck(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::admin_detailAndCheck)
{
    ui->setupUi(this);
    currentRowCount = 0;
    allowedToCheck = 1;
}

admin_detailAndCheck::~admin_detailAndCheck()
{
    delete ui;
}

void admin_detailAndCheck::setAdminID(int id)
{
    this->adminID = id;
}

void admin_detailAndCheck::setAttributes(int type, int listID, int status)
{
    this->type = type;
    this->listID = listID;
    this->status = status;
    updateTable();
}

void admin_detailAndCheck::updateTable()
{
    if(currentRowCount != 0)
    {
        model->removeRows(0, model->rowCount());
        currentRowCount = 0;
    }

    QString status_display;
    switch (status)
    {
    case 0: status_display = "未审核"; break;
    case 1: status_display = "审核未通过";
            ui->btn_uncheck->setDisabled(true);
//            ui->btn_check->setDisabled(true);
//            ui->btn_suspend->setDisabled(true);
            break;
    case 2: status_display = "审核通过";
//            ui->btn_uncheck->setDisabled(true);
            ui->btn_check->setDisabled(true);
//            ui->btn_suspend->setDisabled(true);
            break;
    case 3: status_display = "缺货挂起";
//            ui->btn_uncheck->setDisabled(true);
//            ui->btn_check->setDisabled(true);
            ui->btn_suspend->setDisabled(true);
            break;
    }
    ui->label_status->setText("当前状态："+status_display);

    QString str_query = "select * from ";
    QString str_listID;
    str_listID.setNum(listID);

    model = new QStandardItemModel();
    model->setColumnCount(4);
    model->setHeaderData(0, Qt::Horizontal, "货品编号");
    model->setHeaderData(1, Qt::Horizontal, "货品名");
    if(type == TYPE_PURCHASE)
    {
        model->setHeaderData(2, Qt::Horizontal, "进货数");
        str_query.append("PURCHASE_LOG where purchaseID = ");
        ui->label_checking->setText("正在查看进货单 "+str_listID);
        ui->btn_suspend->setDisabled(true);                         // you can't suspend a purchase list
    }
    else if(type == TYPE_SHIPMENT)
    {
        model->setHeaderData(2, Qt::Horizontal, "出货数");
        str_query.append("SHIPMENT_LOG where shipmentID = ");
        ui->label_checking->setText("正在查看出货单 "+str_listID);
    }
    model->setHeaderData(3, Qt::Horizontal, "库存量");

    // query database and fill in the table
    str_query.append(str_listID);
    str_query.append(";");

    DBHelper *helper = DBHelper::getInstance();
    helper->connectDatabase();
    QSqlQuery query_list, query_goods;
    query_list.prepare(str_query);
    query_list.exec();
    while(query_list.next())
    {
        // column 1, goodsID
        QString goodsID = query_list.value(2).toString();
        model->setItem(currentRowCount, 0, new QStandardItem(goodsID));
        // column 2, goodsName
        QString goodsName = query_list.value(3).toString();
        model->setItem(currentRowCount, 1, new QStandardItem(goodsName));
        // column 3, the number of goods to purchase or shipment
        QString num = query_list.value(4).toString();
        model->setItem(currentRowCount, 2, new QStandardItem(num));
        // column 4, the number of goods in storage
        QString str_query_goods = "select storageNum from GOODS where goodsID = ";
        str_query_goods.append(goodsID);
        str_query_goods.append(";");
        query_goods.prepare(str_query_goods);
        query_goods.exec();
        query_goods.next();
        QString storageNum = query_goods.value(0).toString();
        model->setItem(currentRowCount, 3, new QStandardItem(storageNum));
        // if type is shipment, and shipment amount is bigger than storage,
        // set the storageNum cell to red, and set the allowedToCheck as 0
        if(type == TYPE_SHIPMENT && (storageNum.toInt() < num.toInt()))
        {
            allowedToCheck = 0;
            model->item(currentRowCount, 3)->setForeground(QBrush(QColor(255, 0, 0)));
            ui->btn_check->setDisabled(true);
        }
        currentRowCount++;
    }

    helper->disconnectDatabase();
    ui->tableView_display->setModel(model);
    ui->tableView_display->setEditTriggers(QAbstractItemView::NoEditTriggers);  // dont allow to edit the table
}

void admin_detailAndCheck::on_btn_back_clicked()
{
    this->hide();
    emit(closed());
}

void admin_detailAndCheck::on_btn_check_clicked()
{
    DBHelper *helper = DBHelper::getInstance();
    helper->connectDatabase();

    QString str_listID;
    str_listID.setNum(listID);
    QString str_adminID;
    str_adminID.setNum(adminID);
    QString str_query;

    // update status
    if(type == TYPE_PURCHASE)
    {
        str_query = "update PURCHASE set status = 2, adminID = ";
        str_query.append(str_adminID);
        str_query.append(" where purchaseID = ");
    }
    else if(type == TYPE_SHIPMENT)
    {
        str_query = "update SHIPMENT set status = 2, adminID = ";
        str_query.append(str_adminID);
        str_query.append(" where shipmentID = ");
    }
    str_query.append(str_listID);
    str_query.append(";");

    QSqlQuery query;
    query.prepare(str_query);
    query.exec();


    // update good storage num
    for (int i = 0; i < currentRowCount; ++i)
    {
        QString str_goodsID = model->item(i, 0)->text();
        QString str_changeNum = model->item(i, 2)->text();
        str_query = "select storageNum from GOODS where goodsID = ";
        str_query.append(str_goodsID);
        str_query.append(";");
        query.prepare(str_query);
        query.exec();
        query.next();
        int int_currentNum = query.value(0).toInt();
        int int_changeNum = str_changeNum.toInt();
        qDebug() << "updating storage for goodsID " << model->item(i, 0)->text() <<" ... before :" << int_currentNum;
        if(type == TYPE_PURCHASE)
        {
            int_currentNum += int_changeNum;
        }
        else if(type == TYPE_SHIPMENT)
        {
            int_currentNum -= int_changeNum;
        }
        qDebug() << "after: " << int_currentNum;
        QString str_curruntNum;
        str_curruntNum = QString::number(int_currentNum);
        str_query = "update GOODS set storageNum = :currentNum where goodsID = :goodsID;";
        query.prepare(str_query);
        query.bindValue(":currentNum", int_currentNum);
        query.bindValue(":goodsID", str_goodsID.toInt());
        qDebug() << "____";
        query.exec();
        qDebug() << "^^^^";
    }

    if(type == TYPE_SHIPMENT)
    {
        str_query = "update LACK_LOG set isSatisfied = '2' where shipmentID = :shipmentID";
        query.prepare(str_query);
        query.bindValue(":shipmentID", listID);
        query.exec();
    }

    updateTable();
    helper->disconnectDatabase();
    ui->label_status->setText("当前状态：审核通过");
    ui->btn_check->setDisabled(true);
    ui->btn_uncheck->setDisabled(true);
    ui->btn_suspend->setDisabled(true);
}

void admin_detailAndCheck::on_btn_uncheck_clicked()
{
    DBHelper *helper = DBHelper::getInstance();
    helper->connectDatabase();

    QString str_listID;
    str_listID.setNum(listID);
    QString str_adminID;
    str_adminID.setNum(adminID);
    QString str_query;

    // update status
    if(type == TYPE_PURCHASE)
    {
        str_query = "update PURCHASE set status = 1, adminID = ";
        str_query.append(str_adminID);
        str_query.append(" where purchaseID = ");
    }
    else if(type == TYPE_SHIPMENT)
    {
        str_query = "update SHIPMENT set status = 1, adminID = ";
        str_query.append(str_adminID);
        str_query.append(" where shipmentID = ");
    }
    str_query.append(str_listID);
    str_query.append(";");

    QSqlQuery query;
    query.prepare(str_query);
    query.exec();

    ui->label_status->setText("当前状态：审核不通过");
//    ui->btn_check->setDisabled(false);
    ui->btn_uncheck->setDisabled(true);
    if(type == TYPE_SHIPMENT)
        ui->btn_suspend->setDisabled(false);

    helper->disconnectDatabase();
}

void admin_detailAndCheck::on_btn_suspend_clicked()
{
    ui->label_status->setText("当前状态：缺货挂起");
    ui->btn_check->setDisabled(true);
    ui->btn_uncheck->setDisabled(false);
    ui->btn_suspend->setDisabled(true);

    DBHelper *helper = DBHelper::getInstance();
    helper->connectDatabase();

    QString str_listID;
    str_listID.setNum(listID);
    QString str_adminID;
    str_adminID.setNum(adminID);
    QString str_query;

    // update purchase/shipment status
    if(type == TYPE_PURCHASE)
    {
        str_query = "update PURCHASE set status = 3, adminID = ";
        str_query.append(str_adminID);
        str_query.append(" where purchaseID = ");
    }
    else if(type == TYPE_SHIPMENT)
    {
        str_query = "update SHIPMENT set status = 3, adminID = ";
        str_query.append(str_adminID);
        str_query.append(" where shipmentID = ");
    }
    str_query.append(str_listID);
    str_query.append(";");

    QSqlQuery query;
    query.prepare(str_query);
    query.exec();

    // add lacklog
    str_query = "select MAX(lackLogID) from LACK_LOG;";
    query.prepare(str_query);
    query.exec();
    query.next();

    int int_newLackLogID;
    if(query.value(0).isNull())
        int_newLackLogID = 1;
    else
        int_newLackLogID = query.value(0).toInt() + 1;

    QDateTime time = QDateTime::currentDateTime();
    QString str_time = time.toString("yyyy-MM-dd hh:mm:ss");
//    qDebug() << "current time:" << str_time;

    for(int i = 0; i < currentRowCount; ++i)
    {
        int int_need = model->item(i, 2)->text().toInt();
        int int_storage = model->item(i, 3)->text().toInt();
        qDebug() << "need:" << int_need << "storage:" << int_storage;
        if(int_need > int_storage)
        {
            qDebug() << "Inserting!";
            str_query = "insert into LACK_LOG values(:lackLogID, :shipmentID, :goodsID, :goodsName, :lackNum, '0', :time);";
            query.prepare(str_query);
            query.bindValue(":lackLogID", int_newLackLogID++);
            query.bindValue(":shipmentID", listID);
            query.bindValue(":goodsID", model->item(i, 0)->text().toInt());
            query.bindValue(":goodsName", model->item(i, 1)->text());
            query.bindValue(":lackNum", int_need);
            query.bindValue(":time", str_time);
            query.exec();
        }
    }

    helper->disconnectDatabase();
}
