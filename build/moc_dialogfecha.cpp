/****************************************************************************
** Meta object code from reading C++ file 'dialogfecha.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../dialogfecha.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dialogfecha.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN11DialogFechaE_t {};
} // unnamed namespace

template <> constexpr inline auto DialogFecha::qt_create_metaobjectdata<qt_meta_tag_ZN11DialogFechaE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "DialogFecha",
        "on_dateEdit_userDateChanged",
        "",
        "date",
        "on_pushButton_clicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'on_dateEdit_userDateChanged'
        QtMocHelpers::SlotData<void(const QDate &)>(1, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QDate, 3 },
        }}),
        // Slot 'on_pushButton_clicked'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<DialogFecha, qt_meta_tag_ZN11DialogFechaE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject DialogFecha::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11DialogFechaE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11DialogFechaE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11DialogFechaE_t>.metaTypes,
    nullptr
} };

void DialogFecha::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<DialogFecha *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->on_dateEdit_userDateChanged((*reinterpret_cast<std::add_pointer_t<QDate>>(_a[1]))); break;
        case 1: _t->on_pushButton_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject *DialogFecha::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DialogFecha::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11DialogFechaE_t>.strings))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int DialogFecha::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
