#include "seller_newshipmentlog.h"
#include "ui_seller_newshipmentlog.h"

Seller_newShipmentLog::Seller_newShipmentLog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Seller_newShipmentLog)
{
    DBHelper *helper = DBHelper::getInstance();
    helper->connectDatabase();
    ui->setupUi(this);

    ui->comboBox_goodsName->clear();
    QSqlQuery query;
    query.prepare("select * from GOODS");
    query.exec();
    QStringList items;
    while(query.next()){
        items << query.value(1).toString().trimmed();
    }
    qDebug() << "found goods" << items;
    ui->comboBox_goodsName->addItems(items);

//    helper->disconnectDatabase();
}

Seller_newShipmentLog::~Seller_newShipmentLog()
{
    delete ui;
}

void Seller_newShipmentLog::on_btn_submit_clicked()
{
    if(ui->comboBox_goodsID->currentText().isEmpty()){
        QMessageBox::critical(this,"警告","货品编号不允许为空");
        return;
    }
    if(ui->lineEdit_shipmentNum->text().isEmpty()){
        QMessageBox::critical(this,"警告","出货数量不允许为空");
        return;
    }

    QString goods_name = ui->comboBox_goodsName->currentText();
    QString goods_id = ui->comboBox_goodsID->currentText();
    QString goods_num = ui->lineEdit_shipmentNum->text();

    emit newShipmentLogSubmit(goods_name, goods_num, goods_id);
}

void Seller_newShipmentLog::on_btn_cancel_clicked()
{
    emit(cancelled());
}

void Seller_newShipmentLog::on_comboBox_goodsName_currentTextChanged(const QString &arg1)
{
    ui->comboBox_goodsID->clear();

    DBHelper *helper = DBHelper::getInstance();
    QSqlQuery query;
    helper->connectDatabase();
    query.prepare("select * from GOODS where goodsName = :name;");
    query.bindValue(":name", arg1);
    query.exec();

    QStringList items;
    while(query.next()){
        items << query.value(0).toString().trimmed();
    }
    ui->comboBox_goodsID->addItems(items);

    helper->disconnectDatabase();
    return;
}

void Seller_newShipmentLog::on_comboBox_goodsID_currentTextChanged(const QString &arg1)
{
    int goods_id = arg1.toInt();
    DBHelper *helper = DBHelper::getInstance();
    QSqlQuery query;
    helper->connectDatabase();
    query.prepare("select * from GOODS where goodsID = :id;");
    query.bindValue(":id", goods_id);
    query.exec();

    QString place;
    QString producer;
    QString storage_num;
    while(query.next()){
        place = query.value(3).toString().trimmed();
        producer = query.value(4).toString().trimmed();
        storage_num = query.value(2).toString().trimmed();
    }
    ui->lineEdit_place->setText(place);
    ui->lineEdit_storageNum->setText(storage_num);
    ui->lineEdit_producer->setText(producer);
    ui->lineEdit_place->setReadOnly(true);
    ui->lineEdit_storageNum->setReadOnly(true);
    ui->lineEdit_producer->setReadOnly(true);
}
