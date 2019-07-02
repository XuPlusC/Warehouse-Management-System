#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dbhelper.h"
#include "admin_check.h"
#include "buyer_checklacklog.h"
#include "testdialog.h"

#include <QDebug>
#include <QImage>
#include <QMessageBox>
#include <QMovie>
#include <QPixmap>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->comboBox_type->addItem("管理员");
    ui->comboBox_type->addItem("销售员");
    ui->comboBox_type->addItem("采购员");

    QImage *img = new QImage();
    img->load(":/background.jpg");
    ui->label_picture->setPixmap(QPixmap::fromImage(*img));

//    movie = new QMovie(":/erect.gif");
//    ui->label_picture->setMovie(movie);
//    movie->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// slots
void MainWindow::on_backToMain_adminCheck()
{
    admin_check->hide();
    this->show();
}

void MainWindow::on_btn_adminCheck_clicked()
{
    this->hide();
    admin_check = new Admin_Check(this);
    QObject::connect(admin_check, SIGNAL(backToMain()), this, SLOT(on_backToMain_adminCheck()));
    admin_check->setAdminID(1); // need to be changed
    admin_check->show();
}

void MainWindow::on_backToMain_newPurchase()
{
    buyer_newpurchase->hide();
    this->show();
}

void MainWindow::on_btn_newPurchase_clicked()
{
    this->hide();
    // testDialog = new TestDialog(this);
    buyer_newpurchase = new Buyer_NewPurchase(1, this);
    QObject::connect(buyer_newpurchase, SIGNAL(backToMain()), this, SLOT(on_backToMain_newPurchase()));
    buyer_newpurchase->show();
}

void MainWindow::on_btn_login_clicked()
{
    if(ui->lineEdit_id->text().isEmpty()){
        QMessageBox::critical(this,"警告","用户名不允许为空");
        return;
    }
    if(ui->lineEdit_password->text().isEmpty()){
        QMessageBox::critical(this,"警告","密码不允许为空");
        return;
    }

    DBHelper *dbhelper = DBHelper::getInstance();
    dbhelper->connectDatabase();
    QSqlQuery query;

    switch (ui->comboBox_type->currentIndex())
    {
    case 0:
        query.prepare("select * from ADMIN");
        break;
    case 1:
        query.prepare("select * from SELLER");
        break;
    case 2:
        query.prepare("select * from BUYER");
        break;
    }
    query.exec();

    int loginStatus = 0;
    QString loginName;
    while(query.next())
    {
        qDebug()<<query.value(1).toString();
        if(QString::compare(query.value(0).toString(), ui->lineEdit_id->text()) == 0)
        {
            if(QString::compare(query.value(2).toString(), ui->lineEdit_password->text()) == 0)
            {
                loginName = query.value(1).toString();
                qDebug() << "login success! welcome " << loginName;
                loginStatus = 1;
                break;
            }
            else
            {
                qDebug() << "wrong password!";
                loginStatus = 2;
                break;
            }
        }
    }
    switch (loginStatus)
    {
    case 0:
        QMessageBox::critical(this,"警告","找不到用户名");
        break;
    case 2:
        QMessageBox::critical(this,"警告","密码错误");
        break;
    case 1:
        if(ui->comboBox_type->currentIndex() == 0)  // admin login
        {
            if(admin_main != nullptr)
                delete admin_main;
            admin_main = new Admin_Main(loginName.trimmed(), ui->lineEdit_id->text().toInt(), this);
            QObject::connect(admin_main, SIGNAL(backToMain()), this, SLOT(on_backToMain_adminMain()));
            admin_main->show();
            this->hide();
        }
        if(ui->comboBox_type->currentIndex() == 1)  // seller login
        {
            if(seller_main != nullptr)
                delete seller_main;
            seller_main = new Seller_Main(ui->lineEdit_id->text().toInt(), loginName.trimmed(), this);
            QObject::connect(seller_main, SIGNAL(backToMain()), this, SLOT(on_backToMain_sellerMain()));
            seller_main->show();
            this->hide();
        }
        if(ui->comboBox_type->currentIndex() == 2)  // buyer login
        {
            if(buyer_main != nullptr)
                delete buyer_main;
            buyer_main = new Buyer_Main(ui->lineEdit_id->text().toInt(), loginName.trimmed(), this);
            QObject::connect(buyer_main, SIGNAL(backToMain()), this, SLOT(on_backToMain_buyerMain()));
            buyer_main->show();
            this->hide();
        }
        break;
    }
    dbhelper->disconnectDatabase();
}

void MainWindow::on_backToMain_buyerCheckLackLog()
{
    buyer_checkLackLog->hide();
    this->show();
}

void MainWindow::on_btn_checkLackLog_clicked()
{
    if(buyer_checkLackLog != NULL)
    {
        delete buyer_checkLackLog;
    }
    buyer_checkLackLog = new Buyer_CheckLackLog(this);
    QObject::connect(buyer_checkLackLog, SIGNAL(backToMain()), this, SLOT(on_backToMain_buyerCheckLackLog()));
    this->hide();
    buyer_checkLackLog->show();
    buyer_checkLackLog->setFocus();
}

void MainWindow::on_backToMain_adminMain()
{
    admin_main->hide();
    this->show();
    ui->lineEdit_id->clear();
    ui->lineEdit_password->clear();
}

void MainWindow::on_backToMain_buyerMain()
{
    buyer_main->hide();
    this->show();
    ui->lineEdit_id->clear();
    ui->lineEdit_password->clear();
}

void MainWindow::on_backToMain_sellerMain()
{
    seller_main->hide();
    this->show();
    ui->lineEdit_id->clear();
    ui->lineEdit_password->clear();
}


void MainWindow::on_backToMain_adminGoodsManagement()
{
    admin_goodsmanagement->hide();
    this->show();
}

void MainWindow::on_btn_goodsManage_clicked()
{
    if(admin_goodsmanagement != NULL)
        delete admin_goodsmanagement;

    admin_goodsmanagement = new admin_goodsManagement(this);
    QObject::connect(admin_goodsmanagement, SIGNAL(backToMain()), this, SLOT(on_backToMain_adminGoodsManagement()));
    admin_goodsmanagement->show();
    this->hide();
}

void MainWindow::on_lineEdit_password_textChanged(const QString &arg1)
{
//    if(isMoviewStarted == 0)
//    {
//        movie->start();
//        isMoviewStarted = 1;
//    }
}
