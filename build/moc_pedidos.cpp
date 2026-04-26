/****************************************************************************
** Meta object code from reading C++ file 'pedidos.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../pedidos.h"
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
#error "The header file 'pedidos.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN7pedidosE_t {};
} // unnamed namespace

template <> constexpr inline auto pedidos::qt_create_metaobjectdata<qt_meta_tag_ZN7pedidosE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "pedidos",
        "on_leCod_editingFinished",
        "",
        "on_leUds_textChanged",
        "arg1",
        "on_lePvt_textChanged",
        "on_leIva_textChanged",
        "on_leDescuento_textChanged",
        "on_pushButtonAnadir_clicked",
        "on_leCod_returnPressed",
        "on_pushButtonBorrar_clicked",
        "on_tableView_doubleClicked",
        "QModelIndex",
        "index",
        "on_tableView_clicked",
        "on_leDescripcion_returnPressed",
        "on_pushButtonModificar_clicked",
        "on_leIva_editingFinished",
        "on_dateEdit_editingFinished",
        "on_leBon_textChanged",
        "on_lePvp_textChanged",
        "on_pushButtonImprimir_clicked",
        "on_pushButtonCerrar_clicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'on_leCod_editingFinished'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_leUds_textChanged'
        QtMocHelpers::SlotData<void(const QString &)>(3, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 4 },
        }}),
        // Slot 'on_lePvt_textChanged'
        QtMocHelpers::SlotData<void(const QString &)>(5, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 4 },
        }}),
        // Slot 'on_leIva_textChanged'
        QtMocHelpers::SlotData<void(const QString &)>(6, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 4 },
        }}),
        // Slot 'on_leDescuento_textChanged'
        QtMocHelpers::SlotData<void(const QString &)>(7, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 4 },
        }}),
        // Slot 'on_pushButtonAnadir_clicked'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_leCod_returnPressed'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonBorrar_clicked'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_tableView_doubleClicked'
        QtMocHelpers::SlotData<void(const QModelIndex &)>(11, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 12, 13 },
        }}),
        // Slot 'on_tableView_clicked'
        QtMocHelpers::SlotData<void(const QModelIndex &)>(14, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 12, 13 },
        }}),
        // Slot 'on_leDescripcion_returnPressed'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonModificar_clicked'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_leIva_editingFinished'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_dateEdit_editingFinished'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_leBon_textChanged'
        QtMocHelpers::SlotData<void(const QString &)>(19, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 4 },
        }}),
        // Slot 'on_lePvp_textChanged'
        QtMocHelpers::SlotData<void(const QString &)>(20, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 4 },
        }}),
        // Slot 'on_pushButtonImprimir_clicked'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonCerrar_clicked'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<pedidos, qt_meta_tag_ZN7pedidosE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject pedidos::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7pedidosE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7pedidosE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7pedidosE_t>.metaTypes,
    nullptr
} };

void pedidos::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<pedidos *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->on_leCod_editingFinished(); break;
        case 1: _t->on_leUds_textChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->on_lePvt_textChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->on_leIva_textChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->on_leDescuento_textChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->on_pushButtonAnadir_clicked(); break;
        case 6: _t->on_leCod_returnPressed(); break;
        case 7: _t->on_pushButtonBorrar_clicked(); break;
        case 8: _t->on_tableView_doubleClicked((*reinterpret_cast<std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 9: _t->on_tableView_clicked((*reinterpret_cast<std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 10: _t->on_leDescripcion_returnPressed(); break;
        case 11: _t->on_pushButtonModificar_clicked(); break;
        case 12: _t->on_leIva_editingFinished(); break;
        case 13: _t->on_dateEdit_editingFinished(); break;
        case 14: _t->on_leBon_textChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 15: _t->on_lePvp_textChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 16: _t->on_pushButtonImprimir_clicked(); break;
        case 17: _t->on_pushButtonCerrar_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject *pedidos::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *pedidos::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7pedidosE_t>.strings))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int pedidos::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 18;
    }
    return _id;
}
QT_WARNING_POP
