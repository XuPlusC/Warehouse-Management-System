#ifndef BUYER_NEWPURCHASE_H
#define BUYER_NEWPURCHASE_H

#include "buyer_newpurchaselog.h"

#include <QDialog>
#include <QStandardItemModel>
#include <QString>

namespace Ui {
class Buyer_NewPurchase;
}

class Buyer_NewPurchase : public QDialog
{
    Q_OBJECT

public:
    explicit Buyer_NewPurchase(int buyerID, QWidget *parent = nullptr);
    ~Buyer_NewPurchase();
    void setEditModeAttribute(int userID, int purchaseID);

signals:
    void backToMain();

private slots:
    void on_newpurchaselogAffirm(QString goods_name, QString goods_num, QString goods_id);
    void on_newpurchaselogCancel();

    void on_btn_back_clicked();

    void on_btn_submit_clicked();

    void on_btn_addrow_clicked();

    void on_btn_deleterow_clicked();

private:
    Ui::Buyer_NewPurchase *ui;
    QStandardItemModel *model;
    Buyer_NewPurchaseLog *newPurchaseLog = nullptr;

    int currentRowCount;
    int buyerID;
    int iSEditMode = 0;
    int editModeUserID = 0;
    int editModePurchaseID = 0;
    QStringList oldPurchaseLogID;
};

#endif // BUYER_NEWPURCHASE_H
