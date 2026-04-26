/****************************************************************************
** Meta object code from reading C++ file 'buscarcliente.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../buscarcliente.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'buscarcliente.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13ProxyClientesE_t {};
} // unnamed namespace

template <> constexpr inline auto ProxyClientes::qt_create_metaobjectdata<qt_meta_tag_ZN13ProxyClientesE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ProxyClientes"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ProxyClientes, qt_meta_tag_ZN13ProxyClientesE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ProxyClientes::staticMetaObject = { {
    QMetaObject::SuperData::link<QSortFilterProxyModel::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13ProxyClientesE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13ProxyClientesE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13ProxyClientesE_t>.metaTypes,
    nullptr
} };

void ProxyClientes::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ProxyClientes *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *ProxyClientes::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProxyClientes::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13ProxyClientesE_t>.strings))
        return static_cast<void*>(this);
    return QSortFilterProxyModel::qt_metacast(_clname);
}

int ProxyClientes::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSortFilterProxyModel::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {
struct qt_meta_tag_ZN13BuscarClienteE_t {};
} // unnamed namespace

template <> constexpr inline auto BuscarCliente::qt_create_metaobjectdata<qt_meta_tag_ZN13BuscarClienteE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "BuscarCliente",
        "on_tableView_activated",
        "",
        "QModelIndex",
        "index",
        "on_lineEdit_buscar_textChanged",
        "arg1",
        "selectionChanged",
        "current",
        "previous",
        "on_pushButtonCancelar_clicked",
        "on_pushButtonAceptar_clicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'on_tableView_activated'
        QtMocHelpers::SlotData<void(const QModelIndex &)>(1, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Slot 'on_lineEdit_buscar_textChanged'
        QtMocHelpers::SlotData<void(const QString &)>(5, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 6 },
        }}),
        // Slot 'selectionChanged'
        QtMocHelpers::SlotData<void(const QModelIndex &, const QModelIndex &)>(7, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 3, 8 }, { 0x80000000 | 3, 9 },
        }}),
        // Slot 'on_pushButtonCancelar_clicked'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonAceptar_clicked'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<BuscarCliente, qt_meta_tag_ZN13BuscarClienteE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject BuscarCliente::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13BuscarClienteE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13BuscarClienteE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13BuscarClienteE_t>.metaTypes,
    nullptr
} };

void BuscarCliente::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<BuscarCliente *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->on_tableView_activated((*reinterpret_cast<std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 1: _t->on_lineEdit_buscar_textChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->selectionChanged((*reinterpret_cast<std::add_pointer_t<QModelIndex>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QModelIndex>>(_a[2]))); break;
        case 3: _t->on_pushButtonCancelar_clicked(); break;
        case 4: _t->on_pushButtonAceptar_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject *BuscarCliente::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BuscarCliente::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13BuscarClienteE_t>.strings))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int BuscarCliente::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
