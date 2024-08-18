/****************************************************************************
** Meta object code from reading C++ file 'totalizar.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.14)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../totalizar.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'totalizar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.14. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_totalizar_t {
    QByteArrayData data[12];
    char stringdata0[263];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_totalizar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_totalizar_t qt_meta_stringdata_totalizar = {
    {
QT_MOC_LITERAL(0, 0, 9), // "totalizar"
QT_MOC_LITERAL(1, 10, 27), // "on_pushButtonTicket_clicked"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 32), // "on_lineEditDescuento_textChanged"
QT_MOC_LITERAL(4, 72, 4), // "arg1"
QT_MOC_LITERAL(5, 77, 30), // "on_lineEditEntrega_textChanged"
QT_MOC_LITERAL(6, 108, 30), // "on_comboBox_currentTextChanged"
QT_MOC_LITERAL(7, 139, 31), // "on_comboBox_currentIndexChanged"
QT_MOC_LITERAL(8, 171, 5), // "index"
QT_MOC_LITERAL(9, 177, 27), // "on_pushButtonCobrar_clicked"
QT_MOC_LITERAL(10, 205, 28), // "on_pushButtonFactura_clicked"
QT_MOC_LITERAL(11, 234, 28) // "on_checkBoxVale_stateChanged"

    },
    "totalizar\0on_pushButtonTicket_clicked\0"
    "\0on_lineEditDescuento_textChanged\0"
    "arg1\0on_lineEditEntrega_textChanged\0"
    "on_comboBox_currentTextChanged\0"
    "on_comboBox_currentIndexChanged\0index\0"
    "on_pushButtonCobrar_clicked\0"
    "on_pushButtonFactura_clicked\0"
    "on_checkBoxVale_stateChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_totalizar[] = {

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
       5,    1,   58,    2, 0x08 /* Private */,
       6,    1,   61,    2, 0x08 /* Private */,
       7,    1,   64,    2, 0x08 /* Private */,
       9,    0,   67,    2, 0x08 /* Private */,
      10,    0,   68,    2, 0x08 /* Private */,
      11,    1,   69,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,

       0        // eod
};

void totalizar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<totalizar *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_pushButtonTicket_clicked(); break;
        case 1: _t->on_lineEditDescuento_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->on_lineEditEntrega_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->on_comboBox_currentTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->on_comboBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->on_pushButtonCobrar_clicked(); break;
        case 6: _t->on_pushButtonFactura_clicked(); break;
        case 7: _t->on_checkBoxVale_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject totalizar::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_totalizar.data,
    qt_meta_data_totalizar,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *totalizar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *totalizar::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_totalizar.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int totalizar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
