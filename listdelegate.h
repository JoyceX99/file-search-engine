#ifndef LISTDELEGATE_H
#define LISTDELEGATE_H

#include <QPainter>
#include <QAbstractItemDelegate>

class ListDelegate : public QAbstractItemDelegate
{
    public:
       ListDelegate(QObject *parent = 0);

       void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
       bool editorEvent ( QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) ;
       QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

       void openFile(std::string path) const;

       virtual ~ListDelegate();
};

#endif // LISTDELEGATE_H
