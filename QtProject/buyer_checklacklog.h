#ifndef BUYER_CHECKLACKLOG_H
#define BUYER_CHECKLACKLOG_H
#include <QStandardItemModel>
#include <QWidget>

namespace Ui {
class Buyer_CheckLackLog;
}

class Buyer_CheckLackLog : public QWidget
{
    Q_OBJECT

public:
    explicit Buyer_CheckLackLog(QWidget *parent = nullptr);
    ~Buyer_CheckLackLog();

signals:
    void backToMain();

private slots:
    void on_btn_back_clicked();

    void on_btn_refresh_clicked();

private:
    Ui::Buyer_CheckLackLog *ui;
    QStandardItemModel *model = NULL;
    int currentRowCount = 0;

    void updateTable();
};

#endif // BUYER_CHECKLACKLOG_H
