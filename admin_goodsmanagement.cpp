#include "admin_goodsmanagement.h"
#include "ui_admin_goodsmanagement.h"
#include "dbhelper.h"
#include <QDebug>
#include <QSqlQuery>

admin_goodsManagement::admin_goodsManagement(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::admin_goodsManagement)
{
    ui->setupUi(this);
//    ui->tableView_display->setEditTriggers(QAbstractItemView::NoEditTriggers);

    model = new QStandardItemModel();
    model->setColumnCount(6);
    model->setHeaderData(0, Qt::Horizontal, "货品编号");
    model->setHeaderData(1, Qt::Horizontal, "货品名称");
    model->setHeaderData(2, Qt::Horizontal, "库存量");
    model->setHeaderData(3, Qt::Horizontal, "存放地");
    model->setHeaderData(4, Qt::Horizontal, "生产厂家");
    model->setHeaderData(5, Qt::Horizontal, "修改");
    ui->tableView_display->setModel(model);
    ui->tableView_display->setEditTriggers(QAbstractItemView::NoEditTriggers);  // dont allow to edit the table

    updateTable();
}

admin_goodsManagement::~admin_goodsManagement()
{
    delete ui;
}

void admin_goodsManagement::updateTable()
{
    model->removeRows(0, model->rowCount());
    currentRowCount = 0;

    DBHelper *helper = DBHelper::getInstance();
    helper->connectDatabase();

    QString str_query = "select * from GOODS;";
    QSqlQuery query;
    query.prepare(str_query);
    query.exec();
    while (query.next())
    {
        QString str_goodsID = query.value(0).toString();
        model->setItem(currentRowCount, 0, new QStandardItem(str_goodsID));
        QString str_goodsName = query.value(1).toString();
        model->setItem(currentRowCount, 1, new QStandardItem(str_goodsName));
        QString str_storageNum = query.value(2).toString();
        model->setItem(currentRowCount, 2, new QStandardItem(str_storageNum));
        QString str_place = query.value(3).toString();
        model->setItem(currentRowCount, 3, new QStandardItem(str_place));
        QString str_producer = query.value(4).toString();
        model->setItem(currentRowCount, 4, new QStandardItem(str_producer));

        btn_edit = new QPushButton("修改");
        btn_edit->setProperty("goodsID", str_goodsID);
        btn_edit->setProperty("goodsName", str_goodsName);
        btn_edit->setProperty("storageNum", str_storageNum.toInt());
        btn_edit->setProperty("place", str_place);
        btn_edit->setProperty("producer", str_producer);
        qDebug() << "setting btn" << btn_edit->property("goodsID").toString();
        QObject::connect(btn_edit, SIGNAL(clicked()), this, SLOT(on_btn_edit_clicked()));
        ui->tableView_display->setIndexWidget(model->index(currentRowCount, 5), btn_edit);
        currentRowCount++;
    }

    helper->disconnectDatabase();
    ui->tableView_display->setModel(model);
    ui->tableView_display->resizeColumnsToContents();
    ui->tableView_display->setColumnWidth(5, ui->tableView_display->columnWidth(5) + 35);
}

void admin_goodsManagement::on_btn_back_clicked()
{
    emit(backToMain());
}

void admin_goodsManagement::on_btn_edit_clicked()
{
    QPushButton *btn_temp = (QPushButton*) sender();
    qDebug() << "checking goods " << btn_temp->property("goodsID").toInt();

    if(editDialog != NULL)
        delete editDialog;

    editDialog = new Admin_goodsEdit(
                btn_temp->property("goodsID").toString().trimmed(),
                btn_temp->property("goodsName").toString().trimmed(),
                btn_temp->property("storageNum").toString().trimmed(),
                btn_temp->property("place").toString().trimmed(),
                btn_temp->property("producer").toString().trimmed(),
                this);
    QObject::connect(editDialog, SIGNAL(closed()), this, SLOT(on_edit_closed()));
    editDialog->show();
}

void admin_goodsManagement::on_btn_refresh_clicked()
{
    updateTable();
}

void admin_goodsManagement::on_edit_closed()
{
    delete editDialog;
    editDialog = NULL;
    updateTable();
}

void admin_goodsManagement::on_btn_add_clicked()
{
    DBHelper *helper = DBHelper::getInstance();
    helper->connectDatabase();
    QSqlQuery query;
    QString str_query;

    str_query = "select MAX(goodsID) from GOODS;";
    query.prepare(str_query);
    query.exec();
    int newGoodsID = 0;
    while(query.next())
    {
        newGoodsID = query.value(0).toInt() + 1;
    }

    QString str_goodsID = QString::number(newGoodsID);
    if(editDialog != NULL)
        delete editDialog;
    editDialog = new Admin_goodsEdit(
                str_goodsID,
                "请输入货品名称",
                "请输入库存数量",
                "请输入存放地",
                "请输入生产商",
                this);
    editDialog->setInsertMode();
    QObject::connect(editDialog, SIGNAL(closed()), this, SLOT(on_edit_closed()));
    editDialog->show();
}
