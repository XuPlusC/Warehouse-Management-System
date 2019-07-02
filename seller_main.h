#ifndef SELLER_MAIN_H
#define SELLER_MAIN_H

#include "buyer_checklacklog.h"
#include "buyer_mypurchase.h"
#include "seller_newshipment.h"

#include <QDialog>
#include <QString>

namespace Ui {
class Seller_Main;
}

class Seller_Main : public QDialog
{
    Q_OBJECT

public:
    explicit Seller_Main(int sellerID, QString sellerName, QWidget *parent = nullptr);
    ~Seller_Main();

signals:
    void backToMain();

private slots:
    void on_back_newShipment();
    void on_back_checkLackLog();
    void on_back_myShipment();

    void on_btn_back_clicked();
    void on_btn_newShipment_clicked();
    void on_btn_checkLackLog_clicked();
    void on_btn_myShipment_clicked();

private:
    Ui::Seller_Main *ui;
    int sellerID;
    QString sellerName;

    Seller_newShipment *seller_newShipment = nullptr;
    Buyer_CheckLackLog *seller_checkLackLog = nullptr;
    Buyer_MyPurchase *seller_myShipment = nullptr;
};

#endif // SELLER_MAIN_H
