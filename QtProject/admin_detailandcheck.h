#ifndef ADMIN_DETAILANDCHECK_H
#define ADMIN_DETAILANDCHECK_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class admin_detailAndCheck;
}

class admin_detailAndCheck : public QDialog
{
    Q_OBJECT

public:
    explicit admin_detailAndCheck(QWidget *parent = nullptr);
    ~admin_detailAndCheck();
    void setAttributes(int type, int listID, int status);
    void setAdminID(int id);
    void updateTable();

signals:
    void closed();

private slots:
    void on_btn_back_clicked();

    void on_btn_check_clicked();

    void on_btn_uncheck_clicked();

    void on_btn_suspend_clicked();

private:
    Ui::admin_detailAndCheck *ui;
    // login admin's ID
    int adminID;
    // tableview attributes
    QStandardItemModel *model;
    int currentRowCount;
    // purchase/ shipment list attributes
    int type;
    int listID;
    int status;
    // (legacy) if is allowed to check
    int allowedToCheck;
};

#endif // ADMIN_DETAILANDCHECK_H
