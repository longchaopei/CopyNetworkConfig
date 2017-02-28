#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QAxObject>
#include <tablemodel.h>
#include <QMessageBox>
#include <helpdialog.h>
#include <versiondialog.h>
#include <QTime>

//#define IS_DEBUG

#define HEADER_STR_SRC_TOWNSHIP         "乡镇"
#define HEADER_STR_SRC_VILLAEG          "站点名称"
#define HEADER_STR_SRC_IP_ADDR          "地址"
#define HEADER_STR_SRC_GATEWAY          "网关"
#define HEADER_STR_SRC_NETMASK          "掩码"

#define HEADER_STR_TARGET_ZONING_NAME   "区划名称"
#define HEADER_STR_TARGET_ZONING_CODE   "区划代码"
#define HEADER_STR_TARGET_IP_ADDR       "村路由器IP地址"
#define HEADER_STR_TARGET_GATEWAY       "村路由器网关"
#define HEADER_STR_TARGET_NETMASK       "村路由器IP掩码"

#define STATUS_SUCCESS_STR          "成功"
#define STATUS_FAIL_STR             "失败"
#define STATUS_UNKNOWN_STR          "未知"
#define ACTION_READ_STR             "读取源文件数据"
#define ACTION_START_COPY_STR       "开始拷贝配置"
#define ACTION_STOP_COPY_STR        "拷贝配置结束"
#define ERR_NOT_FOUND_STR(filestr) \
    tr("目标文档没有找到[").append(filestr).append("]")

