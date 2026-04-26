/****************************************************************************
** Meta object code from reading C++ file 'etiquetas.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../etiquetas.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'etiquetas.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN9EtiquetasE_t {};
} // unnamed namespace

template <> constexpr inline auto Etiquetas::qt_create_metaobjectdata<qt_meta_tag_ZN9EtiquetasE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Etiquetas",
        "on_pushButtonImprimir_clicked",
        "",
        "on_lineEditCod_returnPressed",
        "on_lineEditDesc_returnPressed",
        "on_tableView_clicked",
        "QModelIndex",
        "index",
        "borrarLinea",
        "on_pushButton_3_clicked",
        "generarPDF",
        "QList<QList<QString>>",
        "datos",
        "abriPDF",
        "ruta"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'on_pushButtonImprimir_clicked'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_lineEditCod_returnPressed'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_lineEditDesc_returnPressed'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_tableView_clicked'
        QtMocHelpers::SlotData<void(const QModelIndex &)>(5, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 6, 7 },
        }}),
        // Slot 'borrarLinea'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButton_3_clicked'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'generarPDF'
        QtMocHelpers::SlotData<void(const QList<QList<QString> >)>(10, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 11, 12 },
        }}),
        // Slot 'abriPDF'
        QtMocHelpers::SlotData<void(const QString &)>(13, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 14 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Etiquetas, qt_meta_tag_ZN9EtiquetasE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Etiquetas::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9EtiquetasE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9EtiquetasE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN9EtiquetasE_t>.metaTypes,
    nullptr
} };

void Etiquetas::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Etiquetas *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->on_pushButtonImprimir_clicked(); break;
        case 1: _t->on_lineEditCod_returnPressed(); break;
        case 2: _t->on_lineEditDesc_returnPressed(); break;
        case 3: _t->on_tableView_clicked((*reinterpret_cast<std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 4: _t->borrarLinea(); break;
        case 5: _t->on_pushButton_3_clicked(); break;
        case 6: _t->generarPDF((*reinterpret_cast<std::add_pointer_t<QList<QList<QString>>>>(_a[1]))); break;
        case 7: _t->abriPDF((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<QList<QString>> >(); break;
            }
            break;
        }
    }
}

const QMetaObject *Etiquetas::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Etiquetas::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9EtiquetasE_t>.strings))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int Etiquetas::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
