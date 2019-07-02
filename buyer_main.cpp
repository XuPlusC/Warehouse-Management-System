#include "buyer_main.h"
#include "ui_buyer_main.h"
#define TYPE_PURCHASE 0

Buyer_Main::Buyer_Main(int buyerID, QString buyerName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Buyer_Main)
{
    this->buyerID = buyerID;
    this->buyerName = buyerName;
    ui->setupUi(this);
    ui->label_welcome->setText("欢迎，" + buyerName + "!");
}

Buyer_Main::~Buyer_Main()
{
    delete ui;
}

void Buyer_Main::on_btn_back_clicked()
{
    emit(backToMain());
}

void Buyer_Main::on_back_newPurchase()
{
    buyer_newPurchase->hide();
    this->show();
}

void Buyer_Main::on_btn_newPurchase_clicked()
{
    if(buyer_newPurchase != nullptr)
        delete buyer_newPurchase;
    buyer_newPurchase = new Buyer_NewPurchase(buyerID, this);
    QObject::connect(buyer_newPurchase, SIGNAL(backToMain()), this, SLOT(on_back_newPurchase()));
    buyer_newPurchase->show();
    this->hide();
}

void Buyer_Main::on_back_checkLackLog()
{
    buyer_checkLackLog->hide();
    this->show();
}

void Buyer_Main::on_btn_checkLackLog_clicked()
{
    if(buyer_checkLackLog != nullptr)
        delete buyer_checkLackLog;
    buyer_checkLackLog = new Buyer_CheckLackLog(this);
    QObject::connect(buyer_checkLackLog, SIGNAL(backToMain()), this, SLOT(on_back_checkLackLog()));
    buyer_checkLackLog->show();
    this->hide();
}

void Buyer_Main::on_back_myPurchase()
{
    buyer_myPurchase->hide();
    this->show();
}

void Buyer_Main::on_btn_myPurchase_clicked()
{
    if(buyer_myPurchase != nullptr)
        delete buyer_myPurchase;
    buyer_myPurchase = new Buyer_MyPurchase(TYPE_PURCHASE, buyerID, this);
    QObject::connect(buyer_myPurchase, SIGNAL(backToMain()), this, SLOT(on_back_myPurchase()));
    buyer_myPurchase->show();
    this->hide();
}
