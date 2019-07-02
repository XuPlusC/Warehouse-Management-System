#ifndef TESTDIALOG_H
#define TESTDIALOG_H

#include <QDialog>

namespace Ui {
class TestDialog;
}

class TestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TestDialog(QWidget *parent = 0);
    ~TestDialog();
signals:
    void backtoMain();

private slots:
    void on_pushButton_clicked();

private:
    Ui::TestDialog *ui;
};

#endif // TESTDIALOG_H
