#ifndef ADMIN_GOODSMANAGEMENT_H
#define ADMIN_GOODSMANAGEMENT_H
#include "admin_goodsedit.h"
#include <QDialog>
#include <QPushButton>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QString>

namespace Ui {
class admin_goodsManagement;
}

class admin_goodsManagement : public QDialog
{
    Q_OBJECT

public:
    explicit admin_goodsManagement(QWidget *parent = nullptr);
    ~admin_goodsManagement();

signals:
    void backToMain();

private slots:
    void on_btn_back_clicked();
    void on_btn_edit_clicked();
    void on_edit_closed();
    void on_btn_refresh_clicked();

    void on_btn_add_clicked();

private:
    Ui::admin_goodsManagement *ui;
    QPushButton *btn_edit = NULL;
    QStandardItemModel *model = NULL;
    Admin_goodsEdit *editDialog = NULL;
    int currentRowCount = 0;
    void updateTable();
};

#endif // ADMIN_GOODSMANAGEMENT_H
