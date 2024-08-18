/****************************************************************************
** Meta object code from reading C++ file 'verfacturas.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.14)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../verfacturas.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'verfacturas.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.14. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VerFacturas_t {
    QByteArrayData data[10];
    char stringdata0[180];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VerFacturas_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VerFacturas_t qt_meta_stringdata_VerFacturas = {
    {
QT_MOC_LITERAL(0, 0, 11), // "VerFacturas"
QT_MOC_LITERAL(1, 12, 35), // "on_checkBoxTodosProveedores_c..."
QT_MOC_LITERAL(2, 48, 0), // ""
QT_MOC_LITERAL(3, 49, 7), // "checked"
QT_MOC_LITERAL(4, 57, 24), // "on_pushButtonVer_clicked"
QT_MOC_LITERAL(5, 82, 26), // "on_tableView_doubleClicked"
QT_MOC_LITERAL(6, 109, 11), // "QModelIndex"
QT_MOC_LITERAL(7, 121, 5), // "index"
QT_MOC_LITERAL(8, 127, 20), // "on_tableView_clicked"
QT_MOC_LITERAL(9, 148, 31) // "on_pushButtonVerFactura_clicked"

    },
    "VerFacturas\0on_checkBoxTodosProveedores_clicked\0"
    "\0checked\0on_pushButtonVer_clicked\0"
    "on_tableView_doubleClicked\0QModelIndex\0"
    "index\0on_tableView_clicked\0"
    "on_pushButtonVerFactura_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VerFacturas[] = {

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
       1,    1,   39,    2, 0x08 /* Private */,
       4,    0,   42,    2, 0x08 /* Private */,
       5,    1,   43,    2, 0x08 /* Private */,
       8,    1,   46,    2, 0x08 /* Private */,
       9,    0,   49,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void,

       0        // eod
};

void VerFacturas::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VerFacturas *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_checkBoxTodosProveedores_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->on_pushButtonVer_clicked(); break;
        case 2: _t->on_tableView_doubleClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 3: _t->on_tableView_clicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 4: _t->on_pushButtonVerFactura_clicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VerFacturas::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_VerFacturas.data,
    qt_meta_data_VerFacturas,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *VerFacturas::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VerFacturas::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VerFacturas.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int VerFacturas::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
