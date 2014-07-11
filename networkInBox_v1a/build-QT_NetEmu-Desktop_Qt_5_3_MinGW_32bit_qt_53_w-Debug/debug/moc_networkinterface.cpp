/****************************************************************************
** Meta object code from reading C++ file 'networkinterface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../networkInBoxMul_0710/networkInBox_v1a/QT_NetEmu4/networkinterface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'networkinterface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_NetworkEmulator_t {
    QByteArrayData data[8];
    char stringdata[123];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NetworkEmulator_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NetworkEmulator_t qt_meta_stringdata_NetworkEmulator = {
    {
QT_MOC_LITERAL(0, 0, 15),
QT_MOC_LITERAL(1, 16, 19),
QT_MOC_LITERAL(2, 36, 0),
QT_MOC_LITERAL(3, 37, 20),
QT_MOC_LITERAL(4, 58, 19),
QT_MOC_LITERAL(5, 78, 20),
QT_MOC_LITERAL(6, 99, 5),
QT_MOC_LITERAL(7, 105, 17)
    },
    "NetworkEmulator\0addAdapterInterface\0"
    "\0interfaceDescription\0statisticsCollected\0"
    "CollectedStatistics*\0stats\0receiveStatistics"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetworkEmulator[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       4,    1,   32,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   35,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, 0x80000000 | 5,    6,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5,    6,

       0        // eod
};

void NetworkEmulator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        NetworkEmulator *_t = static_cast<NetworkEmulator *>(_o);
        switch (_id) {
        case 0: _t->addAdapterInterface((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->statisticsCollected((*reinterpret_cast< CollectedStatistics*(*)>(_a[1]))); break;
        case 2: _t->receiveStatistics((*reinterpret_cast< CollectedStatistics*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (NetworkEmulator::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NetworkEmulator::addAdapterInterface)) {
                *result = 0;
            }
        }
        {
            typedef void (NetworkEmulator::*_t)(CollectedStatistics * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NetworkEmulator::statisticsCollected)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject NetworkEmulator::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_NetworkEmulator.data,
      qt_meta_data_NetworkEmulator,  qt_static_metacall, 0, 0}
};


const QMetaObject *NetworkEmulator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetworkEmulator::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NetworkEmulator.stringdata))
        return static_cast<void*>(const_cast< NetworkEmulator*>(this));
    return QObject::qt_metacast(_clname);
}

int NetworkEmulator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void NetworkEmulator::addAdapterInterface(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void NetworkEmulator::statisticsCollected(CollectedStatistics * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
