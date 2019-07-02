#ifndef BUYER_MAIN_H
#define BUYER_MAIN_H

#include "buyer_newpurchase.h"
#include "buyer_checklacklog.h"
#include "buyer_mypurchase.h"
#include <QDialog>
#include <QString>

namespace Ui {
class Buyer_Main;
}

class Buyer_Main : public QDialog
{
    Q_OBJECT

public:
    explicit Buyer_Main(int buyerID, QString buyerName, QWidget *parent = nullptr);
    ~Buyer_Main();

signals:
    void backToMain();

private slots:
    void on_back_newPurchase();
    void on_back_checkLackLog();
    void on_back_myPurchase();

    void on_btn_back_clicked();
    void on_btn_newPurchase_clicked();

    void on_btn_checkLackLog_clicked();

    void on_btn_myPurchase_clicked();

private:
    Ui::Buyer_Main *ui;
    int buyerID;
    QString buyerName;

    Buyer_NewPurchase *buyer_newPurchase = nullptr;
    Buyer_CheckLackLog *buyer_checkLackLog = nullptr;
    Buyer_MyPurchase *buyer_myPurchase = nullptr;
};

#endif // BUYER_MAIN_H
