/****************************************************************************
** Meta object code from reading C++ file 'masterthread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../STM32BL_FlashTool/masterthread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'masterthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MasterThread_t {
    QByteArrayData data[15];
    char stringdata[113];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MasterThread_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MasterThread_t qt_meta_stringdata_MasterThread = {
    {
QT_MOC_LITERAL(0, 0, 12),
QT_MOC_LITERAL(1, 13, 8),
QT_MOC_LITERAL(2, 22, 0),
QT_MOC_LITERAL(3, 23, 3),
QT_MOC_LITERAL(4, 27, 3),
QT_MOC_LITERAL(5, 31, 3),
QT_MOC_LITERAL(6, 35, 9),
QT_MOC_LITERAL(7, 45, 13),
QT_MOC_LITERAL(8, 59, 4),
QT_MOC_LITERAL(9, 64, 5),
QT_MOC_LITERAL(10, 70, 1),
QT_MOC_LITERAL(11, 72, 7),
QT_MOC_LITERAL(12, 80, 13),
QT_MOC_LITERAL(13, 94, 7),
QT_MOC_LITERAL(14, 102, 10)
    },
    "MasterThread\0response\0\0CMD\0req\0res\0"
    "respbytes\0response_fail\0fail\0error\0s\0"
    "timeout\0enableButtons\0connect\0disconnect"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MasterThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   49,    2, 0x06 /* Public */,
       7,    1,   58,    2, 0x06 /* Public */,
       9,    1,   61,    2, 0x06 /* Public */,
      11,    1,   64,    2, 0x06 /* Public */,
      12,    0,   67,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    0,   68,    2, 0x08 /* Private */,
      14,    0,   69,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::QString, QMetaType::QByteArray,    3,    4,    5,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MasterThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MasterThread *_t = static_cast<MasterThread *>(_o);
        switch (_id) {
        case 0: _t->response((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< QByteArray(*)>(_a[4]))); break;
        case 1: _t->response_fail((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->error((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->timeout((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->enableButtons(); break;
        case 5: _t->connect(); break;
        case 6: _t->disconnect(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MasterThread::*_t)(int , const QString & , const QString & , QByteArray );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MasterThread::response)) {
                *result = 0;
            }
        }
        {
            typedef void (MasterThread::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MasterThread::response_fail)) {
                *result = 1;
            }
        }
        {
            typedef void (MasterThread::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MasterThread::error)) {
                *result = 2;
            }
        }
        {
            typedef void (MasterThread::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MasterThread::timeout)) {
                *result = 3;
            }
        }
        {
            typedef void (MasterThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MasterThread::enableButtons)) {
                *result = 4;
            }
        }
    }
}

const QMetaObject MasterThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_MasterThread.data,
      qt_meta_data_MasterThread,  qt_static_metacall, 0, 0}
};


const QMetaObject *MasterThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MasterThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MasterThread.stringdata))
        return static_cast<void*>(const_cast< MasterThread*>(this));
    return QThread::qt_metacast(_clname);
}

int MasterThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void MasterThread::response(int _t1, const QString & _t2, const QString & _t3, QByteArray _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MasterThread::response_fail(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MasterThread::error(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MasterThread::timeout(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MasterThread::enableButtons()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}
QT_END_MOC_NAMESPACE
