#include "ui_helpdialog.h"
#include <QToolBar>
#include <helpdialog.h>

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("使用须知");
    initView();
}

HelpDialog::~HelpDialog()
{
    delete ui;
}

void
HelpDialog::initView()
{
    this->setFixedSize(1200, 600);
}


void HelpDialog::on_closeBtn_clicked()
{
    this->close();
}
