#include "dateeditdelegate.h"

#include <QDateTime>

DateEditDelegate::DateEditDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{}

QWidget *DateEditDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const
{
    QDateEdit *editor = new QDateEdit(parent);
    editor->setDisplayFormat("yyyy-MM-dd");
    return editor;
}

void DateEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();

    QDateEdit *fecha = static_cast<QDateEdit *>(editor);
    fecha->setDate(QDate::fromString(value, "yyyy-MM-dd"));
}

void DateEditDelegate::setModelData(QWidget *editor,
                                    QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    QDateEdit *fecha = static_cast<QDateEdit *>(editor);
    QDate value = fecha->date(); //.toString("yyyy-MM-dd");
    model->setData(index, value, Qt::EditRole);
}

void DateEditDelegate::updateEditorGeometry(QWidget *editor,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void DateEditDelegate::paint(QPainter *painter,
                             const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    QDate fecha = QDate(index.model()->data(index, Qt::DisplayRole).toDate());
    painter->drawText(option.rect, Qt::AlignCenter, fecha.toString("yyyy-MM-dd"));
}
