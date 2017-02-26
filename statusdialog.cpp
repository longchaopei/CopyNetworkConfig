#include "ui_statusdialog.h"
#include <QToolBar>
#include <tablemodel.h>
#include <statusdialog.h>

StatusDialog::StatusDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatusDialog)
{
    ui->setupUi(this);
    curRow = 0;
    initView();
}

StatusDialog::~StatusDialog()
{
    delete ui;
}

void
StatusDialog::initView()
{
    // 初始化tableview
    // 表头
    QStringList headerList;
    headerList << "序号" << "动作" << "状态" << "错误信息";
    mModel = new TableModel(0, headerList.size());
    ui->tableView->setModel(mModel);
    mModel->setHorizontalHeaderLabels(headerList);

    // 行宽
    ui->tableView->setColumnWidth(1, 300);
    ui->tableView->setColumnWidth(3, 300);

    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->tableView->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(
                QAbstractItemView::SingleSelection);

    ui->tableView->verticalHeader()->setVisible(false);              //隐藏行表头
    ui->tableView->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化
}


void
StatusDialog::appendRow(QString action,
                        QString stat,
                        QString err)
{
    qDebug("appendRow");
    QStandardItem* numItem
            = new QStandardItem(QString::number(curRow++));
    QStandardItem* actItem
            = new QStandardItem(action);
    QStandardItem* statItem
            = new QStandardItem(stat);
    QStandardItem* errItem
            = new QStandardItem(err);

    QList<QStandardItem*> items;
    items << numItem << actItem << statItem << errItem;
    mModel->appendRow(items);
    qDebug("appendRow  end----------------------->>>>");
}
