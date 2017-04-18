#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "datatype.h"

class QFileDialog;
class TableModel;
class QAxObject;
class HelpDialog;
class VersionDialog;
class CoverInfoDialog;
class VpnDialog;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    const static int mHeaderRow = 0;
    enum CopyStatus {COPY_SUCCESS, COPY_FAIL, COPY_UNKNOWN_ERR};

signals:
    void            appendRowSignal(QString action,
                                    QString stat,
                                    QString err);
    void            coverInfoTableAppendRow(const COUNTRY_INFO, const COUNTRY_INFO);
    void            clearCoverInfoTable();
    void            openCoverDialogSignal();
    void            openVpnDialog();

private slots:
    void            on_startBtn_clicked();

    void            on_srcFileToolBtn_clicked();

    void            on_targetFileToolBtn_clicked();

    void            on_actHelp_triggered();

    void            on_actAbout_triggered();

    void            on_lookupInfoBtn_clicked();

    void on_actionImportVpnConf_triggered();

    void on_tempFunctionRB_toggled(bool checked);

private:
    /**
     * @brief 初始化界面
     */
    void            initView();
    void            clearTableView();
    void            setViewVisible(bool isVisible);
    /**
     * @brief 判断文件是否有效
     */
    bool            assertFile(QString path);
    /**
     * @brief 主拷贝函数
     */
    void            mainCopy();
    /**
     * @brief 临时功能
     */
    void            dotemplateFunction();
    /**
     * @brief 删除多余项
     */
    void            delTaregtSurplusItem();
    /**
     * @brief 读excel
     */
    void            read(QList<QList<QVariant>> &datas);
    /**
     * @brief 写excel
     */
    void            write(QList<QList<QVariant>> &datas);

    QString         getCopyStatusStr(CopyStatus stat);
    void            appendRow(QString action,
                                QString stat,
                                QString err);
    bool            initSrcColumns(QList<QList<QVariant>> &datas);
    bool            initTargetColumns(QList<QList<QVariant>> &datas);
    void            delaymsec(int msec);
    bool            getWorkSheetData(QString path, QList<QList<QVariant>> &datas);
    void            closeEvent(QCloseEvent *event);

    QFileDialog*    mFileDialog;
    TableModel*     mModel;
    HelpDialog*     mHelpDialog;
    VersionDialog*  mVersionDialog;
    CoverInfoDialog* mCoverInfoDialog;
    VpnDialog*      mVpnDialog;

    QString         mSourceFilePath;
    QString         mTargetFilePath;

    int             curRow;
    bool            isStartCopy;
    int             mSrcTownShipColumn;
    int             mSrcVillageColumn;
    int             mSrcIpAddrColumn;
    int             mSrcNetmaskColumn;
    int             mSrcGatewayColumn;
    int             mSrcZoningCodeColumn;
    int             mTargetZoningNameColumn;
    int             mTargetZoningCodeColumn;
    int             mTargetIpAddrColumn;
    int             mTargetNetmaskColumn;
    int             mTargetGatewayColumn;

    //excel操作
    QAxObject*      mSrcWorkBook;
    QAxObject*      mTargetWorkBook;
    QAxObject*      mSrcSheet;
    QAxObject*      mTargetSheet;
    QAxObject*      mSrcRange;
    QAxObject*      mTargetRange;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
