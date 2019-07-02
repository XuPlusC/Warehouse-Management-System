#include "admin_main.h"
#include "ui_admin_main.h"

Admin_Main::Admin_Main(QString adminName, int adminID, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Admin_Main)
{
    this->adminName = adminName;
    this->adminID = adminID;
    ui->setupUi(this);
    ui->label_welcome->setText("欢迎，" + adminName + "!");
}

Admin_Main::~Admin_Main()
{
    delete ui;
}

void Admin_Main::on_btn_back_clicked()
{
    emit(backToMain());
}

void Admin_Main::on_back_goodsmanagement()
{
    admin_goodsmanagement->hide();
    this->show();
    delete admin_goodsmanagement;
}

void Admin_Main::on_btn_changeGoods_clicked()
{
    admin_goodsmanagement = new admin_goodsManagement(this);
    QObject::connect(admin_goodsmanagement, SIGNAL(backToMain()), this, SLOT(on_back_goodsmanagement()));
    admin_goodsmanagement->show();
    this->hide();
}

void Admin_Main::on_back_checklacklog()
{
    admin_checkLackLog->hide();
    this->show();
    delete admin_checkLackLog;
}

void Admin_Main::on_btn_checkLackLog_clicked()
{
    admin_checkLackLog = new Buyer_CheckLackLog(this);
    QObject::connect(admin_checkLackLog, SIGNAL(backToMain()), this, SLOT(on_back_checklacklog()));
    admin_checkLackLog->show();
    this->hide();
}

void Admin_Main::on_back_check()
{
    admin_check->hide();
    this->show();
    delete admin_check;
}

void Admin_Main::on_btn_check_clicked()
{
    admin_check = new Admin_Check(this);
    admin_check->setAdminID(this->adminID);
    QObject::connect(admin_check, SIGNAL(backToMain()), this, SLOT(on_back_check()));
    admin_check->show();
    this->hide();
}
