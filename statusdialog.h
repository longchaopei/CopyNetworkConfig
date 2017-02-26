#ifndef STATUSDIALOG_H
#define STATUSDIALOG_H

#include <QDialog>

class TableModel;

namespace Ui {
class StatusDialog;
}

class StatusDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StatusDialog(QWidget *parent = 0);
    ~StatusDialog();

private slots:
    void            appendRow(QString action,
                                QString stat,
                                QString err);

private:
    void            initView();
    TableModel      *mModel;
    int             curRow;
    Ui::StatusDialog* ui;
};

#endif // STATUSDIALOG_H
