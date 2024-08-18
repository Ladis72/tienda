/****************************************************************************
** Meta object code from reading C++ file 'familias.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.14)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../familias.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'familias.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.14. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Familias_t {
    QByteArrayData data[11];
    char stringdata0[195];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Familias_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Familias_t qt_meta_stringdata_Familias = {
    {
QT_MOC_LITERAL(0, 0, 8), // "Familias"
QT_MOC_LITERAL(1, 9, 27), // "on_pushButtonAnadir_clicked"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 30), // "on_pushButtonModificar_clicked"
QT_MOC_LITERAL(4, 69, 29), // "on_pushButtonDeshacer_clicked"
QT_MOC_LITERAL(5, 99, 27), // "on_pushButtonBorrar_clicked"
QT_MOC_LITERAL(6, 127, 20), // "on_tableView_clicked"
QT_MOC_LITERAL(7, 148, 11), // "QModelIndex"
QT_MOC_LITERAL(8, 160, 5), // "index"
QT_MOC_LITERAL(9, 166, 23), // "on_lineEdit_textChanged"
QT_MOC_LITERAL(10, 190, 4) // "arg1"

    },
    "Familias\0on_pushButtonAnadir_clicked\0"
    "\0on_pushButtonModificar_clicked\0"
    "on_pushButtonDeshacer_clicked\0"
    "on_pushButtonBorrar_clicked\0"
    "on_tableView_clicked\0QModelIndex\0index\0"
    "on_lineEdit_textChanged\0arg1"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Familias[] = {

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
       6,    1,   48,    2, 0x08 /* Private */,
       9,    1,   51,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, QMetaType::QString,   10,

       0        // eod
};

void Familias::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Familias *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_pushButtonAnadir_clicked(); break;
        case 1: _t->on_pushButtonModificar_clicked(); break;
        case 2: _t->on_pushButtonDeshacer_clicked(); break;
        case 3: _t->on_pushButtonBorrar_clicked(); break;
        case 4: _t->on_tableView_clicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 5: _t->on_lineEdit_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Familias::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_Familias.data,
    qt_meta_data_Familias,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Familias::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Familias::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Familias.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int Familias::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
