/****************************************************************************
** Meta object code from reading C++ file 'directorios.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.14)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../directorios.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'directorios.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.14. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Directorios_t {
    QByteArrayData data[13];
    char stringdata0[291];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Directorios_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Directorios_t qt_meta_stringdata_Directorios = {
    {
QT_MOC_LITERAL(0, 0, 11), // "Directorios"
QT_MOC_LITERAL(1, 12, 28), // "on_pushButtonAceptar_clicked"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 28), // "on_toolButtonFactura_clicked"
QT_MOC_LITERAL(4, 71, 27), // "on_toolButtonVentas_clicked"
QT_MOC_LITERAL(5, 99, 30), // "on_toolButtonEtiquetas_clicked"
QT_MOC_LITERAL(6, 130, 30), // "on_toolButtonCaducados_clicked"
QT_MOC_LITERAL(7, 161, 28), // "on_toolButtonArqueos_clicked"
QT_MOC_LITERAL(8, 190, 32), // "on_toolButtonMovimientos_clicked"
QT_MOC_LITERAL(9, 223, 27), // "on_toolButtonCseg_triggered"
QT_MOC_LITERAL(10, 251, 8), // "QAction*"
QT_MOC_LITERAL(11, 260, 4), // "arg1"
QT_MOC_LITERAL(12, 265, 25) // "on_toolButtonCseg_clicked"

    },
    "Directorios\0on_pushButtonAceptar_clicked\0"
    "\0on_toolButtonFactura_clicked\0"
    "on_toolButtonVentas_clicked\0"
    "on_toolButtonEtiquetas_clicked\0"
    "on_toolButtonCaducados_clicked\0"
    "on_toolButtonArqueos_clicked\0"
    "on_toolButtonMovimientos_clicked\0"
    "on_toolButtonCseg_triggered\0QAction*\0"
    "arg1\0on_toolButtonCseg_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Directorios[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x08 /* Private */,
       3,    0,   60,    2, 0x08 /* Private */,
       4,    0,   61,    2, 0x08 /* Private */,
       5,    0,   62,    2, 0x08 /* Private */,
       6,    0,   63,    2, 0x08 /* Private */,
       7,    0,   64,    2, 0x08 /* Private */,
       8,    0,   65,    2, 0x08 /* Private */,
       9,    1,   66,    2, 0x08 /* Private */,
      12,    0,   69,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void,

       0        // eod
};

void Directorios::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Directorios *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_pushButtonAceptar_clicked(); break;
        case 1: _t->on_toolButtonFactura_clicked(); break;
        case 2: _t->on_toolButtonVentas_clicked(); break;
        case 3: _t->on_toolButtonEtiquetas_clicked(); break;
        case 4: _t->on_toolButtonCaducados_clicked(); break;
        case 5: _t->on_toolButtonArqueos_clicked(); break;
        case 6: _t->on_toolButtonMovimientos_clicked(); break;
        case 7: _t->on_toolButtonCseg_triggered((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 8: _t->on_toolButtonCseg_clicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Directorios::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_Directorios.data,
    qt_meta_data_Directorios,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Directorios::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Directorios::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Directorios.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int Directorios::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
