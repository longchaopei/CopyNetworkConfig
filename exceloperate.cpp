#include "exceloperate.h"
#include <QAxObject>
#include <errno.h>
#include <QFile>

ExcelOperate::ExcelOperate()
{

}

const int
ExcelOperate::readSheet(QString path, QList<QList<QVariant> > &retDatas)
{
    if (!QFile::exists(path))
        return -ENOFILE;

    QAxObject excel("Excel.Application");
    excel.setProperty("Visible", false);                                //不显示文档
    excel.querySubObject("WorkBooks")
            ->dynamicCall("Open (const QString&)", path);
//    mSrcWorkBook = excel.querySubObject("ActiveWorkBook");           //获取活动工作簿
//    mSrcSheet = mSrcWorkBook->querySubObject("Worksheets(int)", 1);  //获取第一个表
//    mSrcRange = mSrcSheet->querySubObject("UsedRange");

    QVariant vars =  excel.querySubObject("ActiveWorkBook")
            ->querySubObject("Worksheets(int)", 1)
            ->querySubObject("UsedRange")
            ->dynamicCall("Value");                                     //获取整表数据
    excel.querySubObject("ActiveWorkBook")->dynamicCall("Close(Boolean)", false);                 //关闭表
    excel.dynamicCall("Quit(void)");                                    //释放excel

    QVariantList varRows = vars.toList();
    QVariantList rowData;
    if (!varRows.isEmpty()) {
        int rowCount = varRows.size();
        retDatas.clear();
        for (int i = 0; i < rowCount; i++) {
            rowData = varRows[i].toList();
            //将每一行数据push到retDatas里
            retDatas.push_back(rowData);
        }
    } else {
        return 0;
    }

    return retDatas.size();
}

const int
ExcelOperate::readSheet(QString path, int index, QList<QList<QVariant> > &retDatas)
{

    if (!QFile::exists(path))
        return -ENOFILE;

    if (index <= 0)
        return -EINVAL;

    QAxObject excel("Excel.Application");
    excel.setProperty("Visible", false);                                //不显示文档
    excel.querySubObject("WorkBooks")
            ->dynamicCall("Open (const QString&)", path);
//    mSrcWorkBook = excel.querySubObject("ActiveWorkBook");           //获取活动工作簿
//    mSrcSheet = mSrcWorkBook->querySubObject("Worksheets(int)", 1);  //获取第一个表
//    mSrcRange = mSrcSheet->querySubObject("UsedRange");

    int sheetCount = excel.querySubObject("ActiveWorkBook")
            ->querySubObject("Sheets")
            ->property("Count").toInt();
    if (index > sheetCount)
        return -EINVAL;

    QVariant vars =  excel.querySubObject("ActiveWorkBook")
            ->querySubObject("Worksheets(int)", index)
            ->querySubObject("UsedRange")
            ->dynamicCall("Value");                                     //获取整表数据
    excel.querySubObject("ActiveWorkBook")->dynamicCall("Close(Boolean)", false);                 //关闭表
    excel.dynamicCall("Quit(void)");                                    //释放excel

    QVariantList varRows = vars.toList();
    QVariantList rowData;
    if (!varRows.isEmpty()) {
        int rowCount = varRows.size();
        retDatas.clear();
        for (int i = 0; i < rowCount; i++) {
            rowData = varRows[i].toList();
            //将每一行数据push到retDatas里
            retDatas.push_back(rowData);
        }
    } else {
        return 0;
    }

    return retDatas.size();
}
