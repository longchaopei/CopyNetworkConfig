#ifndef VPNDIALOG_H
#define VPNDIALOG_H

#include <QDialog>

class TableModel;
class QFileDialog;
class WaitDialog;

namespace Ui {
class VpnDialog;
}

class VpnDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VpnDialog(QWidget *parent = 0);
    ~VpnDialog();

signals:
    void            openWaitDialog();
    void            closeWaitDialog();

private slots:
    void on_srcFileToolBtn_clicked();

    void on_targetFileToolBtn_clicked();

    void on_startBtn_clicked();

    void openWindow();

private:
    void            initView();
    bool            initSrcColumn(const QList<QList<QVariant> > &villageDatas,
                                  const QList<QList<QVariant> > &countryDatas);
    int             insertVpnSetting(const QList<QList<QVariant>> srcVillageDatas,
                                     const QList<QList<QVariant>> srcCountryDatas);
    bool            assertFile(QString path);
    void            setViewEnable(bool enable);
    void            delaymsec(int msec);

    WaitDialog*     mWaitDialog;
    QFileDialog*    mFileDialog;
    TableModel*     mModel;
    QString         mSourceFilePath;
    QString         mTargetFilePath;
    int             mCountrySubnetColumn;
    int             mCountrySubnetmaskColumn;
    int             mVillageSubnetmaskColumn;
    int             mVillageSubnetColumn;
    int             mVillageIpColumn;
    bool            isStart;

    Ui::VpnDialog *ui;
};

#endif // VPNDIALOG_H
