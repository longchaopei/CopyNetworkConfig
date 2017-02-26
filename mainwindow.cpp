#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QAxObject>
#include <tablemodel.h>
#include <QMessageBox>

//#define IS_DEBUG

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initView();
    mFileDialog = new QFileDialog();
    mSourceFilePath = "";
    mTargetFilePath = "";
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
    ui->logTableView->setStyleSheet(
                "QTableWidget{background-color:rgb(250, 250, 250);"
                "alternate-background-color:rgb(255, 255, 224);}");     //设置间隔行颜色变化
}

void
MainWindow::setViewVisible(bool isVisible)
{
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
    QList<QList<QVariant>> srcDatas;
    setViewVisible(false);
    read(srcDatas);
    write(srcDatas);
    setViewVisible(true);
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
#ifdef IS_DEBUG
        for (int k = 0; k < datas.size(); k++) {
            qDebug() << "row " << k << "*****************************";
            for (int j = 0; j < datas.at(k).size(); j++) {
                qDebug() << "index[" << j << "]=" << "["
                         << datas.at(k).at(j).toString() << "]";
            }
        }
#endif
        QString searchUnit = "";
        QString targetUnit = "";
        int srcRows = datas.size();
        int targetRows = targetDatas.size();
        for (int srcCurRow = 0; srcCurRow < srcRows; srcCurRow++) {
            searchUnit = datas.at(srcCurRow).at(1).toString()
                    + datas.at(srcCurRow).at(2).toString();
            for (int targetCurRow = 0;
                 targetCurRow < targetRows;
                 targetCurRow++) {
                targetUnit = targetDatas.at(targetCurRow).at(3).toString();
                if (targetUnit.length() < searchUnit.length())
                    continue;
                if (searchUnit == targetUnit.mid(0, searchUnit.length())) {
                    qDebug()<< "row=" << targetCurRow << ", "
                            << "searchUnit=" << searchUnit
                            << ", targetUnit=" << targetUnit;
                }
            }
            qDebug()<< searchUnit;
        }
    } else {
        QString errLog = "文件[" + mTargetFilePath + "] 为空!";
        QMessageBox::critical(this,
                              tr("出错"),
                              errLog,
                              QMessageBox::Ok,
                              QMessageBox::Ok);
    }

    mTargetWorkBook->dynamicCall("Close(Boolean)", false);              //关闭表
    targetExcel.dynamicCall("Quit(void)");                              //释放excel
}
