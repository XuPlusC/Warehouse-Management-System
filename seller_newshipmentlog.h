#ifndef SELLER_NEWSHIPMENTLOG_H
#define SELLER_NEWSHIPMENTLOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QDebug>
#include <QTextCodec>
#include <QMessageBox>
#include "dbhelper.h"

namespace Ui {
class Seller_newShipmentLog;
}

class Seller_newShipmentLog : public QDialog
{
    Q_OBJECT

public:
    explicit Seller_newShipmentLog(QWidget *parent = nullptr);
    ~Seller_newShipmentLog();

signals:
    void newShipmentLogSubmit(QString goods_name, QString goods_num, QString goods_id);
    void cancelled();

private slots:
    void on_btn_submit_clicked();

    void on_btn_cancel_clicked();

    void on_comboBox_goodsName_currentTextChanged(const QString &arg1);

    void on_comboBox_goodsID_currentTextChanged(const QString &arg1);

private:
    Ui::Seller_newShipmentLog *ui;
};

#endif // SELLER_NEWSHIPMENTLOG_H
