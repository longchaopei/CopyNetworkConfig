#include "versiondialog.h"
#include "ui_versiondialog.h"
#include <QDate>
#include <QTime>
#include <QDebug>

VersionDialog::VersionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VersionDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("关于");
    this->setFixedSize(500, 120);
    ui->buildDateLabel->setText(QDateTime::currentDateTime()
                                .toString("yyyy-MM-dd hh:mm:ss.zzz"));
    qDebug() << "datetime = " << QDateTime::currentDateTime()
                                    .toString("yyyy-MM-dd hh:mm:ss.zzz");
}

VersionDialog::~VersionDialog()
{
    delete ui;
}

void
VersionDialog::mousePressEvent(QMouseEvent *ev)
{
    this->close();
}
