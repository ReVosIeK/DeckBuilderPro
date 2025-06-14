/****************************************************************************
** Meta object code from reading C++ file 'Card.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../Card.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Card.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.1. It"
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
struct qt_meta_tag_ZN4CardE_t {};
} // unnamed namespace

template <> constexpr inline auto Card::qt_create_metaobjectdata<qt_meta_tag_ZN4CardE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Card",
        "name",
        "",
        "locale",
        "effectText",
        "is",
        "CardType",
        "type",
        "Hero",
        "Villain",
        "Equipment",
        "Superpower",
        "Location",
        "Starter",
        "SuperVillain",
        "Kick",
        "Weakness"
    };

    QtMocHelpers::UintData qt_methods {
        // Method 'name'
        QtMocHelpers::MethodData<QString(const QString &) const>(1, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::QString, 3 },
        }}),
        // Method 'name'
        QtMocHelpers::MethodData<QString() const>(1, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::QString),
        // Method 'effectText'
        QtMocHelpers::MethodData<QString(const QString &) const>(4, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::QString, 3 },
        }}),
        // Method 'effectText'
        QtMocHelpers::MethodData<QString() const>(4, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::QString),
        // Method 'is'
        QtMocHelpers::MethodData<bool(CardType) const>(5, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { 0x80000000 | 6, 7 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'CardType'
        QtMocHelpers::EnumData<CardType>(6, 6, QMC::EnumFlags{}).add({
            {    8, CardType::Hero },
            {    9, CardType::Villain },
            {   10, CardType::Equipment },
            {   11, CardType::Superpower },
            {   12, CardType::Location },
            {   13, CardType::Starter },
            {   14, CardType::SuperVillain },
            {   15, CardType::Kick },
            {   16, CardType::Weakness },
        }),
    };
    return QtMocHelpers::metaObjectData<Card, qt_meta_tag_ZN4CardE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Card::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4CardE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4CardE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN4CardE_t>.metaTypes,
    nullptr
} };

void Card::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Card *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { QString _r = _t->name((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 1: { QString _r = _t->name();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 2: { QString _r = _t->effectText((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 3: { QString _r = _t->effectText();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 4: { bool _r = _t->is((*reinterpret_cast< std::add_pointer_t<CardType>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

const QMetaObject *Card::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Card::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4CardE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Card::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
