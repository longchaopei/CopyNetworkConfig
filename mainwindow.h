#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QFileDialog;
class TableModel;
class QAxObject;
class HelpDialog;
class VersionDialog;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    enum CopyStatus {COPY_SUCCESS, COPY_FAIL, COPY_UNKNOWN_ERR};

signals:
    void            appendRowSignal(QString action,
                                    QString stat,
                                    QString err);

private slots:
    void on_startBtn_clicked();

    void on_srcFileToolBtn_clicked();

    void on_targetFileToolBtn_clicked();

    void on_actHelp_triggered();

    void on_actAbout_triggered();

private:
    /**
     * @brief 初始化界面
     */
    void            initView();
    void            clearTableView();
    void            setViewVisible(bool isVisible);
    /**
     * @brief 主拷贝函数
     */
    void            mainCopy();
    /**
     * @brief 读excel
     */
    void            read(QList<QList<QVariant>> &datas);
    /**
     * @brief 写excel
     */
    void            write(QList<QList<QVariant>> &datas);
    /**
     * @brief 判断文件是否有效
     */
    bool            assertFile(QString path);

    QString         getCopyStatusStr(CopyStatus stat);
    void            appendRow(QString action,
                                QString stat,
                                QString err);
    bool            initSrcColumns(QList<QList<QVariant>> &datas);
    bool            initTargetColumns(QList<QList<QVariant>> &datas);
    void            delaymsec(int msec);

    QFileDialog*    mFileDialog;
    TableModel*     mModel;
    HelpDialog*     mHelpDialog;
    VersionDialog*  mVersionDialog;

    QString         mSourceFilePath;
    QString         mTargetFilePath;

    int             curRow;
    bool            isStartCopy;
    int             mSrcTownShipColumn;
    int             mSrcVillageColumn;
    int             mSrcIpAddrColumn;
    int             mSrcNetmaskColumn;
    int             mSrcGatewayColumn;
    int             mZoningNameColumn;
    int             mZoningCodeColumn;
    int             mTargetIpAddrColumn;
    int             mTargetNetmaskColumn;
    int             mTargetGatewayColumn;
    const static int mHeaderRow = 0;

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
