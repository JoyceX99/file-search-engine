#ifndef CLEARDELEGATE
#define CLEARDELEGATE

#include <QPainter>
#include <QAbstractItemDelegate>

class ClearDelegate : public QAbstractItemDelegate
{
    public:
       ClearDelegate(QObject *parent = 0) {};

       void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const {
           QRect r = option.rect;
           QString text = index.data(Qt::DisplayRole).toString();

           r = option.rect.adjusted(15, 0, -10, -40);
           painter->setFont( QFont( "Open Sans", 13, QFont::Bold ) );
           painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignBottom|Qt::AlignLeft, text, &r);
       }

       QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const {
           return QSize(200, 75);
       };

       virtual ~ClearDelegate() {};
};

#endif // CLEARDELEGATE

