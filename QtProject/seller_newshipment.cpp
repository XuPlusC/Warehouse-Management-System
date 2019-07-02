#include "seller_newshipment.h"
#include "ui_seller_newshipment.h"

#include <QDateTime>

Seller_newShipment::Seller_newShipment(int sellerID, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Seller_newShipment)
{
    this->sellerID = sellerID;
    ui->setupUi(this);

    model = new QStandardItemModel();
    /*设置列字段名*/
    model->setColumnCount(3);
    model->setHeaderData(0, Qt::Horizontal, "货品编号");
    model->setHeaderData(1, Qt::Horizontal, "货品名称");
    model->setHeaderData(2, Qt::Horizontal, "出库数量");

    currentRowCount = 0;
    ui->tableView_display->setModel(model);
    ui->tableView_display->setEditTriggers(QAbstractItemView::NoEditTriggers);  // dont allow to edit the table
    ui->tableView_display->resizeColumnsToContents();
}

Seller_newShipment::~Seller_newShipment()
{
    delete ui;
}

void Seller_newShipment::on_btn_back_clicked()
{
    emit(backToMain());
}

void Seller_newShipment::on_newshipmentlogCancel()
{
    newShipmentLog->hide();
    model->setRowCount(--currentRowCount);
}

void Seller_newShipment::on_newShipmentLogSubmit(QString goods_name, QString goods_num, QString goods_id)
{
    newShipmentLog->hide();
    for(int i = 0; i < currentRowCount-1; ++i)
    {
        if(model->item(i, 0)->text().toInt() == goods_id.toInt())
        {
            int int_newNum = goods_num.toInt();
            int_newNum += model->item(i, 2)->text().toInt();
            QString str_newNum;
            str_newNum = str_newNum.number(int_newNum);
            qDebug() << "total is " << str_newNum;
            model->setItem(i, 2, new QStandardItem(str_newNum));

            model->setRowCount(--currentRowCount);
            return;
        }
    }

    model->setItem(currentRowCount - 1, 0, new QStandardItem(goods_id));
    model->setItem(currentRowCount - 1, 1, new QStandardItem(goods_name));
    model->setItem(currentRowCount - 1, 2, new QStandardItem(goods_num));
}

void Seller_newShipment::on_btn_add_clicked()
{
    model->setRowCount(++currentRowCount);
    if(newShipmentLog != nullptr)
        delete newShipmentLog;
    newShipmentLog = new Seller_newShipmentLog();
    QObject::connect(newShipmentLog, SIGNAL(newShipmentLogSubmit(QString, QString, QString)), this, SLOT(on_newShipmentLogSubmit(QString, QString, QString)));
    QObject::connect(newShipmentLog, SIGNAL(cancelled()), this, SLOT(on_newshipmentlogCancel()));
    newShipmentLog->show();
}

void Seller_newShipment::on_btn_delete_clicked()
{
    if(currentRowCount > 0)
    {
        currentRowCount--;
        model->removeRow(ui->tableView_display->currentIndex().row());
        for(int i = 0; i < currentRowCount; ++i)
        {
            model->setHeaderData(i, Qt::Vertical, QString('1'+i));
        }
    }
}

void Seller_newShipment::on_btn_submit_clicked()
{
    if(model->rowCount() > 0){
        DBHelper *helper = DBHelper::getInstance();
        helper->connectDatabase();
        QSqlQuery query;

        if(isEditMode == 1)
        {
            // delete all old puchaselog
            for(int i = 0; i < oldShipmentLogID.size(); ++i)
            {
                qDebug() << "deleteing old shipmentlog " << oldShipmentLogID.at(i);
                QString str_query = "delete from SHIPMENT_LOG where shipmentLogID = :oldID;";
                query.prepare(str_query);
                query.bindValue(":oldID", oldShipmentLogID.at(i).toInt());
                query.exec();
            }
        }

        query.prepare("select MAX(shipmentID) from SHIPMENT;");
        query.exec();
        int int_shipmentID = 0;
        while(query.next())
        {
            int_shipmentID = query.value(0).toInt() + 1;
        }
        QDateTime time = QDateTime::currentDateTime();
        QString str_time = time.toString("yyyy-MM-dd hh:mm:ss");
        QString str_query;
        if(isEditMode == 0)
        {
            qDebug() << "Inserting!";
            str_query = "insert into SHIPMENT values(:shipmentID, :sellerID, :time, :status, NULL);";
            query.prepare(str_query);
            query.bindValue(":shipmentID", int_shipmentID);
            query.bindValue(":sellerID", sellerID);
            query.bindValue(":status", 0);
            query.bindValue(":time", str_time);
            query.exec();
        }
        else
        {
            QDateTime time = QDateTime::currentDateTime();
            QString str_time = time.toString("yyyy-MM-dd hh:mm:ss");
            str_query = "update SHIPMENT set status = '0', adminID = null, time = :time where shipmentID = :shipmentID";
            query.prepare(str_query);
            query.bindValue(":shipmentID", editModeShipmentID);
            query.bindValue(":time", str_time);
            query.exec();
        }

        query.prepare("select MAX(shipmentLogID) from SHIPMENT_LOG;");
        query.exec();
        int int_shipmentLogID = 0;
        while(query.next())
        {
            int_shipmentLogID = query.value(0).toInt() + 1;
        }

        for(int i = 0; i < model->rowCount() ; ++i)
        {
            int goods_id = model->item(i, 0)->text().toInt();
            QString goods_name = model->item(i, 1)->text();
            int goods_num = model->item(i, 2)->text().toInt();

            str_query = "insert into SHIPMENT_LOG values(:shipmentLogID, :shipmentID, :goodsID, :goodsName, :num);";
            query.prepare(str_query);
            query.bindValue(":shipmentLogID", int_shipmentLogID++);
            if(isEditMode == 0)
                query.bindValue(":shipmentID", int_shipmentID);
            else
                query.bindValue(":shipmentID", editModeShipmentID);
            query.bindValue(":goodsID", goods_id);
            query.bindValue(":goodsName", goods_name);
            query.bindValue(":num", goods_num);
            query.exec();
        }
        QMessageBox::about(this, "成功", "已提交出货单");
        ui->btn_add->setDisabled(true);
        ui->btn_delete->setDisabled(true);
        ui->btn_submit->setDisabled(true);
        ui->label_info->setText("提交成功，请返回");
    }
    else{
        QMessageBox::critical(this,"警告","不允许提交空记录");
        return;
    }
}

void Seller_newShipment::setEditModeAttributes(int shipmentID)
{
    this->isEditMode = 1;
    this->editModeShipmentID = shipmentID;

    currentRowCount = 0;

    DBHelper *helper = DBHelper::getInstance();
    helper->connectDatabase();

    QString str_query;
    QSqlQuery query;

    str_query = "select * from SHIPMENT_LOG where shipmentID = :shipmentID;";
    query.prepare(str_query);
    query.bindValue(":shipmentID", shipmentID);
    query.exec();
    // QStringList oldShipmentLogID;
    while(query.next())
    {
        QString str_shipmentLogID = query.value(0).toString();
        oldShipmentLogID << str_shipmentLogID;
        QString str_goodsID = query.value(2).toString();
        model->setItem(currentRowCount, 0, new QStandardItem(str_goodsID));
        QString str_goodsName = query.value(3).toString();
        model->setItem(currentRowCount, 1, new QStandardItem(str_goodsName));
        QString str_num = query.value(4).toString();
        model->setItem(currentRowCount, 2, new QStandardItem(str_num));

        currentRowCount++;
    }

    helper->disconnectDatabase();
}
