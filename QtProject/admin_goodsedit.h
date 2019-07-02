#ifndef ADMIN_GOODSEDIT_H
#define ADMIN_GOODSEDIT_H

#include <QDialog>

namespace Ui {
class Admin_goodsEdit;
}

class Admin_goodsEdit : public QDialog
{
    Q_OBJECT

public:
    explicit Admin_goodsEdit(	QString goodsID,
                                QString goodsName,
                                QString storageNum,
                                QString place,
                                QString producer,
                                QWidget *parent = nullptr);
    ~Admin_goodsEdit();
    void setInsertMode();

signals:
    void closed();

private slots:
    void on_btn_submit_clicked();

    void on_btn_back_clicked();

private:
    Ui::Admin_goodsEdit *ui;
    QString goodsID;
    QString goodsName;
    QString storageNum;
    QString place;
    QString producer;

    int isInsertMode = 0;
};

#endif // ADMIN_GOODSEDIT_H
