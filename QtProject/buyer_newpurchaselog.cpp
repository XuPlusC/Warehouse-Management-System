#include "buyer_newpurchaselog.h"
#include "ui_buyer_newpurchaselog.h"

Buyer_NewPurchaseLog::Buyer_NewPurchaseLog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Buyer_NewPurchaseLog)
{
    DBHelper *helper = DBHelper::getInstance();
    helper->connectDatabase();
    ui->setupUi(this);

    ui->goods_name->clear();
    QSqlQuery query;
    query.prepare("select * from GOODS");
    query.exec();
    QStringList items;
    while(query.next()){
        items << query.value(1).toString().trimmed();
    }
    ui->goods_name->addItems(items);
}

Buyer_NewPurchaseLog::~Buyer_NewPurchaseLog()
{
    delete ui;
}

void Buyer_NewPurchaseLog::on_goods_name_currentTextChanged(const QString &arg1)
{
    ui->goods_id->clear();

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
    ui->goods_id->addItems(items);
    return;
}

void Buyer_NewPurchaseLog::on_goods_id_currentTextChanged(const QString &arg1)
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
    ui->place->setText(place);
    ui->storage_num->setText(storage_num);
    ui->producer->setText(producer);
    ui->place->setReadOnly(true);
    ui->storage_num->setReadOnly(true);
    ui->producer->setReadOnly(true);
}

void Buyer_NewPurchaseLog::on_affirm_clicked()
{
    if(ui->goods_id->currentText().isEmpty()){
        QMessageBox::critical(this,"警告","货物id不允许为空");
        return;
    }
    if(ui->goods_num->text().isEmpty()){
        QMessageBox::critical(this,"警告","货物数量不允许为空");
        return;
    }

    QString goods_name = ui->goods_name->currentText();
    QString goods_id = ui->goods_id->currentText();
    QString goods_num = ui->goods_num->text();

    emit newpurchaselogAffirm(goods_name, goods_num, goods_id);
}

void Buyer_NewPurchaseLog::on_btn_cancel_clicked()
{
    emit(cancelled());
}
