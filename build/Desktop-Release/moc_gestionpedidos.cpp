/****************************************************************************
** Meta object code from reading C++ file 'gestionpedidos.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.14)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../gestionpedidos.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gestionpedidos.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.14. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GestionPedidos_t {
    QByteArrayData data[12];
    char stringdata0[236];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GestionPedidos_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GestionPedidos_t qt_meta_stringdata_GestionPedidos = {
    {
QT_MOC_LITERAL(0, 0, 14), // "GestionPedidos"
QT_MOC_LITERAL(1, 15, 26), // "on_pushButtonNuevo_clicked"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 20), // "on_tableView_clicked"
QT_MOC_LITERAL(4, 64, 11), // "QModelIndex"
QT_MOC_LITERAL(5, 76, 5), // "index"
QT_MOC_LITERAL(6, 82, 22), // "on_tableView_activated"
QT_MOC_LITERAL(7, 105, 27), // "on_pushButtonBorrar_clicked"
QT_MOC_LITERAL(8, 133, 30), // "on_pushButtonGestionar_clicked"
QT_MOC_LITERAL(9, 164, 15), // "borrarVariables"
QT_MOC_LITERAL(10, 180, 28), // "on_pushButtonAceptar_clicked"
QT_MOC_LITERAL(11, 209, 26) // "on_tableView_doubleClicked"

    },
    "GestionPedidos\0on_pushButtonNuevo_clicked\0"
    "\0on_tableView_clicked\0QModelIndex\0"
    "index\0on_tableView_activated\0"
    "on_pushButtonBorrar_clicked\0"
    "on_pushButtonGestionar_clicked\0"
    "borrarVariables\0on_pushButtonAceptar_clicked\0"
    "on_tableView_doubleClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GestionPedidos[] = {

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
       3,    1,   55,    2, 0x08 /* Private */,
       6,    1,   58,    2, 0x08 /* Private */,
       7,    0,   61,    2, 0x08 /* Private */,
       8,    0,   62,    2, 0x08 /* Private */,
       9,    0,   63,    2, 0x08 /* Private */,
      10,    0,   64,    2, 0x08 /* Private */,
      11,    1,   65,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,

       0        // eod
};

void GestionPedidos::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GestionPedidos *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_pushButtonNuevo_clicked(); break;
        case 1: _t->on_tableView_clicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 2: _t->on_tableView_activated((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 3: _t->on_pushButtonBorrar_clicked(); break;
        case 4: _t->on_pushButtonGestionar_clicked(); break;
        case 5: _t->borrarVariables(); break;
        case 6: _t->on_pushButtonAceptar_clicked(); break;
        case 7: _t->on_tableView_doubleClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject GestionPedidos::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_GestionPedidos.data,
    qt_meta_data_GestionPedidos,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GestionPedidos::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GestionPedidos::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GestionPedidos.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int GestionPedidos::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
