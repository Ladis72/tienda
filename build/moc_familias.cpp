/****************************************************************************
** Meta object code from reading C++ file 'familias.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../familias.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'familias.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN8FamiliasE_t {};
} // unnamed namespace

template <> constexpr inline auto Familias::qt_create_metaobjectdata<qt_meta_tag_ZN8FamiliasE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Familias",
        "on_pushButtonSeleccionar_clicked",
        "",
        "on_tableView_doubleClicked",
        "QModelIndex",
        "index",
        "on_pushButtonAnadir_clicked",
        "on_pushButtonGuardar_clicked",
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
        // Slot 'on_pushButtonGuardar_clicked'
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
    return QtMocHelpers::metaObjectData<Familias, qt_meta_tag_ZN8FamiliasE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Familias::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8FamiliasE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8FamiliasE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN8FamiliasE_t>.metaTypes,
    nullptr
} };

void Familias::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Familias *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->on_pushButtonSeleccionar_clicked(); break;
        case 1: _t->on_tableView_doubleClicked((*reinterpret_cast<std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 2: _t->on_pushButtonAnadir_clicked(); break;
        case 3: _t->on_pushButtonGuardar_clicked(); break;
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

const QMetaObject *Familias::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Familias::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8FamiliasE_t>.strings))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int Familias::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
