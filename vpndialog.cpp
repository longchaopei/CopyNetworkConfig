#include "vpndialog.h"
#include "ui_vpndialog.h"
#include <QFileDialog>
#include <QDebug>
#include <QAxObject>
#include <tablemodel.h>
#include <QMessageBox>
#include <helpdialog.h>
#include <versiondialog.h>
#include <coverinfodialog.h>
#include <QTime>
#include <mainwindow.h>
#include <exceloperate.h>
#include <waitdialog.h>

#define LOG_TAG                         "VPN_DIALOG"
#include "utils/Log.h"
//#define IS_DEBUG

#define HEADER_STR_SUBNET               "村路由器大子网"
#define HEADER_STR_SUBNETMASK           "村路由器大子网掩码"
#define HEADER_STR_IP_ADDR              "村路由器IP地址"
#define HEADER_STR_COUNTRY_SUBNET       "县网关中心子网"
#define HEADER_STR_COUNTRY_SUBNETMASK   "县网关中心子网掩码"

VpnDialog::VpnDialog(QWidget *parent) :
    QDialog(parent),
    mWaitDialog(new WaitDialog()),
    mFileDialog(new QFileDialog()),
    ui(new Ui::VpnDialog)
{
    ui->setupUi(this);

    connect(this, SIGNAL(openWaitDialog()),
            mWaitDialog, SLOT(openWindow()));
    connect(this, SIGNAL(closeWaitDialog()),
            mWaitDialog, SLOT(closeWindow()));
    initView();
}

VpnDialog::~VpnDialog()
{
    delete ui;
}

void
VpnDialog::openWindow()
{
    this->exec();
}

void
VpnDialog::initView()
{
    this->setWindowTitle("批量添加VPN配置");
    ui->srcFileLineEdit->setFocusPolicy(Qt::NoFocus);
    ui->targetFileLineEdit->setFocusPolicy(Qt::NoFocus);

    // 初始化tableview
    // 表头
    QStringList headerList;
    headerList << "序号" << "动作" << "状态" << "错误信息";
    mModel = new TableModel(0, headerList.size());
    ui->logTableView->setModel(mModel);
    mModel->setHorizontalHeaderLabels(headerList);

    // 行宽
    ui->logTableView->setColumnWidth(1, 300);
    ui->logTableView->setColumnWidth(3, 300);

    //设置单元格不可编辑,单击选中一行且只能选中一行
    ui->logTableView->setEditTriggers(
                QAbstractItemView::NoEditTriggers);
    ui->logTableView->setSelectionBehavior(
                QAbstractItemView::SelectRows);
    ui->logTableView->setSelectionMode(
                QAbstractItemView::SingleSelection);

    ui->logTableView->horizontalHeader()
            ->setSectionResizeMode(3, QHeaderView::Stretch);

    ui->logTableView->verticalHeader()->setVisible(false);              //隐藏行表头
    ui->logTableView->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{"
                "background-color:rgb(234, 234, 234)}");                //表头颜色

    ui->logTableView->setAlternatingRowColors(true);
    ui->logTableView->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化
}

void
VpnDialog::setViewEnable(bool enable)
{
    delaymsec(10);
    ui->srcFileToolBtn->setEnabled(enable);
    ui->targetFileToolBtn->setEnabled(enable);
    ui->startBtn->setEnabled(enable);
}

void
VpnDialog::delaymsec(int msec)
{
#if 0
    // 阻塞延时
    QTime now;
    QTime n=QTime::currentTime();

    do{
        now=QTime::currentTime();
    } while (n.msecsTo(now) <= msec);
#else
    // 非阻塞延时
    // 因为读写excel操作太占cpu，
    // 加此延时函数，给其他代码可以处理，如之前不加延时时，界面更新会被阻塞
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
#endif
}

void
VpnDialog::on_srcFileToolBtn_clicked()
{
    mFileDialog->setWindowTitle(tr("选择IP信息表"));
    mFileDialog->setDirectory(".");                                     //默认打开目录为当前目录
    mFileDialog->setFileMode(QFileDialog::ExistingFile);                //选择存在的文件
    mFileDialog->setNameFilter(QString("所有excel文件(*.xls *.xlsx);"));

    if (mFileDialog->exec() == QDialog::Accepted) {
        qDebug("on_srcFileToolBtn_clicked selected");
        mSourceFilePath = mFileDialog->selectedFiles()[0];
        ui->srcFileLineEdit->setText(mSourceFilePath);
    }
}

void
VpnDialog::on_targetFileToolBtn_clicked()
{
    mFileDialog->setWindowTitle(tr("选择路由器配置文件"));
    mFileDialog->setDirectory(".");                                     //默认打开目录为当前目录
    mFileDialog->setFileMode(QFileDialog::ExistingFile);                //选择存在的文件
    mFileDialog->setNameFilter(QString("所有路由器配置文件(*.con);"));

    if (mFileDialog->exec() == QDialog::Accepted) {
        qDebug("on_targetFileToolBtn_clicked selected");
        mTargetFilePath = mFileDialog->selectedFiles()[0];
        ui->targetFileLineEdit->setText(mTargetFilePath);
    }
}

