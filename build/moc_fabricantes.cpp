/****************************************************************************
** Meta object code from reading C++ file 'fabricantes.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../fabricantes.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fabricantes.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN11FabricantesE_t {};
} // unnamed namespace

template <> constexpr inline auto Fabricantes::qt_create_metaobjectdata<qt_meta_tag_ZN11FabricantesE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Fabricantes",
        "on_pushButtonSeleccionar_clicked",
        "",
        "on_tableView_doubleClicked",
        "QModelIndex",
        "index",
        "on_pushButtonAnadir_clicked",
        "on_pushButtonModificar_clicked",
        "on_pushButtonDeshacer_clicked",
        "on_pushButtonBorrar_clicked",
        "on_tableView_clicked",
        "on_lineEdit_textChanged",
        "arg1",
        "on_pushButtonUnificar_clicked",
        "on_pushButtonSaneador_clicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'on_pushButtonSeleccionar_clicked'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_tableView_doubleClicked'
        QtMocHelpers::SlotData<void(const QModelIndex &)>(3, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 4, 5 },
        }}),
        // Slot 'on_pushButtonAnadir_clicked'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonModificar_clicked'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonDeshacer_clicked'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonBorrar_clicked'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_tableView_clicked'
        QtMocHelpers::SlotData<void(const QModelIndex &)>(10, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 4, 5 },
        }}),
        // Slot 'on_lineEdit_textChanged'
        QtMocHelpers::SlotData<void(const QString &)>(11, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 12 },
        }}),
        // Slot 'on_pushButtonUnificar_clicked'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonSaneador_clicked'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Fabricantes, qt_meta_tag_ZN11FabricantesE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Fabricantes::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11FabricantesE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11FabricantesE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11FabricantesE_t>.metaTypes,
    nullptr
} };

void Fabricantes::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Fabricantes *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->on_pushButtonSeleccionar_clicked(); break;
        case 1: _t->on_tableView_doubleClicked((*reinterpret_cast<std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 2: _t->on_pushButtonAnadir_clicked(); break;
        case 3: _t->on_pushButtonModificar_clicked(); break;
        case 4: _t->on_pushButtonDeshacer_clicked(); break;
        case 5: _t->on_pushButtonBorrar_clicked(); break;
        case 6: _t->on_tableView_clicked((*reinterpret_cast<std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 7: _t->on_lineEdit_textChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 8: _t->on_pushButtonUnificar_clicked(); break;
        case 9: _t->on_pushButtonSaneador_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject *Fabricantes::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Fabricantes::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11FabricantesE_t>.strings))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int Fabricantes::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
