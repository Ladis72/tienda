/****************************************************************************
** Meta object code from reading C++ file 'gestorencargosdialog.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../gestorencargosdialog.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gestorencargosdialog.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN20GestorEncargosDialogE_t {};
} // unnamed namespace

template <> constexpr inline auto GestorEncargosDialog::qt_create_metaobjectdata<qt_meta_tag_ZN20GestorEncargosDialogE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "GestorEncargosDialog",
        "on_comboBoxEstado_currentTextChanged",
        "",
        "arg1",
        "on_lineEditFiltroCliente_textChanged",
        "on_btnMarcarRecibido_clicked",
        "on_btnMarcarEntregado_clicked",
        "on_btnBorrar_clicked",
        "on_btnCerrar_clicked",
        "on_btnNuevoEncargo_clicked",
        "on_btnCobrarTPV_clicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'on_comboBoxEstado_currentTextChanged'
        QtMocHelpers::SlotData<void(const QString &)>(1, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Slot 'on_lineEditFiltroCliente_textChanged'
        QtMocHelpers::SlotData<void(const QString &)>(4, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Slot 'on_btnMarcarRecibido_clicked'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btnMarcarEntregado_clicked'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btnBorrar_clicked'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btnCerrar_clicked'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btnNuevoEncargo_clicked'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btnCobrarTPV_clicked'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<GestorEncargosDialog, qt_meta_tag_ZN20GestorEncargosDialogE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject GestorEncargosDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20GestorEncargosDialogE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20GestorEncargosDialogE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN20GestorEncargosDialogE_t>.metaTypes,
    nullptr
} };

void GestorEncargosDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<GestorEncargosDialog *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->on_comboBoxEstado_currentTextChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->on_lineEditFiltroCliente_textChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->on_btnMarcarRecibido_clicked(); break;
        case 3: _t->on_btnMarcarEntregado_clicked(); break;
        case 4: _t->on_btnBorrar_clicked(); break;
        case 5: _t->on_btnCerrar_clicked(); break;
        case 6: _t->on_btnNuevoEncargo_clicked(); break;
        case 7: _t->on_btnCobrarTPV_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject *GestorEncargosDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GestorEncargosDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20GestorEncargosDialogE_t>.strings))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int GestorEncargosDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