bool
VpnDialog::initSrcColumn(const QList<QList<QVariant>> &villageDatas,
                         const QList<QList<QVariant>> &countryDatas)
{
    mVillageIpColumn = villageDatas.at(MainWindow::mHeaderRow)
            .indexOf(HEADER_STR_IP_ADDR);
    mVillageSubnetColumn = villageDatas.at(MainWindow::mHeaderRow)
            .indexOf(HEADER_STR_SUBNET);
    mVillageSubnetmaskColumn = villageDatas.at(MainWindow::mHeaderRow)
            .indexOf(HEADER_STR_SUBNETMASK);
    mCountrySubnetColumn = countryDatas.at(MainWindow::mHeaderRow)
            .indexOf(HEADER_STR_COUNTRY_SUBNET);
    mCountrySubnetmaskColumn = countryDatas.at(MainWindow::mHeaderRow)
            .indexOf(HEADER_STR_COUNTRY_SUBNETMASK);

    qDebug() << "mVillageIpColumn = " << mVillageIpColumn << ", "
             << "mVillageSubnetColumn = " << mVillageSubnetColumn << ", "
             << "mVillageSubnetmaskColumn = " << mVillageSubnetmaskColumn << ", "
             << "mCountrySubnetColumn = " << mCountrySubnetColumn << ", "
             << "mCountrySubnetmaskColumn = " << mCountrySubnetmaskColumn;

    if (mVillageIpColumn < 0 ||
            mVillageSubnetColumn < 0 ||
            mVillageSubnetmaskColumn < 0 ||
            mCountrySubnetColumn < 0 ||
            mCountrySubnetmaskColumn < 0)
        return false;

    return true;
}

bool
VpnDialog::assertFile(QString path)
{
    if (!QFile::exists(path)) {
        QString errLog;
        errLog = "文件[" + path + "] 不存在！";
        QMessageBox::critical(this,
                              tr("出错"),
                              errLog,
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return false;
    }

    return true;
}

void
VpnDialog::on_startBtn_clicked()
{
    ALOGDTRACE();

    if (isStart)
        return;

    if (ui->srcFileLineEdit->text() == NULL ||
            ui->targetFileLineEdit->text() == NULL) {
        QMessageBox::critical(this,
                              tr("出错"),
                              tr("源文件路径或目标文件路径不能为空!\n"),
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }

    QList<QList<QVariant>> srcCountryDatas;
    QList<QList<QVariant>> srcVillageDatas;

    if (!assertFile(mSourceFilePath) || !assertFile(mTargetFilePath))
        return;

    isStart = true;
    setViewEnable(false);
    delaymsec(10);
//    emit openWaitDialog();
    int ret = ExcelOperate::readSheet(mSourceFilePath, 1, srcVillageDatas);
    ret = ExcelOperate::readSheet(mSourceFilePath, 2, srcCountryDatas);

    if (!initSrcColumn(srcVillageDatas, srcCountryDatas)) {
        QString errLog = "文件[" + mSourceFilePath + "] 表头格式错误";
        QMessageBox::critical(this,
                              tr("出错"),
                              errLog,
                              QMessageBox::Ok,
                              QMessageBox::Ok);
    } else {
        insertVpnSetting(srcVillageDatas, srcCountryDatas);
    }

    setViewEnable(true);
    isStart = false;
//    emit closeWaitDialog();
}

int
VpnDialog::insertVpnSetting(const QList<QList<QVariant>> srcVillageDatas,
                            const QList<QList<QVariant>> srcCountryDatas)
{
    ALOGDTRACE();

    QString villageIp;
    QString villageSubNet;
    QString villageSubNetmask;
    QString vpnStr;
    QString ipRouteStr;
    QString countrySubNet =
            srcCountryDatas.at(1).at(mCountrySubnetColumn).toString() == "" ?
                "127.0.0.1" :
                srcCountryDatas.at(1).at(mCountrySubnetColumn).toString();
    QString countrySubNetmask = srcCountryDatas.at(1).at(mCountrySubnetmaskColumn).toString() == "" ?
                "127.0.0.1" :
                srcCountryDatas.at(1).at(mCountrySubnetmaskColumn).toString();

    int size = srcVillageDatas.size();
    for (int pos = 1; pos < size; pos++) {
        villageIp = srcVillageDatas.at(pos).at(mVillageIpColumn).toString();
        villageSubNet = srcVillageDatas.at(pos).at(mVillageSubnetColumn).toString();
        villageSubNetmask = srcVillageDatas.at(pos).at(mVillageSubnetmaskColumn).toString();

        if (villageIp == "" || villageSubNet == "" || villageSubNetmask == "")
            continue;

        ALOGD("row[%d] villageIp = %s, villageSubNet = %s, villageSubNetmask = %s,"
              "countrySubNet = %s, countrySubNetmask = %s\n",
              pos,
              villageIp.toStdString().data(),
              villageSubNet.toStdString().data(),
              villageSubNetmask.toStdString().data(),
              countrySubNet.toStdString().data(),
              countrySubNetmask.toStdString().data());
    }

    emit openWaitDialog();
}
