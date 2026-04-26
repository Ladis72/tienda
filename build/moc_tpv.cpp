/****************************************************************************
** Meta object code from reading C++ file 'tpv.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../tpv.h"
#include <QtGui/qtextcursor.h>
#include <QtGui/qscreen.h>
#include <QtCharts/qlineseries.h>
#include <QtCharts/qabstractbarseries.h>
#include <QtCharts/qvbarmodelmapper.h>
#include <QtCharts/qboxplotseries.h>
#include <QtCharts/qcandlestickseries.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCharts/qpieseries.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCharts/qboxplotseries.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCharts/qpieseries.h>
#include <QtCharts/qpieseries.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCharts/qxyseries.h>
#include <QtCharts/qxyseries.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCharts/qboxplotseries.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCharts/qpieseries.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCharts/qxyseries.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tpv.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN3TpvE_t {};
} // unnamed namespace

template <> constexpr inline auto Tpv::qt_create_metaobjectdata<qt_meta_tag_ZN3TpvE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Tpv",
        "cerrar_tpv",
        "",
        "on_btn_cerrar_clicked",
        "on_lineEdit_cod_returnPressed",
        "on_btn_anadir_clicked",
        "on_btn_borrar_clicked",
        "on_lineEdit_desc_returnPressed",
        "on_btn_cobrar_clicked",
        "on_tableViewTicketsPendientes_clicked",
        "QModelIndex",
        "index",
        "on_pushButtonBorrarTodo_clicked",
        "on_btn_modificar_clicked",
        "on_lineEdit_cod_cliente_editingFinished",
        "on_lineEdit_nobre_cliente_returnPressed",
        "on_lineEdit_precio_returnPressed",
        "on_tableView_doubleClicked",
        "on_tableView_clicked",
        "on_btn_preTicket_clicked",
        "on_btn_encargo_clicked",
        "on_btnGestorEncargos_clicked",
        "usarVale",
        "ticket",
        "idVale",
        "cantVale",
        "mostrarFoto"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'cerrar_tpv'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'on_btn_cerrar_clicked'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_lineEdit_cod_returnPressed'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btn_anadir_clicked'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btn_borrar_clicked'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_lineEdit_desc_returnPressed'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btn_cobrar_clicked'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_tableViewTicketsPendientes_clicked'
        QtMocHelpers::SlotData<void(const QModelIndex &)>(9, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 10, 11 },
        }}),
        // Slot 'on_pushButtonBorrarTodo_clicked'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btn_modificar_clicked'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_lineEdit_cod_cliente_editingFinished'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_lineEdit_nobre_cliente_returnPressed'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_lineEdit_precio_returnPressed'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_tableView_doubleClicked'
        QtMocHelpers::SlotData<void(const QModelIndex &)>(17, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 10, 11 },
        }}),
        // Slot 'on_tableView_clicked'
        QtMocHelpers::SlotData<void(const QModelIndex &)>(18, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 10, 11 },
        }}),
        // Slot 'on_btn_preTicket_clicked'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btn_encargo_clicked'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btnGestorEncargos_clicked'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'usarVale'
        QtMocHelpers::SlotData<void(int, int, double)>(22, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 23 }, { QMetaType::Int, 24 }, { QMetaType::Double, 25 },
        }}),
        // Slot 'mostrarFoto'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Tpv, qt_meta_tag_ZN3TpvE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Tpv::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN3TpvE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN3TpvE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN3TpvE_t>.metaTypes,
    nullptr
} };

void Tpv::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Tpv *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->cerrar_tpv(); break;
        case 1: _t->on_btn_cerrar_clicked(); break;
        case 2: _t->on_lineEdit_cod_returnPressed(); break;
        case 3: _t->on_btn_anadir_clicked(); break;
        case 4: _t->on_btn_borrar_clicked(); break;
        case 5: _t->on_lineEdit_desc_returnPressed(); break;
        case 6: _t->on_btn_cobrar_clicked(); break;
        case 7: _t->on_tableViewTicketsPendientes_clicked((*reinterpret_cast<std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 8: _t->on_pushButtonBorrarTodo_clicked(); break;
        case 9: _t->on_btn_modificar_clicked(); break;
        case 10: _t->on_lineEdit_cod_cliente_editingFinished(); break;
        case 11: _t->on_lineEdit_nobre_cliente_returnPressed(); break;
        case 12: _t->on_lineEdit_precio_returnPressed(); break;
        case 13: _t->on_tableView_doubleClicked((*reinterpret_cast<std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 14: _t->on_tableView_clicked((*reinterpret_cast<std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 15: _t->on_btn_preTicket_clicked(); break;
        case 16: _t->on_btn_encargo_clicked(); break;
        case 17: _t->on_btnGestorEncargos_clicked(); break;
        case 18: _t->usarVale((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3]))); break;
        case 19: _t->mostrarFoto(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Tpv::*)()>(_a, &Tpv::cerrar_tpv, 0))
            return;
    }
}

const QMetaObject *Tpv::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Tpv::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN3TpvE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Tpv::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 20;
    }
    return _id;
}

// SIGNAL 0
void Tpv::cerrar_tpv()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
