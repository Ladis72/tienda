/****************************************************************************
** Meta object code from reading C++ file 'proveedores.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../proveedores.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'proveedores.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN11ProveedoresE_t {};
} // unnamed namespace

template <> constexpr inline auto Proveedores::qt_create_metaobjectdata<qt_meta_tag_ZN11ProveedoresE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Proveedores",
        "on_pushButtonNuevo_clicked",
        "",
        "on_pushButtonAnterior_clicked",
        "on_pushButtonSiguiente_clicked",
        "on_lineEditNombre_returnPressed",
        "on_pushButtonModificar_clicked",
        "on_lineEditFormaPago_textChanged",
        "arg1",
        "on_pushButtonFPago_clicked",
        "on_lineEditCod_editingFinished",
        "on_pushButton_clicked",
        "on_radioButtonComprasFacturas_clicked",
        "on_radioButtonComprasMeses_clicked",
        "on_radioButtonComprasAnos_clicked",
        "on_pushButtonBorrar_clicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'on_pushButtonNuevo_clicked'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonAnterior_clicked'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonSiguiente_clicked'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_lineEditNombre_returnPressed'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonModificar_clicked'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_lineEditFormaPago_textChanged'
        QtMocHelpers::SlotData<void(const QString &)>(7, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 8 },
        }}),
        // Slot 'on_pushButtonFPago_clicked'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_lineEditCod_editingFinished'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButton_clicked'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_radioButtonComprasFacturas_clicked'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_radioButtonComprasMeses_clicked'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_radioButtonComprasAnos_clicked'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonBorrar_clicked'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Proveedores, qt_meta_tag_ZN11ProveedoresE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Proveedores::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11ProveedoresE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11ProveedoresE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11ProveedoresE_t>.metaTypes,
    nullptr
} };

void Proveedores::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Proveedores *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->on_pushButtonNuevo_clicked(); break;
        case 1: _t->on_pushButtonAnterior_clicked(); break;
        case 2: _t->on_pushButtonSiguiente_clicked(); break;
        case 3: _t->on_lineEditNombre_returnPressed(); break;
        case 4: _t->on_pushButtonModificar_clicked(); break;
        case 5: _t->on_lineEditFormaPago_textChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->on_pushButtonFPago_clicked(); break;
        case 7: _t->on_lineEditCod_editingFinished(); break;
        case 8: _t->on_pushButton_clicked(); break;
        case 9: _t->on_radioButtonComprasFacturas_clicked(); break;
        case 10: _t->on_radioButtonComprasMeses_clicked(); break;
        case 11: _t->on_radioButtonComprasAnos_clicked(); break;
        case 12: _t->on_pushButtonBorrar_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject *Proveedores::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Proveedores::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11ProveedoresE_t>.strings))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int Proveedores::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 13;
    }
    return _id;
}
QT_WARNING_POP
