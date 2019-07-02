#include "testdialog.h"
#include "ui_testdialog.h"
#include <QMainWindow>

TestDialog::TestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestDialog)
{
    ui->setupUi(this);
}

TestDialog::~TestDialog()
{
    delete ui;
}

void TestDialog::on_pushButton_clicked()
{
    emit(backtoMain());
}
