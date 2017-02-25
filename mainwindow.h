#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QFileDialog;
class TableModel;
class QAxObject;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_startBtn_clicked();

    void on_srcFileToolBtn_clicked();

    void on_targetFileToolBtn_clicked();

private:
    /**
     * @brief 初始化界面
     */
    void            initView();
    /**
     * @brief 主拷贝函数
     */
    void            mainCopy();
    /**
     * @brief 判断文件是否有效
     */
    bool            assertFile(QString path);
    QFileDialog*    mFileDialog;
    TableModel      *mModel;
    QString         mSourceFilePath;
    QString         mTargetFilePath;

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
