/****************************************************************************
** Meta object code from reading C++ file 'mainwidget.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../include/qtclient/mainwidget.hpp"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwidget.hpp' doesn't include <QObject>."
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
struct qt_meta_tag_ZN10MainWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto MainWidget::qt_create_metaobjectdata<qt_meta_tag_ZN10MainWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MainWidget",
        "handleLoginRequest",
        "",
        "id",
        "string",
        "password",
        "handleRegisterRequest",
        "name",
        "handleMessageReceived",
        "json",
        "js",
        "handleConnected",
        "handleDisconnected",
        "handleSendOneChat",
        "toId",
        "msg",
        "handleSendGroupChat",
        "groupId",
        "handleContactChatRequest",
        "userId",
        "handleContactGroupChatRequest",
        "handleAddFriend",
        "friendId",
        "handleCreateGroup",
        "groupName",
        "groupDesc",
        "handleAddGroup",
        "handleLogout"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'handleLoginRequest'
        QtMocHelpers::SlotData<void(int, const string &)>(1, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { 0x80000000 | 4, 5 },
        }}),
        // Slot 'handleRegisterRequest'
        QtMocHelpers::SlotData<void(const string &, const string &)>(6, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 4, 7 }, { 0x80000000 | 4, 5 },
        }}),
        // Slot 'handleMessageReceived'
        QtMocHelpers::SlotData<void(json)>(8, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 9, 10 },
        }}),
        // Slot 'handleConnected'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'handleDisconnected'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'handleSendOneChat'
        QtMocHelpers::SlotData<void(int, const string &)>(13, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 14 }, { 0x80000000 | 4, 15 },
        }}),
        // Slot 'handleSendGroupChat'
        QtMocHelpers::SlotData<void(int, const string &)>(16, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 17 }, { 0x80000000 | 4, 15 },
        }}),
        // Slot 'handleContactChatRequest'
        QtMocHelpers::SlotData<void(int)>(18, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 19 },
        }}),
        // Slot 'handleContactGroupChatRequest'
        QtMocHelpers::SlotData<void(int)>(20, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 17 },
        }}),
        // Slot 'handleAddFriend'
        QtMocHelpers::SlotData<void(int)>(21, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 22 },
        }}),
        // Slot 'handleCreateGroup'
        QtMocHelpers::SlotData<void(const string &, const string &)>(23, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 4, 24 }, { 0x80000000 | 4, 25 },
        }}),
        // Slot 'handleAddGroup'
        QtMocHelpers::SlotData<void(int)>(26, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 17 },
        }}),
        // Slot 'handleLogout'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MainWidget, qt_meta_tag_ZN10MainWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MainWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10MainWidgetE_t>.metaTypes,
    nullptr
} };

void MainWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->handleLoginRequest((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<string>>(_a[2]))); break;
        case 1: _t->handleRegisterRequest((*reinterpret_cast<std::add_pointer_t<string>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<string>>(_a[2]))); break;
        case 2: _t->handleMessageReceived((*reinterpret_cast<std::add_pointer_t<json>>(_a[1]))); break;
        case 3: _t->handleConnected(); break;
        case 4: _t->handleDisconnected(); break;
        case 5: _t->handleSendOneChat((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<string>>(_a[2]))); break;
        case 6: _t->handleSendGroupChat((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<string>>(_a[2]))); break;
        case 7: _t->handleContactChatRequest((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->handleContactGroupChatRequest((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 9: _t->handleAddFriend((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 10: _t->handleCreateGroup((*reinterpret_cast<std::add_pointer_t<string>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<string>>(_a[2]))); break;
        case 11: _t->handleAddGroup((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 12: _t->handleLogout(); break;
        default: ;
        }
    }
}

const QMetaObject *MainWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int MainWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 13;
    }
    return _id;
}
QT_WARNING_POP
