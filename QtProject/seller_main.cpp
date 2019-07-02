#include "seller_main.h"
#include "ui_seller_main.h"
#define TYPE_SHIPMENT 1

Seller_Main::Seller_Main(int sellerID, QString sellerName,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Seller_Main)
{
    this->sellerID = sellerID;
    this->sellerName = sellerName;
    ui->setupUi(this);
    ui->label_welcome->setText("欢迎，" + sellerName + "!");
}

Seller_Main::~Seller_Main()
{
    delete ui;
}

void Seller_Main::on_btn_back_clicked()
{
    emit(backToMain());
}

void Seller_Main::on_back_newShipment()
{
    seller_newShipment->hide();
    this->show();
}

void Seller_Main::on_btn_newShipment_clicked()
{
    if(seller_newShipment != nullptr)
        delete seller_newShipment;
    seller_newShipment = new Seller_newShipment(sellerID, this);
    QObject::connect(seller_newShipment, SIGNAL(backToMain()), this, SLOT(on_back_newShipment()));
    seller_newShipment->show();
    this->hide();
}

void Seller_Main::on_back_checkLackLog()
{
    seller_checkLackLog->hide();
    this->show();
}

void Seller_Main::on_btn_checkLackLog_clicked()
{
    if(seller_checkLackLog != nullptr)
        delete seller_checkLackLog;
    seller_checkLackLog = new Buyer_CheckLackLog(this);
    QObject::connect(seller_checkLackLog, SIGNAL(backToMain()), this, SLOT(on_back_checkLackLog()));
    seller_checkLackLog->show();
    this->hide();
}

void Seller_Main::on_back_myShipment()
{
    seller_myShipment->hide();
    this->show();
}

void Seller_Main::on_btn_myShipment_clicked()
{
    if(seller_myShipment != nullptr)
        delete seller_myShipment;
    seller_myShipment = new Buyer_MyPurchase(TYPE_SHIPMENT, sellerID, this);
    QObject::connect(seller_myShipment, SIGNAL(backToMain()), this, SLOT(on_back_myShipment()));
    seller_myShipment->show();
    this->hide();
}
