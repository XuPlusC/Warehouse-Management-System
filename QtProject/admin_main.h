#ifndef ADMIN_MAIN_H
#define ADMIN_MAIN_H

#include "admin_goodsmanagement.h"
#include "buyer_checklacklog.h"
#include "admin_check.h"
#include <QString>
#include <QMainWindow>

namespace Ui {
class Admin_Main;
}

class Admin_Main : public QMainWindow
{
    Q_OBJECT

public:
    explicit Admin_Main(QString adminName, int adminID, QWidget *parent = nullptr);
    ~Admin_Main();

private slots:
    void on_back_goodsmanagement();
    void on_back_checklacklog();
    void on_back_check();

    void on_btn_back_clicked();
    void on_btn_changeGoods_clicked();
    void on_btn_checkLackLog_clicked();


    void on_btn_check_clicked();

signals:
    void backToMain();

private:
    Ui::Admin_Main *ui;
    QString adminName;
    int adminID;

    admin_goodsManagement *admin_goodsmanagement = nullptr;
    Admin_Check *admin_check = nullptr;
    Buyer_CheckLackLog *admin_checkLackLog = nullptr;
};

#endif // ADMIN_MAIN_H
