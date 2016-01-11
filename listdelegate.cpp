#include "listdelegate.h"

ListDelegate::ListDelegate(QObject *parent)
: QAbstractItemDelegate(parent) {}

void ListDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const {
    QRect r = option.rect;

    QPen rectPen(Qt::NoPen);
    QPen fontPen(QColor::fromRgb(51,51,51), 1, Qt::SolidLine);

    if (index.data(Qt::WhatsThisRole) == "AlertNone") { //no docs found
        r = option.rect.adjusted(10, 0, -10, -48);
        painter->setFont( QFont( "Open Sans", 13, QFont::Normal ) );
        painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignBottom|Qt::AlignLeft, "No Documents Found", &r);
    } else {
        if(option.state & QStyle::State_Selected) {
            painter->setBrush(QColor(230, 230, 230));
        } else {
            //BACKGROUND ALTERNATING COLORS
            painter->setBrush( (index.row() % 2) ? Qt::white : QColor(250,250,250) );
        }

        painter->setPen(rectPen);
        painter->drawRect(r);
        painter->setPen(fontPen);
        //painter->drawLine(QPointF(r.left(), r.top()+r.height()), QPointF(r.left()+r.width(), r.top()+r.height()));

        //GET FILE, PREVIEW
        QString file = index.data(Qt::DisplayRole).toString();
        QString preview = index.data(Qt::UserRole).toString();

        int padding = 10;

        //FILE
        r = option.rect.adjusted(padding, 0, -10, -42);
        painter->setFont( QFont( "Open Sans", 15, QFont::Bold ) );
        painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignBottom|Qt::AlignLeft, file, &r);

        //PREVIEW
        r = option.rect.adjusted(padding, 38, -10, 0);
        painter->setFont( QFont( "Open Sans", 12, QFont::Normal ) );
        painter->drawText(r.left(), r.top(), r.width(), r.height()-5, Qt::TextWordWrap|Qt::ElideRight, preview, &r); //Qt::TextWordWrap
    }
}

QSize ListDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const {
   return QSize(200, 75); // very dumb value
}

ListDelegate::~ListDelegate() {

}
