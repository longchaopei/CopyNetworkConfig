#ifndef EXCELOPERATE_H
#define EXCELOPERATE_H

#include <QObject>

class ExcelOperate
{
public:
    ExcelOperate();

    const static int readSheet(QString path, QList<QList<QVariant>> &retDatas);
    const static int readSheet(QString path, int index, QList<QList<QVariant>> &retDatas);
};

#endif // EXCELOPERATE_H
