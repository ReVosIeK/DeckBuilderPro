/****************************************************************************
** Meta object code from reading C++ file 'Player.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../Player.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Player.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN6PlayerE_t {};
} // unnamed namespace

template <> constexpr inline auto Player::qt_create_metaobjectdata<qt_meta_tag_ZN6PlayerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Player",
        "currentPowerChanged",
        "",
        "handChanged",
        "playedCardsChanged",
        "discardPileChanged",
        "deckChanged",
        "playCard",
        "index",
        "name",
        "currentPower",
        "hand",
        "QQmlListProperty<Card>",
        "playedCards",
        "discardPileSize",
        "deckSize"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'currentPowerChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'handChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'playedCardsChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'discardPileChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'deckChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'playCard'
        QtMocHelpers::MethodData<void(int)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 8 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'name'
        QtMocHelpers::PropertyData<QString>(9, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'currentPower'
        QtMocHelpers::PropertyData<int>(10, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'hand'
        QtMocHelpers::PropertyData<QQmlListProperty<Card>>(11, 0x80000000 | 12, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 1),
        // property 'playedCards'
        QtMocHelpers::PropertyData<QQmlListProperty<Card>>(13, 0x80000000 | 12, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 2),
        // property 'discardPileSize'
        QtMocHelpers::PropertyData<int>(14, QMetaType::Int, QMC::DefaultPropertyFlags, 3),
        // property 'deckSize'
        QtMocHelpers::PropertyData<int>(15, QMetaType::Int, QMC::DefaultPropertyFlags, 4),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Player, qt_meta_tag_ZN6PlayerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Player::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6PlayerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6PlayerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN6PlayerE_t>.metaTypes,
    nullptr
} };

void Player::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Player *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->currentPowerChanged(); break;
        case 1: _t->handChanged(); break;
        case 2: _t->playedCardsChanged(); break;
        case 3: _t->discardPileChanged(); break;
        case 4: _t->deckChanged(); break;
        case 5: _t->playCard((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Player::*)()>(_a, &Player::currentPowerChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Player::*)()>(_a, &Player::handChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Player::*)()>(_a, &Player::playedCardsChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (Player::*)()>(_a, &Player::discardPileChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (Player::*)()>(_a, &Player::deckChanged, 4))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->name(); break;
        case 1: *reinterpret_cast<int*>(_v) = _t->currentPower(); break;
        case 2: *reinterpret_cast<QQmlListProperty<Card>*>(_v) = _t->hand(); break;
        case 3: *reinterpret_cast<QQmlListProperty<Card>*>(_v) = _t->playedCards(); break;
        case 4: *reinterpret_cast<int*>(_v) = _t->discardPileSize(); break;
        case 5: *reinterpret_cast<int*>(_v) = _t->deckSize(); break;
        default: break;
        }
    }
}

const QMetaObject *Player::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Player::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6PlayerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Player::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void Player::currentPowerChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Player::handChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Player::playedCardsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Player::discardPileChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void Player::deckChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
