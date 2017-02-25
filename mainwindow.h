#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QFileDialog;
class TableModel;

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
    QFileDialog*    mFileDialog;
    TableModel      *mModel;
    QString         mSourceFilePath;
    QString         mTargetFilePath;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
