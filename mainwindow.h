#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "testdialog.h"
#include "admin_check.h"
#include "admin_goodsmanagement.h"
#include "admin_main.h"

#include "buyer_checklacklog.h"
#include "buyer_main.h"
#include "buyer_newpurchase.h"

#include "seller_main.h"

#include <QMainWindow>
#include <QMovie>
#include <QString>
#include <QtSql/QSqlDatabase>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btn_adminCheck_clicked();

    void on_backToMain_adminMain();
    void on_backToMain_adminCheck();
    void on_backToMain_adminGoodsManagement();

    void on_backToMain_buyerMain();
    void on_backToMain_newPurchase();
    void on_backToMain_buyerCheckLackLog();

    void on_backToMain_sellerMain();

    void on_btn_newPurchase_clicked();

    void on_btn_login_clicked();

    void on_btn_checkLackLog_clicked();

    void on_btn_goodsManage_clicked();

    void on_lineEdit_password_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    // TestDialog *testDialog;
    Admin_Main *admin_main = nullptr;
    Admin_Check *admin_check = NULL;
    admin_goodsManagement *admin_goodsmanagement = NULL;

    Buyer_Main *buyer_main = nullptr;
    Buyer_NewPurchase *buyer_newpurchase = NULL;
    Buyer_CheckLackLog *buyer_checkLackLog = NULL;

    Seller_Main *seller_main = nullptr;

    QMovie *movie;
    int isMoviewStarted = 0;
};

#endif // MAINWINDOW_H
