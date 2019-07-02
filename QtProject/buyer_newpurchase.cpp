#include "buyer_newpurchase.h"
#include "ui_buyer_newpurchase.h"
#include <QDateTime>
#include <QDebug>
#include <QMessageBox>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QSqlQuery>

Buyer_NewPurchase::Buyer_NewPurchase(int buyerID, QWidget *parent) :
    QDialog (parent),
    ui(new Ui::Buyer_NewPurchase)
{
    this->buyerID = buyerID;

    ui->setupUi(this);

    model= new QStandardItemModel();

    /*设置列字段名*/
    model->setColumnCount(3);
    model->setHeaderData(0, Qt::Horizontal, "货品编号");
    model->setHeaderData(1, Qt::Horizontal, "货品名称");
    model->setHeaderData(2, Qt::Horizontal, "货品数量");

    currentRowCount = 0;
    ui->tableView_display->setModel(model);
    ui->tableView_display->setEditTriggers(QAbstractItemView::NoEditTriggers);  // dont allow to edit the table
    ui->tableView_display->resizeColumnsToContents();
}

Buyer_NewPurchase::~Buyer_NewPurchase()
{
    delete ui;
}

void Buyer_NewPurchase::on_btn_back_clicked()
{
    emit(backToMain());
}


void Buyer_NewPurchase::on_btn_submit_clicked()
{
    if(model->rowCount() > 0){
        DBHelper *helper = DBHelper::getInstance();
        helper->connectDatabase();
        QSqlQuery query;

        if(iSEditMode == 1)
        {
            // delete all old puchaselog
            for(int i = 0; i < oldPurchaseLogID.size(); ++i)
            {
                qDebug() << "deleteing old puchaselog " << oldPurchaseLogID.at(i);
                QString str_query = "delete from PURCHASE_LOG where purchaseLogID = :oldID;";
                query.prepare(str_query);
                query.bindValue(":oldID", oldPurchaseLogID.at(i).toInt());
                query.exec();
            }
        }

        query.prepare("select MAX(purchaseID) from PURCHASE;");
        query.exec();
        int purchase_id = 0;
        while(query.next())
        {
            purchase_id = query.value(0).toInt() + 1;
        }
        QDateTime time = QDateTime::currentDateTime();
        QString str_time = time.toString("yyyy-MM-dd hh:mm:ss");
        QString str_query;
        if(iSEditMode == 0)
        {
            qDebug() << "Inserting!";
            str_query = "insert into PURCHASE values(:purchaseID, :buyerID, :time, :status, NULL);";
            query.prepare(str_query);

            query.bindValue(":purchaseID", purchase_id);
            query.bindValue(":buyerID", buyerID);
            query.bindValue(":status", 0);
            query.bindValue(":time", str_time);
            query.exec();
        }
        else
        {
            QDateTime time = QDateTime::currentDateTime();
            QString str_time = time.toString("yyyy-MM-dd hh:mm:ss");
            str_query = "update PURCHASE set status = '0', adminID = null, time = :time where purchaseID = :purchaseID";
            query.prepare(str_query);
            query.bindValue(":purchaseID", editModePurchaseID);
            query.bindValue(":time", str_time);
            query.exec();
        }

        query.prepare("select MAX(purchaseLogID) from PURCHASE_LOG;");
        query.exec();
        int purchaselog_id = 1;
        while(query.next())
        {
            purchaselog_id = query.value(0).toInt() + 1;
        }

        for(int i = 0; i < model->rowCount() ; ++i)
        {
            int goods_id = model->item(i, 0)->text().toInt();
            QString goods_name = model->item(i, 1)->text();
            int goods_num = model->item(i, 2)->text().toInt();

            str_query = "insert into PURCHASE_LOG values(:purchaseLogID, :purchaseID, :goodsID, :goodsName, :num);";
            query.prepare(str_query);
            query.bindValue(":purchaseLogID", purchaselog_id++);
            if(iSEditMode == 0)
                query.bindValue(":purchaseID", purchase_id);
            else
                query.bindValue(":purchaseID", editModePurchaseID);
            query.bindValue(":goodsID", goods_id);
            query.bindValue(":goodsName", goods_name);
            query.bindValue(":num", goods_num);
            query.exec();
        }
        QMessageBox::about(this, "成功", "已提交采购单");
        ui->btn_addrow->setDisabled(true);
        ui->btn_deleterow->setDisabled(true);
        ui->btn_submit->setDisabled(true);
        ui->label_info->setText("提交成功，请返回");
    }
    else{
        QMessageBox::critical(this,"警告","不允许提交空记录");
        return;
    }
}

void Buyer_NewPurchase::on_newpurchaselogAffirm(QString goods_name, QString goods_num, QString goods_id)
{
    newPurchaseLog->hide();
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

void Buyer_NewPurchase::on_newpurchaselogCancel()
{
    newPurchaseLog->hide();
    model->setRowCount(--currentRowCount);
}

void Buyer_NewPurchase::on_btn_addrow_clicked()
{
    model->setRowCount(++currentRowCount);
    if(newPurchaseLog != nullptr)
        delete newPurchaseLog;
    newPurchaseLog = new Buyer_NewPurchaseLog();
    QObject::connect(newPurchaseLog, SIGNAL(newpurchaselogAffirm(QString, QString, QString)), this, SLOT(on_newpurchaselogAffirm(QString, QString, QString)));
    QObject::connect(newPurchaseLog, SIGNAL(cancelled()), this, SLOT(on_newpurchaselogCancel()));
    newPurchaseLog->show();
}

void Buyer_NewPurchase::on_btn_deleterow_clicked()
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

void Buyer_NewPurchase::setEditModeAttribute(int userID, int purchaseID)
{
    iSEditMode = 1;
    this->editModeUserID = userID;
    this->editModePurchaseID = purchaseID;

    currentRowCount = 0;

    DBHelper *helper = DBHelper::getInstance();
    helper->connectDatabase();

    QString str_query;
    QSqlQuery query;

    str_query = "select * from PURCHASE_LOG where purchaseID = :purchaseID;";
    query.prepare(str_query);
    query.bindValue(":purchaseID", purchaseID);
    query.exec();
//    QStringList oldPurchaseLogID;
    oldPurchaseLogID.clear();
    while(query.next())
    {
        QString str_purchaseLogID = query.value(0).toString();
        oldPurchaseLogID << str_purchaseLogID;
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
