#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QStandardItemModel>

/**
 * @brief The TableModel class:委托，设置居中显示
 */
class TableModel : public QStandardItemModel
{
    Q_OBJECT
public:
    TableModel(QObject *parent=NULL) : QStandardItemModel(parent) { }
    TableModel(int row, int column, QObject *parent=NULL)
        : QStandardItemModel(row, column, parent) { }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
    {
        if( Qt::TextAlignmentRole == role )
            return Qt::AlignCenter;
        return QStandardItemModel::data(index, role);
    }
};

#endif // TABLEMODEL_H
