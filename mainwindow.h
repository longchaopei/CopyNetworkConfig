#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QFileDialog;
class TableModel;
class QAxObject;
class StatusDialog;

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

private:
    /**
     * @brief 初始化界面
     */
    void            initView();
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

    int             curRow;
    bool            isStartCopy;
    QFileDialog*    mFileDialog;
    TableModel      *mModel;
    QString         mSourceFilePath;
    QString         mTargetFilePath;
    StatusDialog*   mStatusDialog;

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
