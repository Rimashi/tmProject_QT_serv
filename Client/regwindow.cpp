#include "regwindow.h"

RegWindow::RegWindow(QObject *parent)
    : QAbstractItemModel(parent)
{}

QVariant RegWindow::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

QModelIndex RegWindow::index(int row, int column, const QModelIndex &parent) const
{
    // FIXME: Implement me!
}

QModelIndex RegWindow::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
}

int RegWindow::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

int RegWindow::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

QVariant RegWindow::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}
