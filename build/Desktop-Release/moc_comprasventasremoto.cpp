/****************************************************************************
** Meta object code from reading C++ file 'comprasventasremoto.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.14)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../comprasventasremoto.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'comprasventasremoto.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.14. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_comprasVentasRemoto_t {
    QByteArrayData data[8];
    char stringdata0[215];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_comprasVentasRemoto_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_comprasVentasRemoto_t qt_meta_stringdata_comprasVentasRemoto = {
    {
QT_MOC_LITERAL(0, 0, 19), // "comprasVentasRemoto"
QT_MOC_LITERAL(1, 20, 31), // "on_radioButtonVentasDia_clicked"
QT_MOC_LITERAL(2, 52, 0), // ""
QT_MOC_LITERAL(3, 53, 30), // "on_radioButtonVentaMes_clicked"
QT_MOC_LITERAL(4, 84, 31), // "on_radioButtonVentasAno_clicked"
QT_MOC_LITERAL(5, 116, 32), // "on_radioButtonComprasDia_clicked"
QT_MOC_LITERAL(6, 149, 32), // "on_radioButtonComprasMes_clicked"
QT_MOC_LITERAL(7, 182, 32) // "on_radioButtonComprasano_clicked"

    },
    "comprasVentasRemoto\0on_radioButtonVentasDia_clicked\0"
    "\0on_radioButtonVentaMes_clicked\0"
    "on_radioButtonVentasAno_clicked\0"
    "on_radioButtonComprasDia_clicked\0"
    "on_radioButtonComprasMes_clicked\0"
    "on_radioButtonComprasano_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_comprasVentasRemoto[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x08 /* Private */,
       3,    0,   45,    2, 0x08 /* Private */,
       4,    0,   46,    2, 0x08 /* Private */,
       5,    0,   47,    2, 0x08 /* Private */,
       6,    0,   48,    2, 0x08 /* Private */,
       7,    0,   49,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void comprasVentasRemoto::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<comprasVentasRemoto *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_radioButtonVentasDia_clicked(); break;
        case 1: _t->on_radioButtonVentaMes_clicked(); break;
        case 2: _t->on_radioButtonVentasAno_clicked(); break;
        case 3: _t->on_radioButtonComprasDia_clicked(); break;
        case 4: _t->on_radioButtonComprasMes_clicked(); break;
        case 5: _t->on_radioButtonComprasano_clicked(); break;
        default: ;
        }
    }
    (void)_a;
}

QT_INIT_METAOBJECT const QMetaObject comprasVentasRemoto::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_comprasVentasRemoto.data,
    qt_meta_data_comprasVentasRemoto,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *comprasVentasRemoto::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *comprasVentasRemoto::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_comprasVentasRemoto.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int comprasVentasRemoto::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
