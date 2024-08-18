/****************************************************************************
** Meta object code from reading C++ file 'historicotickets.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.14)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../historicotickets.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'historicotickets.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.14. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HistoricoTickets_t {
    QByteArrayData data[9];
    char stringdata0[195];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HistoricoTickets_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HistoricoTickets_t qt_meta_stringdata_HistoricoTickets = {
    {
QT_MOC_LITERAL(0, 0, 16), // "HistoricoTickets"
QT_MOC_LITERAL(1, 17, 30), // "on_pushButtonConsultar_clicked"
QT_MOC_LITERAL(2, 48, 0), // ""
QT_MOC_LITERAL(3, 49, 29), // "on_tableViewTickets_activated"
QT_MOC_LITERAL(4, 79, 11), // "QModelIndex"
QT_MOC_LITERAL(5, 91, 5), // "index"
QT_MOC_LITERAL(6, 97, 29), // "on_pushButtonImprimir_clicked"
QT_MOC_LITERAL(7, 127, 30), // "on_pushButtonFormaPago_clicked"
QT_MOC_LITERAL(8, 158, 36) // "on_pushButtonImprimirFactura_..."

    },
    "HistoricoTickets\0on_pushButtonConsultar_clicked\0"
    "\0on_tableViewTickets_activated\0"
    "QModelIndex\0index\0on_pushButtonImprimir_clicked\0"
    "on_pushButtonFormaPago_clicked\0"
    "on_pushButtonImprimirFactura_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HistoricoTickets[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x08 /* Private */,
       3,    1,   40,    2, 0x08 /* Private */,
       6,    0,   43,    2, 0x08 /* Private */,
       7,    0,   44,    2, 0x08 /* Private */,
       8,    0,   45,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void HistoricoTickets::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<HistoricoTickets *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_pushButtonConsultar_clicked(); break;
        case 1: _t->on_tableViewTickets_activated((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 2: _t->on_pushButtonImprimir_clicked(); break;
        case 3: _t->on_pushButtonFormaPago_clicked(); break;
        case 4: _t->on_pushButtonImprimirFactura_clicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject HistoricoTickets::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_HistoricoTickets.data,
    qt_meta_data_HistoricoTickets,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *HistoricoTickets::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HistoricoTickets::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HistoricoTickets.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int HistoricoTickets::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
