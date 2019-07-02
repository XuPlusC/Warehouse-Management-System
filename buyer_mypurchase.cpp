#include "buyer_mypurchase.h"
#include "ui_buyer_mypurchase.h"
#define TYPE_PURCHASE 0
#define TYPE_SHIPMENT 1

Buyer_MyPurchase::Buyer_MyPurchase(int type, int userID, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Buyer_MyPurchase)
{
    this->type = type;
    this->userID = userID;
    ui->setupUi(this);

    model = new QStandardItemModel();
    model->setColumnCount(5);
    if(type == TYPE_PURCHASE)
    {
        model->setHeaderData(0, Qt::Horizontal, "采购单编号");
    }
    else
    {
        model->setHeaderData(0, Qt::Horizontal, "出货单编号");
    }
    model->setHeaderData(1, Qt::Horizontal, "提交时间");
    model->setHeaderData(2, Qt::Horizontal, "状态");
    model->setHeaderData(3, Qt::Horizontal, "处理人");
    model->setHeaderData(4, Qt::Horizontal, "修改");

    ui->tableView_display->setModel(model);
    ui->tableView_display->setEditTriggers(QAbstractItemView::NoEditTriggers);
    updateTable();
}

Buyer_MyPurchase::~Buyer_MyPurchase()
{
    delete ui;
}

void Buyer_MyPurchase::on_btn_back_clicked()
{
    emit(backToMain());
}

void Buyer_MyPurchase::updateTable()
{
    model->removeRows(0, model->rowCount());
    currentRowCount = 0;

    DBHelper *helper = DBHelper::getInstance();
    helper->connectDatabase();

    QString str_query;
    QSqlQuery query;
    if(type == TYPE_PURCHASE)
    {
        qDebug() << "preparing checking my purchase for user" << userID;
        str_query= "select * from PURCHASE where buyerID = :userID;";
    }
    else
    {
        qDebug() << "preparing checking my shipment";
        str_query= "select * from SHIPMENT where sellerID = :userID;";
    }
    query.prepare(str_query);
    query.bindValue(":userID", userID);
    query.exec();
    while (query.next())
    {
        qDebug() << "found" << query.value(0).toString();
        // column 1, purchaseID/shipmentID
        QString str_listID = query.value(0).toString();
        model->setItem(currentRowCount, 0, new QStandardItem(str_listID));
        // column 2, datetime
        QString str_datetime = query.value(2).toString();
        model->setItem(currentRowCount, 1, new QStandardItem(str_datetime));
        // column 3, status
        int int_status = query.value(3).toInt();
        QString str_status;
        switch (int_status)
        {
        case 0: str_status = "未审核"; break;
        case 1: str_status = "审核未通过"; break;
        case 2: str_status = "审核通过"; break;
        case 3: str_status = "缺货挂起"; break;
        }
        model->setItem(currentRowCount, 2, new QStandardItem(str_status));
        // column 4, adminID
        QString str_adminID;
        if(int_status == 0)
            str_adminID = "无";
        else
            str_adminID = query.value(4).toString().trimmed();
        model->setItem(currentRowCount, 3, new QStandardItem(str_adminID));
        // column 5, button
        if(int_status == 2) // if passed, no button
        {
            QString str_noEdit = "不可修改";
            model->setItem(currentRowCount, 4, new QStandardItem(str_noEdit));
        }
        else
        {
            btn_edit = new QPushButton("修改");
            btn_edit->setProperty("type", type);
            btn_edit->setProperty("listID", str_listID.toInt());
            qDebug() << "setting btn" << btn_edit->property("listID").toString();
            QObject::connect(btn_edit, SIGNAL(clicked()), this, SLOT(on_btn_edit_clicked()));
            ui->tableView_display->setIndexWidget(model->index(currentRowCount, 4), btn_edit);
        }
        currentRowCount++;
    }

    helper->disconnectDatabase();
    ui->tableView_display->setModel(model);
    ui->tableView_display->resizeColumnsToContents();
    ui->tableView_display->setColumnWidth(4, ui->tableView_display->columnWidth(4) + 35);
}

void Buyer_MyPurchase::on_back_edit_purchase()
{
    edit_purchase->hide();
    this->show();
    updateTable();
}

void Buyer_MyPurchase::on_back_edit_shipment()
{
    edit_shipment->hide();
    this->show();
    updateTable();
}

void Buyer_MyPurchase::on_btn_edit_clicked()
{
    QPushButton *btn_temp = (QPushButton*) sender();
    qDebug() << "checking list " << btn_temp->property("listID").toInt();

    if(type == TYPE_PURCHASE)
    {
        if(edit_purchase != nullptr)
            delete edit_purchase;

        edit_purchase = new Buyer_NewPurchase(userID, this);
        edit_purchase->setEditModeAttribute(userID, btn_temp->property("listID").toInt());
        QObject::connect(edit_purchase, SIGNAL(backToMain()), this, SLOT(on_back_edit_purchase()));
        edit_purchase->show();
        this->hide();
    }
    else if (type == TYPE_SHIPMENT)
    {
        if(edit_shipment != nullptr)
            delete edit_shipment;

        edit_shipment = new Seller_newShipment(userID, this);
        edit_shipment->setEditModeAttributes(btn_temp->property("listID").toInt());
        QObject::connect(edit_shipment, SIGNAL(backToMain()), this, SLOT(on_back_edit_shipment()));
        edit_shipment->show();
        this->hide();
    }

//    if(editDialog != NULL)
//        delete editDialog;

//    editDialog = new Admin_goodsEdit(
//                btn_temp->property("goodsID").toString(),
//                btn_temp->property("goodsName").toString(),
//                btn_temp->property("storageNum").toString(),
//                btn_temp->property("place").toString(),
//                btn_temp->property("producer").toString(),
//                this);
//    QObject::connect(editDialog, SIGNAL(closed()), this, SLOT(on_edit_closed()));
//    editDialog->show();
}
