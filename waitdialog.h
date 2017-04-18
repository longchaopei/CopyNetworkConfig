#ifndef WAITDIALOG_H
#define WAITDIALOG_H

#include <QDialog>

class QMovie;

namespace Ui {
class WaitDialog;
}

class WaitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WaitDialog(QWidget *parent = 0);
    ~WaitDialog();

private slots:
    void openWindow();
    void closeWindow();

private:
    QMovie *movie;
    Ui::WaitDialog *ui;
};

#endif // WAITDIALOG_H
