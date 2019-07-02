#ifndef BUYER_NEWPURCHASELOG_H
#define BUYER_NEWPURCHASELOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QDebug>
#include <QTextCodec>
#include <QMessageBox>
#include "dbhelper.h"

namespace Ui {
class Buyer_NewPurchaseLog;
}

class Buyer_NewPurchaseLog : public QDialog
{
    Q_OBJECT

public:
    explicit Buyer_NewPurchaseLog(QWidget *parent = nullptr);
    ~Buyer_NewPurchaseLog();

signals:
    void newpurchaselogAffirm(QString goods_name, QString goods_num, QString goods_id);
    void cancelled();

private slots:
    void on_goods_name_currentTextChanged(const QString &arg1);

    void on_goods_id_currentTextChanged(const QString &arg1);

    void on_affirm_clicked();

    void on_btn_cancel_clicked();

private:
    Ui::Buyer_NewPurchaseLog *ui;
};

#endif // BUYER_NEWPURCHASELOG_H
