/****************************************************************************
** Meta object code from reading C++ file 'clientes.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../clientes.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'clientes.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN8ClientesE_t {};
} // unnamed namespace

template <> constexpr inline auto Clientes::qt_create_metaobjectdata<qt_meta_tag_ZN8ClientesE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Clientes",
        "on_pushButtonAnterior_clicked",
        "",
        "on_pushButtonNuevo_clicked",
        "on_pushButtonModificar_clicked",
        "on_pushButtonBorrar_clicked",
        "on_pushButtonSiguiente_clicked",
        "on_lineEditNombre_returnPressed",
        "on_lineEditCod_editingFinished",
        "on_radioButtonMeses_clicked",
        "on_radioButtonAnos_clicked",
        "on_radioButtonFechas_clicked",
        "on_dateEditHasta_userDateChanged",
        "date",
        "on_dateEditDesde_userDateChanged",
        "on_tableView_clicked",
        "QModelIndex",
        "index",
        "on_tableView2_doubleClicked",
        "on_radioButtonCantidad_clicked",
        "on_radioButtonFecha_clicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'on_pushButtonAnterior_clicked'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonNuevo_clicked'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonModificar_clicked'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonBorrar_clicked'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonSiguiente_clicked'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_lineEditNombre_returnPressed'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_lineEditCod_editingFinished'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_radioButtonMeses_clicked'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_radioButtonAnos_clicked'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_radioButtonFechas_clicked'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_dateEditHasta_userDateChanged'
        QtMocHelpers::SlotData<void(const QDate &)>(12, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QDate, 13 },
        }}),
        // Slot 'on_dateEditDesde_userDateChanged'
        QtMocHelpers::SlotData<void(const QDate &)>(14, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QDate, 13 },
        }}),
        // Slot 'on_tableView_clicked'
        QtMocHelpers::SlotData<void(const QModelIndex &)>(15, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 16, 17 },
        }}),
        // Slot 'on_tableView2_doubleClicked'
        QtMocHelpers::SlotData<void(const QModelIndex &)>(18, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 16, 17 },
        }}),
        // Slot 'on_radioButtonCantidad_clicked'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_radioButtonFecha_clicked'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Clientes, qt_meta_tag_ZN8ClientesE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Clientes::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8ClientesE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8ClientesE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN8ClientesE_t>.metaTypes,
    nullptr
} };

void Clientes::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Clientes *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->on_pushButtonAnterior_clicked(); break;
        case 1: _t->on_pushButtonNuevo_clicked(); break;
        case 2: _t->on_pushButtonModificar_clicked(); break;
        case 3: _t->on_pushButtonBorrar_clicked(); break;
        case 4: _t->on_pushButtonSiguiente_clicked(); break;
        case 5: _t->on_lineEditNombre_returnPressed(); break;
        case 6: _t->on_lineEditCod_editingFinished(); break;
        case 7: _t->on_radioButtonMeses_clicked(); break;
        case 8: _t->on_radioButtonAnos_clicked(); break;
        case 9: _t->on_radioButtonFechas_clicked(); break;
        case 10: _t->on_dateEditHasta_userDateChanged((*reinterpret_cast<std::add_pointer_t<QDate>>(_a[1]))); break;
        case 11: _t->on_dateEditDesde_userDateChanged((*reinterpret_cast<std::add_pointer_t<QDate>>(_a[1]))); break;
        case 12: _t->on_tableView_clicked((*reinterpret_cast<std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 13: _t->on_tableView2_doubleClicked((*reinterpret_cast<std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 14: _t->on_radioButtonCantidad_clicked(); break;
        case 15: _t->on_radioButtonFecha_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject *Clientes::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Clientes::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8ClientesE_t>.strings))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int Clientes::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 16;
    }
    return _id;
}
QT_WARNING_POP
