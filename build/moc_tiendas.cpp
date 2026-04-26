/****************************************************************************
** Meta object code from reading C++ file 'tiendas.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../tiendas.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tiendas.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN7tiendasE_t {};
} // unnamed namespace

template <> constexpr inline auto tiendas::qt_create_metaobjectdata<qt_meta_tag_ZN7tiendasE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "tiendas",
        "on_pushButtonNuevo_clicked",
        "",
        "on_pushButtonGuardar_clicked",
        "on_pushButtonCancelar_clicked",
        "on_pushButtonBorrar_clicked",
        "on_pushButtonRefrescar_clicked",
        "on_pushButtonSslCa_clicked",
        "on_checkBoxMaster_stateChanged",
        "arg1",
        "on_checkBoxLocal_stateChanged",
        "on_tableViewTiendas_clicked",
        "QModelIndex",
        "index"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'on_pushButtonNuevo_clicked'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonGuardar_clicked'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonCancelar_clicked'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonBorrar_clicked'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonRefrescar_clicked'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonSslCa_clicked'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_checkBoxMaster_stateChanged'
        QtMocHelpers::SlotData<void(int)>(8, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
        // Slot 'on_checkBoxLocal_stateChanged'
        QtMocHelpers::SlotData<void(int)>(10, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
        // Slot 'on_tableViewTiendas_clicked'
        QtMocHelpers::SlotData<void(const QModelIndex &)>(11, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 12, 13 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<tiendas, qt_meta_tag_ZN7tiendasE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject tiendas::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7tiendasE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7tiendasE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7tiendasE_t>.metaTypes,
    nullptr
} };

void tiendas::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<tiendas *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->on_pushButtonNuevo_clicked(); break;
        case 1: _t->on_pushButtonGuardar_clicked(); break;
        case 2: _t->on_pushButtonCancelar_clicked(); break;
        case 3: _t->on_pushButtonBorrar_clicked(); break;
        case 4: _t->on_pushButtonRefrescar_clicked(); break;
        case 5: _t->on_pushButtonSslCa_clicked(); break;
        case 6: _t->on_checkBoxMaster_stateChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->on_checkBoxLocal_stateChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->on_tableViewTiendas_clicked((*reinterpret_cast<std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *tiendas::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *tiendas::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7tiendasE_t>.strings))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int tiendas::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
