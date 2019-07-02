#ifndef ADMIN_CHECK_H
#define ADMIN_CHECK_H
#include "admin_detailandcheck.h"
#include <QDialog>
#include <QPushButton>
#include <QStandardItemModel>

namespace Ui {
class Admin_Check;
}

class Admin_Check : public QDialog
{
    Q_OBJECT

public:
    explicit Admin_Check(QWidget *parent = nullptr);
    ~Admin_Check();
    void setAdminID(int id);

signals:
    void backToMain();

private slots:
    void on_btn_back_clicked();
    void on_btn_refresh_clicked();
    void btn_detail_clicked();
    void detail_closed();

private:
    Ui::Admin_Check *ui;
    QPushButton *btn_detail;
    QStandardItemModel *model;
    int currentRowCount;
    admin_detailAndCheck *detailDialog;

    int adminID;
};

#endif // ADMIN_CHECK_H
