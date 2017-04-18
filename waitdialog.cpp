#include "waitdialog.h"
#include "ui_waitdialog.h"
#include <QMovie>
#include <waitdialog.h>

WaitDialog::WaitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WaitDialog)
{
    ui->setupUi(this);
    this->setWindowOpacity(1);  //设置透明用；
    this->setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint);
    //取消对话框标题
    this->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);

    //取消对话框标题和边框
    this->setAutoFillBackground(true);
    this->setContentsMargins(0,0,0,0);
    ui->waitLabel->setContentsMargins(0,0,0,0);
    /*QPalettepalette;palette.setBrush(QPalette::Background,QBrush(QPixmap("E:/qml/imgdialog/loading.gif")));
    this->setPalette(palette)*/;
    movie = new QMovie(":/icon/icon/loading.gif");
    ui->waitLabel->setMovie(movie);
    movie->start();
}

WaitDialog::~WaitDialog()
{
    delete movie;
    delete ui;
}

void
WaitDialog::openWindow()
{
    this->exec();
}

void
WaitDialog::closeWindow()
{
    this->close();
}
