#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QAxObject>
#include <tablemodel.h>
#include <QMessageBox>

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
    if (!assertFile(mSourceFilePath))
        return;
    if (!assertFile(mTargetFilePath))
        return;

    QAxObject srcExcel("Excel.Application");
//    srcExcel.setProperty("Visible", true);                            //显示文档
    srcExcel.setProperty("Visible", false);                             //不显示文档
    srcExcel.querySubObject("WorkBooks")
            ->dynamicCall("Open (const QString&)", mSourceFilePath);
    QVariant titleValue = srcExcel.property("Caption");                 //获取标题
    qDebug() << QString("excel title : ") << titleValue;

    mSrcWorkBook = srcExcel.querySubObject("ActiveWorkBook");           //获取活动工作簿
//    QAxObject *srcWorkSheets = srcWorkBook->querySubObject("WorkSheets");   //获取所有表
//    int sheet_count = work_sheets->property("Count").toInt();           //获取工作表数目
    mSrcSheet = mSrcWorkBook->querySubObject("Worksheets(int)", 1);     //获取第一个表
//    mSrcRange = mSrcSheet->querySubObject("Cells(int,int)", 1, 1);
//    qDebug() << mSrcRange->dynamicCall("Value2()").toString();

    mSrcRange = mSrcSheet->querySubObject("UsedRange");
    QVariant vars = mSrcRange->dynamicCall("Value");
    QVariantList varRows = vars.toList();
    if (!varRows.isEmpty()) {
        int rowCount = varRows.size();
        QVariantList rowData;
        QList<QList<QVariant>> res;
        for (int i = 0; i < rowCount; i++) {
            rowData = varRows[i].toList();
            res.push_back(rowData);
        }

        for (int j = 0; j < res.at(0).size(); j++) {
            qDebug() << "index[" << j << "]=" << res.at(0).at(j).toString();
        }
    }

    // 释放资源
    mSrcWorkBook->dynamicCall("Close(Boolean)", false);
    srcExcel.dynamicCall("Quit(void)");
}
