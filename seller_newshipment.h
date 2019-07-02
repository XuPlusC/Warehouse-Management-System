#ifndef SELLER_NEWSHIPMENT_H
#define SELLER_NEWSHIPMENT_H

#include "seller_newshipmentlog.h"

#include <QDialog>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QString>

namespace Ui {
class Seller_newShipment;
}

class Seller_newShipment : public QDialog
{
    Q_OBJECT

public:
    explicit Seller_newShipment(int sellerID, QWidget *parent = nullptr);
    ~Seller_newShipment();
    void setEditModeAttributes(int shipmentID);

signals:
    void backToMain();

private slots:
    void on_newShipmentLogSubmit(QString goods_name, QString goods_num, QString goods_id);
    void on_newshipmentlogCancel();

    void on_btn_back_clicked();

    void on_btn_add_clicked();

    void on_btn_delete_clicked();

    void on_btn_submit_clicked();

private:
    Ui::Seller_newShipment *ui;
    QStandardItemModel *model;

    Seller_newShipmentLog *newShipmentLog = nullptr;

    int currentRowCount = 0;
    int sellerID;
    int isEditMode = 0;
    int editModeShipmentID = 0;
    QStringList oldShipmentLogID;
};

#endif // SELLER_NEWSHIPMENT_H
