/****************************************************************************
** Meta object code from reading C++ file 'clientes.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../clientes.h"
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
        "on_checkBoxTiendasConectadas_clicked",
        "on_tableView2_doubleClicked",
        "QModelIndex",
        "index",
        "on_tableView2_clicked",
        "on_radioButtonCantidad_clicked",
        "on_radioButtonFecha_clicked",
        "on_comboBoxAgrupacion_currentIndexChanged",
        "on_comboBoxTipoGrafico_currentIndexChanged",
        "on_dateEditDesde_dateChanged",
        "date",
        "on_lineEditBuscarProducto_textChanged",
        "arg1",
        "on_btn_encargos_cliente_clicked",
        "on_dateEditHasta_dateChanged",
        "on_dateEditDesde_2_dateChanged",
        "on_dateEditHasta_2_dateChanged",
        "on_pushButtonUnificar_clicked",
        "on_pushButtonSaneador_clicked",
        "onBarClicked",
        "QBarSet*",
        "barset",
        "onBarHovered",
        "status",
        "cargarTicketsPorRango",
        "rangoMapeado"
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
        // Slot 'on_checkBoxTiendasConectadas_clicked'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_tableView2_doubleClicked'
        QtMocHelpers::SlotData<void(const QModelIndex &)>(10, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 11, 12 },
        }}),
        // Slot 'on_tableView2_clicked'
        QtMocHelpers::SlotData<void(const QModelIndex &)>(13, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 11, 12 },
        }}),
        // Slot 'on_radioButtonCantidad_clicked'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_radioButtonFecha_clicked'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_comboBoxAgrupacion_currentIndexChanged'
        QtMocHelpers::SlotData<void(int)>(16, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 12 },
        }}),
        // Slot 'on_comboBoxTipoGrafico_currentIndexChanged'
        QtMocHelpers::SlotData<void(int)>(17, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 12 },
        }}),
        // Slot 'on_dateEditDesde_dateChanged'
        QtMocHelpers::SlotData<void(const QDate &)>(18, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QDate, 19 },
        }}),
        // Slot 'on_lineEditBuscarProducto_textChanged'
        QtMocHelpers::SlotData<void(const QString &)>(20, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 21 },
        }}),
        // Slot 'on_btn_encargos_cliente_clicked'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_dateEditHasta_dateChanged'
        QtMocHelpers::SlotData<void(const QDate &)>(23, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QDate, 19 },
        }}),
        // Slot 'on_dateEditDesde_2_dateChanged'
        QtMocHelpers::SlotData<void(const QDate &)>(24, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QDate, 19 },
        }}),
        // Slot 'on_dateEditHasta_2_dateChanged'
        QtMocHelpers::SlotData<void(const QDate &)>(25, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QDate, 19 },
        }}),
        // Slot 'on_pushButtonUnificar_clicked'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonSaneador_clicked'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBarClicked'
        QtMocHelpers::SlotData<void(int, QBarSet *)>(28, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 12 }, { 0x80000000 | 29, 30 },
        }}),
        // Slot 'onBarHovered'
        QtMocHelpers::SlotData<void(bool, int, QBarSet *)>(31, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 32 }, { QMetaType::Int, 12 }, { 0x80000000 | 29, 30 },
        }}),
        // Slot 'cargarTicketsPorRango'
        QtMocHelpers::SlotData<void(const QString &)>(33, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 34 },
        }}),
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
        case 7: _t->on_checkBoxTiendasConectadas_clicked(); break;
        case 8: _t->on_tableView2_doubleClicked((*reinterpret_cast<std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 9: _t->on_tableView2_clicked((*reinterpret_cast<std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 10: _t->on_radioButtonCantidad_clicked(); break;
        case 11: _t->on_radioButtonFecha_clicked(); break;
        case 12: _t->on_comboBoxAgrupacion_currentIndexChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 13: _t->on_comboBoxTipoGrafico_currentIndexChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 14: _t->on_dateEditDesde_dateChanged((*reinterpret_cast<std::add_pointer_t<QDate>>(_a[1]))); break;
        case 15: _t->on_lineEditBuscarProducto_textChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 16: _t->on_btn_encargos_cliente_clicked(); break;
        case 17: _t->on_dateEditHasta_dateChanged((*reinterpret_cast<std::add_pointer_t<QDate>>(_a[1]))); break;
        case 18: _t->on_dateEditDesde_2_dateChanged((*reinterpret_cast<std::add_pointer_t<QDate>>(_a[1]))); break;
        case 19: _t->on_dateEditHasta_2_dateChanged((*reinterpret_cast<std::add_pointer_t<QDate>>(_a[1]))); break;
        case 20: _t->on_pushButtonUnificar_clicked(); break;
        case 21: _t->on_pushButtonSaneador_clicked(); break;
        case 22: _t->onBarClicked((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QBarSet*>>(_a[2]))); break;
        case 23: _t->onBarHovered((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QBarSet*>>(_a[3]))); break;
        case 24: _t->cargarTicketsPorRango((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 22:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QBarSet* >(); break;
            }
            break;
        case 23:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 2:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QBarSet* >(); break;
            }
            break;
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
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    }
    return _id;
}
QT_WARNING_POP
