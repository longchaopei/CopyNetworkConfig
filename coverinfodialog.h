#ifndef COVERINFODIALOG_H
#define COVERINFODIALOG_H

#include <QDialog>
#include "datatype.h"

class TableModel;
class QStandardItem;

namespace Ui {
class CoverInfoDialog;
}

class CoverInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CoverInfoDialog(QWidget *parent = 0);
    ~CoverInfoDialog();

private slots:
    void    appendRow(const COUNTRY_INFO before, const COUNTRY_INFO after);
    void    clearTable();
    void    openWindow();

private:
    void    initView();
    void    initBeforeTableview();
    void    initAfterTableview();
    void    beforeTableAppendRow(const COUNTRY_INFO before);
    void    afterTableAppendRow(const COUNTRY_INFO after);
    bool    initItems(QList<QStandardItem*> &items, const COUNTRY_INFO info);

    int             mRowCount;
    TableModel*     mBeforeModel;
    TableModel*     mAfterModel;

    Ui::CoverInfoDialog *ui;
};

#endif // COVERINFODIALOG_H
