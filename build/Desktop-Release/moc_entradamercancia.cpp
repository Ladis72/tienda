/****************************************************************************
** Meta object code from reading C++ file 'entradamercancia.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.14)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../entradamercancia.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'entradamercancia.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.14. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_EntradaMercancia_t {
    QByteArrayData data[13];
    char stringdata0[282];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_EntradaMercancia_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_EntradaMercancia_t qt_meta_stringdata_EntradaMercancia = {
    {
QT_MOC_LITERAL(0, 0, 16), // "EntradaMercancia"
QT_MOC_LITERAL(1, 17, 28), // "on_pushButtonAceptar_clicked"
QT_MOC_LITERAL(2, 46, 0), // ""
QT_MOC_LITERAL(3, 47, 28), // "on_lineEditCod_returnPressed"
QT_MOC_LITERAL(4, 76, 33), // "on_pushButtonAgregarLinea_cli..."
QT_MOC_LITERAL(5, 110, 29), // "on_lineEditDesc_returnPressed"
QT_MOC_LITERAL(6, 140, 27), // "on_pushButtonBorrar_clicked"
QT_MOC_LITERAL(7, 168, 20), // "on_tableView_clicked"
QT_MOC_LITERAL(8, 189, 11), // "QModelIndex"
QT_MOC_LITERAL(9, 201, 5), // "index"
QT_MOC_LITERAL(10, 207, 36), // "on_dateEditCaducidad_editingF..."
QT_MOC_LITERAL(11, 244, 32), // "on_comboBoxProcedencia_activated"
QT_MOC_LITERAL(12, 277, 4) // "arg1"

    },
    "EntradaMercancia\0on_pushButtonAceptar_clicked\0"
    "\0on_lineEditCod_returnPressed\0"
    "on_pushButtonAgregarLinea_clicked\0"
    "on_lineEditDesc_returnPressed\0"
    "on_pushButtonBorrar_clicked\0"
    "on_tableView_clicked\0QModelIndex\0index\0"
    "on_dateEditCaducidad_editingFinished\0"
    "on_comboBoxProcedencia_activated\0arg1"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_EntradaMercancia[] = {

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
       7,    1,   59,    2, 0x08 /* Private */,
      10,    0,   62,    2, 0x08 /* Private */,
      11,    1,   63,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   12,

       0        // eod
};

void EntradaMercancia::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<EntradaMercancia *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_pushButtonAceptar_clicked(); break;
        case 1: _t->on_lineEditCod_returnPressed(); break;
        case 2: _t->on_pushButtonAgregarLinea_clicked(); break;
        case 3: _t->on_lineEditDesc_returnPressed(); break;
        case 4: _t->on_pushButtonBorrar_clicked(); break;
        case 5: _t->on_tableView_clicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 6: _t->on_dateEditCaducidad_editingFinished(); break;
        case 7: _t->on_comboBoxProcedencia_activated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject EntradaMercancia::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_EntradaMercancia.data,
    qt_meta_data_EntradaMercancia,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *EntradaMercancia::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EntradaMercancia::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_EntradaMercancia.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int EntradaMercancia::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
