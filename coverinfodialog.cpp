#include "coverinfodialog.h"
#include "ui_coverinfodialog.h"
#include "tablemodel.h"
#include "datatype.h"
#include <QStandardItem>

#define LOG_TAG                         "COVER_DIALOG"
#include "utils/Log.h"

CoverInfoDialog::CoverInfoDialog(QWidget *parent) :
    QDialog(parent),
    mRowCount(0),
    ui(new Ui::CoverInfoDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("查看覆盖信息");
    this->setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    initView();

    connect((QObject*)ui->beforeCoverTableview->verticalScrollBar(), SIGNAL(valueChanged(int)),
            (QObject*)ui->afterCoverTableview->verticalScrollBar(), SLOT(setValue(int)));
}

CoverInfoDialog::~CoverInfoDialog()
{
    delete ui;
}

void
CoverInfoDialog::updateVerticalScrollBar(int value)
{
    ALOGD("%s enter, value = %d\n",  __FUNCTION__, value);
}

void
CoverInfoDialog::initView()
{
    initBeforeTableview();
    initAfterTableview();
}

void
CoverInfoDialog::initBeforeTableview()
{
    //设置首行标题
    QStringList headerList;
    headerList << "编号" << "区划名称" << "区划代码" << "IP地址" << "IP掩码" << "IP网关";

    mBeforeModel = new TableModel(0, headerList.size());
    ui->beforeCoverTableview->setModel(mBeforeModel);
    mBeforeModel->setHorizontalHeaderLabels(headerList);

    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->beforeCoverTableview->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->beforeCoverTableview->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->beforeCoverTableview->setSelectionMode(
                QAbstractItemView::SingleSelection);

    ui->beforeCoverTableview->horizontalHeader()
            ->setSectionResizeMode(4, QHeaderView::Stretch);
    //    ui->annualTableview->setHorizontalScrollBarPolicy(
    //                Qt::ScrollBarAlwaysOff);                          //隐藏滚动条
    ui->beforeCoverTableview->verticalHeader()->setVisible(false);      //隐藏行表头
    ui->beforeCoverTableview->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->beforeCoverTableview->setAlternatingRowColors(true);
    ui->beforeCoverTableview->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化

    ui->beforeCoverTableview->setColumnWidth(0, 100);
    ui->beforeCoverTableview->setColumnWidth(1, 200);
    ui->beforeCoverTableview->setColumnWidth(2, 150);
    ui->beforeCoverTableview->setColumnWidth(3, 150);
    ui->beforeCoverTableview->setColumnWidth(4, 150);
    ui->beforeCoverTableview->setColumnWidth(5, 150);
}

void
CoverInfoDialog::initAfterTableview()
{
    //设置首行标题
    QStringList headerList;
    headerList << "编号" << "区划名称" << "区划代码" << "IP地址" << "IP掩码" << "IP网关";

    mAfterModel = new TableModel(0, headerList.size());
    ui->afterCoverTableview->setModel(mAfterModel);
    mAfterModel->setHorizontalHeaderLabels(headerList);

    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->afterCoverTableview->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->afterCoverTableview->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->afterCoverTableview->setSelectionMode(
                QAbstractItemView::SingleSelection);

    ui->afterCoverTableview->horizontalHeader()
            ->setSectionResizeMode(4, QHeaderView::Stretch);
    //    ui->annualTableview->setHorizontalScrollBarPolicy(
    //                Qt::ScrollBarAlwaysOff);                          //隐藏滚动条
    ui->afterCoverTableview->verticalHeader()->setVisible(false);       //隐藏行表头
    ui->afterCoverTableview->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->afterCoverTableview->setAlternatingRowColors(true);
    ui->afterCoverTableview->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化

    ui->afterCoverTableview->setColumnWidth(0, 100);
    ui->afterCoverTableview->setColumnWidth(1, 200);
    ui->afterCoverTableview->setColumnWidth(2, 150);
    ui->afterCoverTableview->setColumnWidth(3, 150);
    ui->afterCoverTableview->setColumnWidth(4, 150);
    ui->afterCoverTableview->setColumnWidth(5, 150);
}

void
CoverInfoDialog::clearTable()
{
    if (mBeforeModel->rowCount() > 0)
        mBeforeModel->removeRows(0, mBeforeModel->rowCount());

    if (mAfterModel->rowCount() > 0)
        mAfterModel->removeRows(0, mAfterModel->rowCount());

    mRowCount = 0;
}

void
CoverInfoDialog::openWindow()
{
    this->exec();
}

void
CoverInfoDialog::appendRow(const COUNTRY_INFO before, const COUNTRY_INFO after)
{
    mRowCount++;
    beforeTableAppendRow(before);
    afterTableAppendRow(after);
}

void
CoverInfoDialog::beforeTableAppendRow(const COUNTRY_INFO before)
{
    ALOGD("before zoningname = %s,"
          "zoningcode = %s,"
          "ip = [%s],"
          "netmask = [%s],"
          "gateway = [%s]",
          before.countryName.toStdString().data(),
          before.zoningCode.toStdString().data(),
          before.ipAddr.toStdString().data(),
          before.netmask.toStdString().data(),
          before.gateWay.toStdString().data());

    QList<QStandardItem*> items;
    initItems(items, before);
    mBeforeModel->appendRow(items);
}

void
CoverInfoDialog::afterTableAppendRow(const COUNTRY_INFO after)
{
    ALOGD("after zoningname = %s,"
          "zoningcode = %s,"
          "ip = [%s],"
          "netmask = [%s],"
          "gateway = [%s]",
          after.countryName.toStdString().data(),
          after.zoningCode.toStdString().data(),
          after.ipAddr.toStdString().data(),
          after.netmask.toStdString().data(),
          after.gateWay.toStdString().data());

    QList<QStandardItem*> items;
    initItems(items, after);
    mAfterModel->appendRow(items);
}

bool
CoverInfoDialog::initItems(QList<QStandardItem*> &items, const COUNTRY_INFO info)
{
    QStandardItem* number = new QStandardItem(QString::number(mRowCount));
    QStandardItem* name = new QStandardItem(info.countryName);
    QStandardItem* code = new QStandardItem(info.zoningCode);
    QStandardItem* ip = new QStandardItem(info.ipAddr);
    QStandardItem* netmask = new QStandardItem(info.netmask);
    QStandardItem* gateway = new QStandardItem(info.gateWay);

    items << number << name << code << ip << netmask << gateway;
    return true;
}
