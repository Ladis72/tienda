/****************************************************************************
** Meta object code from reading C++ file 'tiendas.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.14)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../tiendas.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tiendas.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.14. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_tiendas_t {
    QByteArrayData data[11];
    char stringdata0[253];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_tiendas_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_tiendas_t qt_meta_stringdata_tiendas = {
    {
QT_MOC_LITERAL(0, 0, 7), // "tiendas"
QT_MOC_LITERAL(1, 8, 26), // "on_pushButtonNuevo_clicked"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 30), // "on_pushButtonSiguiente_clicked"
QT_MOC_LITERAL(4, 67, 29), // "on_pushButtonAnterior_clicked"
QT_MOC_LITERAL(5, 97, 30), // "on_pushButtonModificar_clicked"
QT_MOC_LITERAL(6, 128, 27), // "on_pushButtonBorrar_clicked"
QT_MOC_LITERAL(7, 156, 30), // "on_pushButtonRefrescar_clicked"
QT_MOC_LITERAL(8, 187, 30), // "on_checkBoxMaster_stateChanged"
QT_MOC_LITERAL(9, 218, 4), // "arg1"
QT_MOC_LITERAL(10, 223, 29) // "on_checkBoxLocal_stateChanged"

    },
    "tiendas\0on_pushButtonNuevo_clicked\0\0"
    "on_pushButtonSiguiente_clicked\0"
    "on_pushButtonAnterior_clicked\0"
    "on_pushButtonModificar_clicked\0"
    "on_pushButtonBorrar_clicked\0"
    "on_pushButtonRefrescar_clicked\0"
    "on_checkBoxMaster_stateChanged\0arg1\0"
    "on_checkBoxLocal_stateChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_tiendas[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x08 /* Private */,
       3,    0,   55,    2, 0x08 /* Private */,
       4,    0,   56,    2, 0x08 /* Private */,
       5,    0,   57,    2, 0x08 /* Private */,
       6,    0,   58,    2, 0x08 /* Private */,
       7,    0,   59,    2, 0x08 /* Private */,
       8,    1,   60,    2, 0x08 /* Private */,
      10,    1,   63,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,

       0        // eod
};

void tiendas::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<tiendas *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_pushButtonNuevo_clicked(); break;
        case 1: _t->on_pushButtonSiguiente_clicked(); break;
        case 2: _t->on_pushButtonAnterior_clicked(); break;
        case 3: _t->on_pushButtonModificar_clicked(); break;
        case 4: _t->on_pushButtonBorrar_clicked(); break;
        case 5: _t->on_pushButtonRefrescar_clicked(); break;
        case 6: _t->on_checkBoxMaster_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->on_checkBoxLocal_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject tiendas::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_tiendas.data,
    qt_meta_data_tiendas,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *tiendas::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *tiendas::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_tiendas.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int tiendas::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
