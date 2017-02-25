#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <tablemodel.h>

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
    QStringList headerList;
    headerList << "序号" << "动作" << "状态" << "错误信息";
    mModel = new TableModel(0, headerList.size());
    ui->logTableView->setModel(mModel);
    mModel->setHorizontalHeaderLabels(headerList);
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
    mainCopy();
}

void
MainWindow::mainCopy()
{

}
