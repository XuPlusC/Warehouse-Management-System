#include "admin_goodsedit.h"
#include "ui_admin_goodsedit.h"
#include "dbhelper.h"
#include <QSqlQuery>

Admin_goodsEdit::Admin_goodsEdit(
        QString goodsID,
        QString goodsName,
        QString storageNum,
        QString place,
        QString producer,
        QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Admin_goodsEdit)
{
    this->goodsID = goodsID;
    this->goodsName = goodsName;
    this->storageNum = storageNum;
    this->place = place;
    this->producer = producer;

    ui->setupUi(this);
    ui->label_goodsID->setText("修改货品" + goodsID + "号：");
    ui->lineEdit_goodsName->setText(goodsName);
    ui->lineEdit_storageNum->setText(storageNum);
    ui->lineEdit_place->setText(place);
    ui->lineEdit_producer->setText(producer);
}

Admin_goodsEdit::~Admin_goodsEdit()
{
    delete ui;
}


void Admin_goodsEdit::on_btn_submit_clicked()
{
    DBHelper *helper = DBHelper::getInstance();
    helper->connectDatabase();
    QSqlQuery query;
    QString str_query;

    if(isInsertMode == 0)
    {
        str_query = "update GOODS SET goodsName = :goodsName, storageNum = :storageNum, place = :place, producer = :producer where goodsID = :goodsID";
        query.prepare(str_query);
        query.bindValue(":goodsName", ui->lineEdit_goodsName->text());
        query.bindValue(":storageNum", ui->lineEdit_storageNum->text().toInt());
        query.bindValue(":place", ui->lineEdit_place->text());
        query.bindValue(":producer", ui->lineEdit_producer->text());
        query.bindValue(":goodsID", goodsID.toInt());
        query.exec();
    }
    else
    {
        str_query = "insert into GOODS values(:goodsID, :goodsName, :storageNum, :place, :producer);";
        query.prepare(str_query);
        query.bindValue(":goodsID", goodsID.toInt());
        query.bindValue(":goodsName", ui->lineEdit_goodsName->text());
        query.bindValue(":storageNum", ui->lineEdit_storageNum->text().toInt());
        query.bindValue(":place", ui->lineEdit_place->text());
        query.bindValue(":producer", ui->lineEdit_producer->text());
        query.exec();
    }

    helper->disconnectDatabase();
    emit(closed());
}



void Admin_goodsEdit::on_btn_back_clicked()
{
    emit(closed());
}


void Admin_goodsEdit::setInsertMode()
{
    this->isInsertMode = 1;
    ui->label_goodsID->setText("添加货品" + this->goodsID + "号：");
}
