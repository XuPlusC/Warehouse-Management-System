#include "ui_admin_check.h"
#include "admin_check.h"
#include "admin_detailandcheck.h"
#include "dbhelper.h"
#include <QDebug>
#include <QPushButton>
#include <QSqlQuery>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QString>
#define TYPE_PURCHASE 0
#define TYPE_SHIPMENT 1

Admin_Check::Admin_Check(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Admin_Check)
{
    ui->setupUi(this);
    currentRowCount = 0;
    detailDialog = NULL;

    model= new QStandardItemModel();

    /*设置列字段名*/
    model->setColumnCount(5);
    model->setHeaderData(0, Qt::Horizontal, "类别");
    model->setHeaderData(1, Qt::Horizontal, "单号");
    model->setHeaderData(2, Qt::Horizontal, "审核状态");
    model->setHeaderData(3, Qt::Horizontal, "处理人工号");
    model->setHeaderData(4, Qt::Horizontal, "操作");
    ui->tableView_display->setModel(model);
}

Admin_Check::~Admin_Check()
{
    delete ui;
}

void Admin_Check::setAdminID(int id)
{
    this->adminID = id;
}

// slots
void Admin_Check::on_btn_back_clicked()
{
    emit(backToMain());
}

void Admin_Check::on_btn_refresh_clicked()
{
    // remove all rows, prepare to refresh
    model->removeRows(0, model->rowCount());
    currentRowCount = 0;

    DBHelper *helper = DBHelper::getInstance();
    helper->connectDatabase();
    QSqlQuery query;

    // if onlysuspend mode is on, filter all purchase log
    if(ui->checkBox_Purchase->isChecked() && !ui->checkBox_OnlySuspend->isChecked())
    {
        query.prepare("select * from PURCHASE;");
        query.exec();
        while(query.next())
        {
            // if onlyme mode is on, filter the logs which are not checked by the logged-in admin
            if(!query.value(4).isNull() && ui->checkBox_OnlyMe->isChecked())
            {
                int operateID = query.value(4).toInt();
                if(operateID != adminID)
                    continue;
            }

//            qDebug() << query.value(0).toString() << "\t" << query.value(2).toInt();
            // column 1, list type
            model->setItem(currentRowCount, 0, new QStandardItem("进货单"));
            // column 2, purchase id
            QString purchaseID = query.value(0).toString();
            model->setItem(currentRowCount, 1, new QStandardItem(purchaseID));
            // column 3, check status
            int status = query.value(3).toInt();
            QString status_display;
            switch (status)
            {
            case 0: status_display = "未审核"; break;
            case 1: status_display = "审核未通过"; break;
            case 2: status_display = "审核通过"; break;
            }
            model->setItem(currentRowCount, 2, new QStandardItem(status_display));
            // column 4, adminID
            QString ID;
            if(status == 0)
                ID = "无";
            else
                ID = query.value(4).toString();
            model->setItem(currentRowCount, 3, new QStandardItem(ID));
            // column 5, button
            btn_detail = new QPushButton("查看与审批");
            btn_detail->setProperty("listID", purchaseID);
            btn_detail->setProperty("type", TYPE_PURCHASE);
            btn_detail->setProperty("status", status);
            QObject::connect(btn_detail, SIGNAL(clicked(bool)), this, SLOT(btn_detail_clicked()));
            ui->tableView_display->setIndexWidget(model->index(currentRowCount, 4), btn_detail);
            currentRowCount++;
        }
    }

    if(ui->checkBox_shipment->isChecked())
    {
        query.prepare("select * from SHIPMENT;");
        query.exec();
        while(query.next())
        {
            // if onlyme mode is on, filter the logs which are not checked by the logged-in admin
            if(!query.value(4).isNull() && ui->checkBox_OnlyMe->isChecked())
            {
                int operateID = query.value(4).toInt();
                if(operateID != adminID)
                    continue;
            }
            // if onlysuspend mode is on, filter all logs that are not in suspending status
            if(ui->checkBox_OnlySuspend->isChecked() && query.value(3).toInt() != 3)
                continue;

//            qDebug() << query.value(0).toString() << "\t" << query.value(2).toInt();
            // column 1, list type
            model->setItem(currentRowCount, 0, new QStandardItem("出货单"));
            // column 2, shipment id
            QString shipmentID = query.value(0).toString();
            model->setItem(currentRowCount, 1, new QStandardItem(shipmentID));
            // column 3, check status
            int status = query.value(3).toInt();
            QString status_display;
            switch (status)
            {
            case 0: status_display = "未审核"; break;
            case 1: status_display = "审核未通过"; break;
            case 2: status_display = "审核通过"; break;
            case 3: status_display = "缺货挂起"; break;
            }
            model->setItem(currentRowCount, 2, new QStandardItem(status_display));
            // column 4, adminID
            QString ID;
            if(status == 0)
                ID = "无";
            else
                ID = query.value(4).toString();
            model->setItem(currentRowCount, 3, new QStandardItem(ID));
            // column 5, button
            btn_detail = new QPushButton("查看与审批");
            btn_detail->setProperty("listID", shipmentID);
            btn_detail->setProperty("type", TYPE_SHIPMENT);
            btn_detail->setProperty("status", status);
            QObject::connect(btn_detail, SIGNAL(clicked(bool)), this, SLOT(btn_detail_clicked()));
            ui->tableView_display->setIndexWidget(model->index(currentRowCount, 4), btn_detail);
            currentRowCount++;
        }
    }

    helper->disconnectDatabase();
    ui->tableView_display->setModel(model);
    ui->tableView_display->setEditTriggers(QAbstractItemView::NoEditTriggers);  // dont allow to edit the table
}

void Admin_Check::btn_detail_clicked()
{
    if(detailDialog != NULL)
        delete detailDialog;    // allow one detail dialog at most to exist

    QPushButton *btn_temp = (QPushButton*)sender();
//    qDebug() << "checking status for type:" << btn_temp->property("type").toInt()
//             << ", purchaseID:" << btn_temp->property("purchaseID").toString();
    detailDialog = new admin_detailAndCheck(this);
    detailDialog->setAttributes(btn_temp->property("type").toInt(),
                                btn_temp->property("listID").toInt(),
                                btn_temp->property("status").toInt());
    detailDialog->setAdminID(this->adminID);
    // prevent from memory leaking
    QObject::connect(detailDialog, SIGNAL(closed()), this, SLOT(detail_closed()));
    detailDialog->show();
}

void Admin_Check::detail_closed()
{
    delete detailDialog;    // prevent from memory leaking
    detailDialog = NULL;
    ui->btn_refresh->click(); // refresh the tableview
}
