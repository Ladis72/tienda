/****************************************************************************
** Meta object code from reading C++ file 'cambiodatosproducto.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../cambiodatosproducto.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cambiodatosproducto.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN19CambioDatosProductoE_t {};
} // unnamed namespace

template <> constexpr inline auto CambioDatosProducto::qt_create_metaobjectdata<qt_meta_tag_ZN19CambioDatosProductoE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "CambioDatosProducto",
        "on_radioButtonNombreAlmacen_clicked",
        "",
        "on_radioButtonNombrePedido_clicked",
        "on_radioButtonNombrePersonal_clicked",
        "on_radioButtonPVPPedido_clicked",
        "on_radioButtonPVPAlmacen_clicked",
        "on_radioButtonPVPPersonal_clicked",
        "on_lineEditNombrePersonal_editingFinished",
        "on_lineEditPVPPersonal_editingFinished",
        "on_radioButtonDatosPedido_clicked",
        "on_radioButtonDatosAlmacen_clicked",
        "on_radioButton_clicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'on_radioButtonNombreAlmacen_clicked'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_radioButtonNombrePedido_clicked'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_radioButtonNombrePersonal_clicked'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_radioButtonPVPPedido_clicked'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_radioButtonPVPAlmacen_clicked'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_radioButtonPVPPersonal_clicked'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_lineEditNombrePersonal_editingFinished'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_lineEditPVPPersonal_editingFinished'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_radioButtonDatosPedido_clicked'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_radioButtonDatosAlmacen_clicked'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_radioButton_clicked'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<CambioDatosProducto, qt_meta_tag_ZN19CambioDatosProductoE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject CambioDatosProducto::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19CambioDatosProductoE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19CambioDatosProductoE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN19CambioDatosProductoE_t>.metaTypes,
    nullptr
} };

void CambioDatosProducto::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<CambioDatosProducto *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->on_radioButtonNombreAlmacen_clicked(); break;
        case 1: _t->on_radioButtonNombrePedido_clicked(); break;
        case 2: _t->on_radioButtonNombrePersonal_clicked(); break;
        case 3: _t->on_radioButtonPVPPedido_clicked(); break;
        case 4: _t->on_radioButtonPVPAlmacen_clicked(); break;
        case 5: _t->on_radioButtonPVPPersonal_clicked(); break;
        case 6: _t->on_lineEditNombrePersonal_editingFinished(); break;
        case 7: _t->on_lineEditPVPPersonal_editingFinished(); break;
        case 8: _t->on_radioButtonDatosPedido_clicked(); break;
        case 9: _t->on_radioButtonDatosAlmacen_clicked(); break;
        case 10: _t->on_radioButton_clicked(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject *CambioDatosProducto::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CambioDatosProducto::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19CambioDatosProductoE_t>.strings))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int CambioDatosProducto::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