#define GET_ACTION_COPY_FILE_STR(filestr)  \
    tr("拷贝").append(filestr).append("配置")

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    curRow(1),
    isStartCopy(false),
    mVersionDialog(new VersionDialog()),
    mHelpDialog(new HelpDialog()),
    mFileDialog(new QFileDialog()),
    mSourceFilePath(""),
    mTargetFilePath(""),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("CopyNetworkConfig");
    initView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::initView()
{
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
MainWindow::clearTableView()
{
    mModel->clear();
    QStringList headerList;
    headerList << "序号" << "动作" << "状态" << "错误信息";
    mModel->setHorizontalHeaderLabels(headerList);
    // 行宽
    ui->logTableView->setColumnWidth(1, 300);
    ui->logTableView->setColumnWidth(3, 300);
    curRow = 1;
}

void
MainWindow::setViewVisible(bool isVisible)
{
    delaymsec(10);
    ui->srcFileToolBtn->setEnabled(isVisible);
    ui->targetFileToolBtn->setEnabled(isVisible);
    ui->startBtn->setEnabled(isVisible);
}

void
MainWindow::on_srcFileToolBtn_clicked()
{
    mFileDialog->setWindowTitle(tr("选择源文件"));
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
MainWindow::on_targetFileToolBtn_clicked()
{
    mFileDialog->setWindowTitle(tr("选择目标文件"));
    mFileDialog->setDirectory(".");                                     //默认打开目录为当前目录
    mFileDialog->setFileMode(QFileDialog::ExistingFile);                //选择存在的文件
    mFileDialog->setNameFilter(QString("所有excel文件(*.xls *.xlsx);"));

    if (mFileDialog->exec() == QDialog::Accepted) {
        qDebug("on_targetFileToolBtn_clicked selected");
        mTargetFilePath = mFileDialog->selectedFiles()[0];
        ui->targetFileLineEdit->setText(mTargetFilePath);
    }
}

void MainWindow::on_startBtn_clicked()
{
    delaymsec(10);
    if (isStartCopy)
        return;

    if (mSourceFilePath == NULL ||
            mTargetFilePath == NULL) {
        qDebug("mainCopy, path is null.");
        QMessageBox::critical(this,
                              tr("出错"),
                              tr("源文件路径或目标文件路径不能为空!\n"),
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    mainCopy();
}

bool
MainWindow::assertFile(QString path)
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
MainWindow::mainCopy()
{
    clearTableView();
    QList<QList<QVariant>> srcDatas;
    appendRow("开始任务", STATUS_SUCCESS_STR, "");
    setViewVisible(false);
    isStartCopy = true;
    for (int i = 0; i < 100000000; i++);
    read(srcDatas);
    if (!initSrcColumns(srcDatas)) {
        QString errLog = "文件[" + mSourceFilePath + "] 表头格式错误";
        QMessageBox::critical(this,
                              tr("出错"),
                              errLog,
                              QMessageBox::Ok,
                              QMessageBox::Ok);
    } else {
        write(srcDatas);
    }
    appendRow("结束任务", STATUS_SUCCESS_STR, "");
    isStartCopy = false;
    setViewVisible(true);
}

bool MainWindow::initSrcColumns(QList<QList<QVariant>> &datas)
{
    mSrcTownShipColumn =
            datas.at(mHeaderRow).indexOf(HEADER_STR_SRC_TOWNSHIP);
    mSrcVillageColumn =
            datas.at(mHeaderRow).indexOf(HEADER_STR_SRC_VILLAEG);
    mSrcIpAddrColumn =
            datas.at(mHeaderRow).indexOf(HEADER_STR_SRC_IP_ADDR);
    mSrcGatewayColumn =
            datas.at(mHeaderRow).indexOf(HEADER_STR_SRC_GATEWAY);
    mSrcNetmaskColumn =
            datas.at(mHeaderRow).indexOf(HEADER_STR_SRC_NETMASK);
    if (mSrcTownShipColumn < 0 ||
            mSrcVillageColumn < 0 ||
            mSrcIpAddrColumn < 0 ||
            mSrcGatewayColumn < 0 ||
            mSrcNetmaskColumn < 0) {
        return false;
    }

    qDebug() << "mSrcTownShipColumn = " << mSrcTownShipColumn << ", "
             << "mSrcVillageColumn = " << mSrcVillageColumn << ", "
             << "mSrcIpAddrColumn = " << mSrcIpAddrColumn << ", "
             << "mSrcGatewayColumn = " << mSrcGatewayColumn << ", "
             << "mSrcNetmaskColumn = " << mSrcNetmaskColumn;

    return true;
}

bool MainWindow::initTargetColumns(QList<QList<QVariant>> &datas)
{
    mZoningNameColumn =
            datas.at(mHeaderRow).indexOf(HEADER_STR_TARGET_ZONING_NAME);
    mTargetIpAddrColumn =
            datas.at(mHeaderRow).indexOf(HEADER_STR_TARGET_IP_ADDR);
    mTargetGatewayColumn =
            datas.at(mHeaderRow).indexOf(HEADER_STR_TARGET_GATEWAY);
    mTargetNetmaskColumn =
            datas.at(mHeaderRow).indexOf(HEADER_STR_TARGET_NETMASK);
    if (mZoningNameColumn < 0 ||
            mTargetIpAddrColumn < 0 ||
            mTargetGatewayColumn < 0 ||
            mTargetNetmaskColumn < 0) {
        return false;
    }

    qDebug() << "mZoningNameColumn = " << mZoningNameColumn << ", "
             << "mTargetIpAddrColumn = " << mTargetIpAddrColumn << ", "
             << "mTargetGatewayColumn = " << mTargetGatewayColumn << ", "
             << "mTargetNetmaskColumn = " << mTargetNetmaskColumn;

    return true;
}

void
MainWindow::read(QList<QList<QVariant>> &datas)
{
    if (!assertFile(mSourceFilePath))
        return;

    QAxObject srcExcel("Excel.Application");
    srcExcel.setProperty("Visible", false);                             //不显示文档
    srcExcel.querySubObject("WorkBooks")
            ->dynamicCall("Open (const QString&)", mSourceFilePath);
    mSrcWorkBook = srcExcel.querySubObject("ActiveWorkBook");           //获取活动工作簿
    mSrcSheet = mSrcWorkBook->querySubObject("Worksheets(int)", 1);     //获取第一个表
    mSrcRange = mSrcSheet->querySubObject("UsedRange");
    QVariant vars = mSrcRange->dynamicCall("Value");                    //获取整表数据
    mSrcWorkBook->dynamicCall("Close(Boolean)", false);                 //关闭表
    srcExcel.dynamicCall("Quit(void)");                                 //释放excel

    QVariantList varRows = vars.toList();
    QVariantList rowData;
    if (!varRows.isEmpty()) {
        int rowCount = varRows.size();
        datas.clear();
        for (int i = 0; i < rowCount; i++) {
            rowData = varRows[i].toList();
            //将每一行数据push到srcDatas里
            datas.push_back(rowData);
        }
        appendRow(ACTION_READ_STR, STATUS_SUCCESS_STR, "");
#ifdef IS_DEBUG
        for (int k = 0; k < datas.size(); k++) {
            qDebug() << "row " << k << "*****************************";
            for (int j = 0; j < datas.at(k).size(); j++) {
                qDebug() << "index[" << j << "]=" << "["
                         << datas.at(k).at(j).toString() << "]";
            }
        }
#endif
    } else {
        QString errLog = "文件[" + mSourceFilePath + "] 为空!";
        QMessageBox::critical(this,
                              tr("出错"),
                              errLog,
                              QMessageBox::Ok,
                              QMessageBox::Ok);
    }
}

void
MainWindow::write(QList<QList<QVariant>> &datas)
{
    if (!assertFile(mTargetFilePath))
        return;

    qDebug("\n\n\n\n--------------------write--------------------------------\n\n");

    if (datas.isEmpty())
        QMessageBox::critical(this,
                              tr("出错"),
                              tr("未知出错"),
                              QMessageBox::Ok,
                              QMessageBox::Ok);

    QAxObject targetExcel("Excel.Application");
    targetExcel.setProperty("Visible", false);                          //不显示文档
    targetExcel.setProperty("DisplayAlerts", false);                   //关闭告警弹窗
    targetExcel.querySubObject("WorkBooks")
            ->dynamicCall("Open (const QString&)", mTargetFilePath);
    mTargetWorkBook = targetExcel.querySubObject("ActiveWorkBook");     //获取活动工作簿
    mTargetSheet = mTargetWorkBook->querySubObject("Worksheets(int)", 1);   //获取第一个表
    mTargetRange = mTargetSheet->querySubObject("UsedRange");
    QVariant targetVars = mTargetRange->dynamicCall("Value");           //获取整表数据

    QVariantList varRows = targetVars.toList();
    QVariantList rowData;
    QList<QList<QVariant>> targetDatas;
    if (!varRows.isEmpty()) {
        int rowCount = varRows.size();
        for (int i = 0; i < rowCount; i++) {
            rowData = varRows[i].toList();
            //将每一行数据push到srcDatas里
            targetDatas.push_back(rowData);
        }
        if (!initTargetColumns(targetDatas)) {
            QString errLog = "文件[" + mTargetFilePath + "] 表头格式错误";
            QMessageBox::critical(this,
                                  tr("出错"),
                                  errLog,
                                  QMessageBox::Ok,
                                  QMessageBox::Ok);
            return;
        }
#ifdef IS_DEBUG
        for (int k = 0; k < targetDatas.size(); k++) {
            qDebug() << "row " << k << "*****************************";
            for (int j = 0; j < targetDatas.at(k).size(); j++) {
                qDebug() << "index[" << j << "]=" << "["
                         << targetDatas.at(k).at(j).toString() << "]";
            }
        }
#endif
        QString searchUnit = "";
        QString targetUnit = "";
        QString ip, mask, gateway;
        bool isFound = false;
        int srcRows = datas.size();
        int targetRows = targetDatas.size();

        appendRow(ACTION_START_COPY_STR, "", "");
        for (int srcCurRow = 0; srcCurRow < srcRows; srcCurRow++) {
            searchUnit = datas.at(srcCurRow).at(mSrcTownShipColumn).toString()
                    + datas.at(srcCurRow).at(mSrcVillageColumn).toString();
            isFound = false;
            for (int targetCurRow = 0;
                 targetCurRow < targetRows;
                 targetCurRow++) {
                targetUnit = targetDatas.at(targetCurRow).at(mZoningNameColumn).toString();
                if (targetUnit.length() < searchUnit.length())
                    continue;
                if (searchUnit == targetUnit.mid(0, searchUnit.length())) {
                        // 拷贝配置
                        qDebug()<<"srcRow=" << srcCurRow << ", "
                                <<"targetrow=" << targetCurRow << ", "
                                << "searchUnit=" << searchUnit
                                << ", targetUnit=" << targetUnit;
                        ip = datas.at(srcCurRow).at(mSrcIpAddrColumn).toString();
                        gateway = datas.at(srcCurRow).at(mSrcGatewayColumn).toString();
                        mask = datas.at(srcCurRow).at(mSrcNetmaskColumn).toString();
                        mTargetSheet->querySubObject("Cells(int,int)",
                                                     targetCurRow+1,
                                                     mTargetIpAddrColumn+1)->setProperty("Value", ip);
                        mTargetSheet->querySubObject("Cells(int,int)",
                                                     targetCurRow+1,
                                                     mTargetNetmaskColumn+1)->setProperty("Value", mask);
                        mTargetSheet->querySubObject("Cells(int,int)",
                                                     targetCurRow+1,
                                                     mTargetGatewayColumn+1)->setProperty("Value", gateway);
                        isFound = true;
                        appendRow(GET_ACTION_COPY_FILE_STR(searchUnit),
                                  STATUS_SUCCESS_STR,
                                  "");
                        break;
                }
            }
            if (!isFound)
                appendRow(GET_ACTION_COPY_FILE_STR(searchUnit),
                          STATUS_FAIL_STR,
                          ERR_NOT_FOUND_STR(searchUnit));
        }
        appendRow(ACTION_STOP_COPY_STR, "", "");
    } else {
        QString errLog = "文件[" + mTargetFilePath + "] 为空!";
        QMessageBox::critical(this,
                              tr("出错"),
                              errLog,
                              QMessageBox::Ok,
                              QMessageBox::Ok);
    }

    mTargetWorkBook->dynamicCall("Save()");
    mTargetWorkBook->dynamicCall("Close(Boolean)", false);              //关闭表
    targetExcel.dynamicCall("Quit(void)");                              //释放excel
}

QString
MainWindow::getCopyStatusStr(CopyStatus stat)
{
    switch (stat) {
    case COPY_SUCCESS:
        return STATUS_SUCCESS_STR;
    case COPY_FAIL:
        return STATUS_FAIL_STR;
    default:
        break;
    }

    return STATUS_UNKNOWN_STR;
}

void
MainWindow::appendRow(QString action,
                      QString stat,
                      QString err)
{
    delaymsec(10);
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
    ui->logTableView->scrollToBottom();                                 //每插入一条，滚动到底部
}

void
MainWindow::delaymsec(int msec)
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

void MainWindow::on_actHelp_triggered()
{
    if (!isStartCopy)
        mHelpDialog->exec();
}

void MainWindow::on_actAbout_triggered()
{
    if (!isStartCopy)
        mVersionDialog->exec();
}
