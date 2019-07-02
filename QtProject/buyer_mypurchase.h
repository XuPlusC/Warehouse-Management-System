#ifndef BUYER_MYPURCHASE_H
#define BUYER_MYPURCHASE_H

#include "dbhelper.h"
#include "buyer_newpurchase.h"
#include "seller_newshipment.h"

#include <QDebug>
#include <QDialog>
#include <QPushButton>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QString>
#include <QSqlQuery>

namespace Ui {
class Buyer_MyPurchase;
}

class Buyer_MyPurchase : public QDialog
{
    Q_OBJECT

public:
    explicit Buyer_MyPurchase(int type, int userID, QWidget *parent = nullptr);
    ~Buyer_MyPurchase();

signals:
    void backToMain();

private slots:
    void on_back_edit_purchase();
    void on_back_edit_shipment();

    void on_btn_back_clicked();
    void on_btn_edit_clicked();

private:
    Ui::Buyer_MyPurchase *ui;    
    int currentRowCount = 0;
    int type;
    int userID;

    QPushButton *btn_edit = nullptr;
    QStandardItemModel *model = nullptr;

    Buyer_NewPurchase *edit_purchase = nullptr;
    Seller_newShipment *edit_shipment = nullptr;
    void updateTable();
};

#endif // BUYER_MYPURCHASE_H
