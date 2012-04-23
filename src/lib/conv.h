/****************************************************************************
** $Id: conv.h,v 1.8 2006/07/04 05:42:27 red75 Exp $
**
** Trade equipment common portable library project
**
** Copyright (C) 1999-2006 Leader InfoTech.  All rights reserved.
**
** This file is part of the Library of the Ananas
** automation accounting system.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.leaderit.ru/ or email sales@leaderit.ru
**
** Contact org@leaderit.ru if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#ifndef _CONV_H_217412934_
#define _CONV_H_217412934_

#include "teglobal.h"
#include <qstring.h>
#include <qvaluevector.h>
#include <qdatetime.h>
#include <qvariant.h>
#include <qcstring.h>

template <class T>
struct LocVarType
{
    typedef T Type;
};

template <>
struct LocVarType<const QString &>
{
    typedef QString Type;
};

template <>
struct LocVarType<QString &>
{
    typedef QString Type;
};

template <>
struct LocVarType<const int &>
{
    typedef int Type;
};

template <>
struct LocVarType<int &>
{
    typedef int Type;
};

template <>
struct LocVarType<const QDateTime &>
{
    typedef QDateTime Type;
};

template <>
struct LocVarType<QDateTime &>
{
    typedef QDateTime Type;
};

template <>
struct LocVarType<const float &>
{
    typedef float Type;
};

template <>
struct LocVarType<float &>
{
    typedef float Type;
};

template <>
struct LocVarType<double &>
{
    typedef double Type;
};

template <>
struct LocVarType<const double &>
{
    typedef double Type;
};

template <>
struct LocVarType<const char *>
{
    typedef QCString Type;
};

template <class T>
QString toString(T v)
{
    return QString("%1").arg(v);
};

template <>
LIB_EXPORT QString toString<double>(double v);

template <>
LIB_EXPORT QString toString<QVariant>(QVariant v);

template <>
LIB_EXPORT QString toString<float>(float v);

template <class T>
T fromQVariant(const QVariant & s)
{
    return (T)s;
};

template <>
LIB_EXPORT bool fromQVariant<bool>(const QVariant &);

template <>
LIB_EXPORT int fromQVariant<int>(const QVariant &);

template <>
LIB_EXPORT uint fromQVariant<uint>(const QVariant &);

template <>
LIB_EXPORT float fromQVariant<float>(const QVariant &);

template <>
LIB_EXPORT double fromQVariant<double>(const QVariant &);

template <>
LIB_EXPORT Q_LLONG fromQVariant<Q_LLONG>(const QVariant &);

template <>
LIB_EXPORT Q_ULLONG fromQVariant<Q_ULLONG>(const QVariant &);

template <>
LIB_EXPORT Q_ULONG fromQVariant<Q_ULONG>(const QVariant &);

template <>
LIB_EXPORT Q_LONG fromQVariant<Q_LONG>(const QVariant &);

template <>
LIB_EXPORT QString fromQVariant<QString>(const QVariant &);

template <>
LIB_EXPORT QCString fromQVariant<QCString>(const QVariant &);

struct LIB_EXPORT memFunc
{
    virtual int argc()=0;
    // This function returns zero on successful execution, and nonzero otherwise.
    virtual int operator()(const QValueVector<QVariant> & , QString & , QString & )=0;
    virtual ~memFunc(){};
};

template <class T, class R>
struct NonConstFunc0
{
    typedef R (T::*memFunPtr)();
};

template <class T>
struct NonConstProc0
{
    typedef void (T::*Type)();
};

template <class T>
struct ConstProc0
{
    typedef void (T::*Type)() const;
};

template <class T, class R, class P1>
struct NonConstFunc1
{
    typedef R (T::*memFunPtr)(P1);
};

template <class T, class R, class P1, class P2>
struct NonConstFunc2
{
    typedef R (T::*memFunPtr)(P1,P2);
};

template <class T, class R, class P1, class P2, class P3>
struct NonConstFunc3
{
    typedef R (T::*memFunPtr)(P1,P2,P3);
};

template <class T, class R, class P1, class P2, class P3, class P4>
struct NonConstFunc4
{
    typedef R (T::*memFunPtr)(P1,P2,P3,P4);
};

template <class T, class R, class P1, class P2, class P3, class P4, class P5>
struct NonConstFunc5
{
    typedef R (T::*memFunPtr)(P1,P2,P3,P4,P5);
};

template <class T, class R>
struct ConstFunc0
{
    typedef R (T::*memFunPtr)() const;
};

template <class T, class R, class P1>
struct ConstFunc1
{
    typedef R (T::*memFunPtr)(P1) const;
};

template <class T, class R, class P1, class P2>
struct ConstFunc2
{
    typedef R (T::*memFunPtr)(P1,P2) const;
};

template <class T, class R, class P1, class P2, class P3>
struct ConstFunc3
{
    typedef R (T::*memFunPtr)(P1,P2,P3) const;
};

template <class T, class R, class P1, class P2, class P3, class P4>
struct ConstFunc4
{
    typedef R (T::*memFunPtr)(P1,P2,P3,P4) const;
};

template <class T, class R, class P1, class P2, class P3, class P4, class P5>
struct ConstFunc5
{
    typedef R (T::*memFunPtr)(P1,P2,P3,P4,P5) const;
};


// We are using compiler generated copyctor, it's ok for now.

template <class T, class R, class Func=NonConstFunc0<T,R> >
struct memFunc0 : memFunc
{
    typedef typename Func::memFunPtr memFunPtr;
    memFunPtr m_pfunc;
    T * m_pobj;
    enum {arity=0};
    memFunc0(T * pobj, memFunPtr pfunc):m_pfunc(pfunc),m_pobj(pobj){};
    virtual int operator()(const QValueVector<QVariant> & parms, QString & res, QString & err)
    {
        if (parms.size()!=arity)
        {
            err=QString("Number of provided parameters doesn't match function definition");
            return 1;
        };
        res=toString<R>((m_pobj->*m_pfunc)());
        return 0;
    };
    virtual int argc()
    {
        return arity;
    };
};

template <class T, class R, class P1, class Func=NonConstFunc1<T,R,P1> >
struct memFunc1 : memFunc
{
    typedef typename LocVarType<P1>::Type PL1;
    typedef typename Func::memFunPtr memFunPtr;
    memFunPtr m_pfunc;
    T * m_pobj;
    enum {arity=1};
    memFunc1(T * pobj, memFunPtr pfunc):m_pfunc(pfunc),m_pobj(pobj){};
    virtual int operator()(const QValueVector<QVariant> & parms, QString & res, QString & err)
    {
        if (parms.size()!=arity)
        {
            err=QString("Number of provided parameters doesn't match function definition");
            return 1;
        };
        PL1 p1=fromQVariant<PL1>(parms[0]);
        res=toString<R>((m_pobj->*m_pfunc)(p1));
        return 0;
    };
    virtual int argc()
    {
        return arity;
    };
};

template <class T, class R, class P1, class P2, class Func=NonConstFunc2<T,R,P1,P2> >
struct memFunc2 : memFunc
{
    typedef typename LocVarType<P1>::Type PL1;
    typedef typename LocVarType<P2>::Type PL2;
    typedef typename Func::memFunPtr memFunPtr;
    memFunPtr m_pfunc;
    T * m_pobj;
    enum {arity=2};
    memFunc2(T * pobj, memFunPtr pfunc):m_pfunc(pfunc),m_pobj(pobj){};
    virtual int operator()(const QValueVector<QVariant> & parms, QString & res, QString & err)
    {
        if (parms.size()!=arity)
        {
            err=QString("Number of provided parameters doesn't match function definition");
            return 1;
        };
        PL1 p1=fromQVariant<PL1>(parms[0]);
        PL2 p2=fromQVariant<PL2>(parms[1]);
        res=toString<R>((m_pobj->*m_pfunc)(p1,p2));
        return 0;
    };
    virtual int argc()
    {
        return arity;
    };
};

template <class T, class R, class P1, class P2, class P3, class Func=NonConstFunc3<T,R,P1,P2,P3> >
struct memFunc3 : memFunc
{
    typedef typename LocVarType<P1>::Type PL1;
    typedef typename LocVarType<P2>::Type PL2;
    typedef typename LocVarType<P3>::Type PL3;
    typedef typename Func::memFunPtr memFunPtr;
    memFunPtr m_pfunc;
    T * m_pobj;
    enum {arity=3};
    memFunc3(T * pobj, memFunPtr pfunc):m_pfunc(pfunc),m_pobj(pobj){};
    virtual int operator()(const QValueVector<QVariant> & parms, QString & res, QString & err)
    {
        if (parms.size()!=arity)
        {
            err=QString("Number of provided parameters doesn't match function definition");
            return 1;
        };
        PL1 p1=fromQVariant<PL1>(parms[0]);
        PL2 p2=fromQVariant<PL2>(parms[1]);
        PL3 p3=fromQVariant<PL3>(parms[2]);
        res=toString<R>((m_pobj->*m_pfunc)(p1,p2,p3));
        return 0;
    };
    virtual int argc()
    {
        return arity;
    };
};

template <class T, class R, class P1, class P2, class P3, class P4, class Func=NonConstFunc4<T,R,P1,P2,P3,P4> >
struct memFunc4 : memFunc
{
    typedef typename LocVarType<P1>::Type PL1;
    typedef typename LocVarType<P2>::Type PL2;
    typedef typename LocVarType<P3>::Type PL3;
    typedef typename LocVarType<P4>::Type PL4;
    typedef typename Func::memFunPtr memFunPtr;
    memFunPtr m_pfunc;
    T * m_pobj;
    enum {arity=4};
    memFunc4(T * pobj, memFunPtr pfunc):m_pfunc(pfunc),m_pobj(pobj){};
    virtual int operator()(const QValueVector<QVariant> & parms, QString & res, QString & err)
    {
        if (parms.size()!=arity)
        {
            err=QString("Number of provided parameters doesn't match function definition");
            return 1;
        };
        PL1 p1=fromQVariant<PL1>(parms[0]);
        PL2 p2=fromQVariant<PL2>(parms[1]);
        PL3 p3=fromQVariant<PL3>(parms[2]);
        PL4 p4=fromQVariant<PL4>(parms[3]);
        res=toString<R>((m_pobj->*m_pfunc)(p1,p2,p3,p4));
        return 0;
    };
    virtual int argc()
    {
        return arity;
    };
};

template <class T, class R, class P1, class P2, class P3, class P4, class P5, 
    class Func=NonConstFunc5<T,R,P1,P2,P3,P4,P5> >
struct memFunc5 : memFunc
{
    typedef typename LocVarType<P1>::Type PL1;
    typedef typename LocVarType<P2>::Type PL2;
    typedef typename LocVarType<P3>::Type PL3;
    typedef typename LocVarType<P4>::Type PL4;
    typedef typename LocVarType<P5>::Type PL5;
    typedef typename Func::memFunPtr memFunPtr;
    memFunPtr m_pfunc;
    T * m_pobj;
    enum {arity=5};
    memFunc5(T * pobj, memFunPtr pfunc):m_pfunc(pfunc),m_pobj(pobj){};
    virtual int operator()(const QValueVector<QVariant> & parms, QString & res, QString & err)
    {
        if (parms.size()!=arity)
        {
            err=QString("Number of provided parameters doesn't match function definition");
            return 1;
        };
        PL1 p1=fromQVariant<PL1>(parms[0]);
        PL2 p2=fromQVariant<PL2>(parms[1]);
        PL3 p3=fromQVariant<PL3>(parms[2]);
        PL4 p4=fromQVariant<PL4>(parms[3]);
        PL5 p5=fromQVariant<PL5>(parms[4]);
        res=toString<R>((m_pobj->*m_pfunc)(p1,p2,p3,p4,p5));
        return 0;
    };
    virtual int argc()
    {
        return arity;
    };
};

struct memProc
{
    virtual int operator()(const QValueVector<QVariant> & , QString & )
    {
        return 0;
    };
    virtual int argc()
    {
        return 0;
    };
};

template <class T, class FT=NonConstProc0<T> >
struct memProc0 : memProc
{
    typedef typename FT::Type memProcPtr;
    T * m_pobj;
    memProcPtr m_pproc;
    enum {arity=0};
    memProc0(T * pobj, memProcPtr pproc):m_pobj(pobj),m_pproc(pproc){};
    virtual int operator()(const QValueVector<QVariant> & parms, QString & err)
    {
        if (parms.size()!=arity)
        {
            err=QString("Number of provided parameters doesn't match function definition");
            return 1;
        };
        (m_pobj->*m_pproc)();
        return 0;
    };
    virtual int argc()
    {
        return arity;
    };
};

template <class T, class P1>
struct memProc1 : memProc
{
    typedef void (T::*memProcPtr)(P1);
    typedef typename LocVarType<P1>::Type PL1;
    T * m_pobj;
    memProcPtr m_pproc;
    enum {arity=1};
    memProc1(T * pobj, memProcPtr pproc):m_pobj(pobj),m_pproc(pproc){};
    virtual int operator()(const QValueVector<QVariant> & parms, QString & err)
    {
        if (parms.size()!=arity)
        {
            err=QString("Number of provided parameters doesn't match function definition");
            return 1;
        };
        PL1 p1=fromQVariant<PL1>(parms[0]);
        (m_pobj->*m_pproc)(p1);
        return 0;
    };
    virtual int argc()
    {
        return arity;
    };
};

template <class T, class P1, class P2>
struct memProc2 : memProc
{
    typedef void (T::*memProcPtr)(P1,P2);
    typedef typename LocVarType<P1>::Type PL1;
    typedef typename LocVarType<P2>::Type PL2;
    memProcPtr m_pproc;
    T * m_pobj;
    enum {arity=2};
    memProc2(T * pobj, memProcPtr pproc):m_pproc(pproc),m_pobj(pobj){};
    virtual int operator()(const QValueVector<QVariant> & parms, QString & err)
    {
        if (parms.size()!=arity)
        {
            err=QString("Number of provided parameters doesn't match function definition");
            return 1;
        };
        PL1 p1=fromQVariant<PL1>(parms[0]);
        PL2 p2=fromQVariant<PL2>(parms[1]);
        (m_pobj->*m_pproc)(p1,p2);
        return 0;
    };
    virtual int argc()
    {
        return arity;
    };
};

template <class T, class P1, class P2, class P3>
struct memProc3 : memProc
{
    typedef void (T::*memProcPtr)(P1,P2,P3);
    typedef typename LocVarType<P1>::Type PL1;
    typedef typename LocVarType<P2>::Type PL2;
    typedef typename LocVarType<P3>::Type PL3;
    memProcPtr m_pproc;
    T * m_pobj;
    enum {arity=3};
    memProc3(T * pobj, memProcPtr pproc):m_pproc(pproc),m_pobj(pobj){};
    virtual int operator()(const QValueVector<QVariant> & parms, QString & err)
    {
        if (parms.size()!=arity)
        {
            err=QString("Number of provided parameters doesn't match function definition");
            return 1;
        };
        PL1 p1=fromQVariant<PL1>(parms[0]);
        PL2 p2=fromQVariant<PL2>(parms[1]);
        PL3 p3=fromQVariant<PL3>(parms[2]);
        (m_pobj->*m_pproc)(p1,p2,p3);
        return 0;
    };
    virtual int argc()
    {
        return arity;
    };
};

template <class T, class P1, class P2, class P3, class P4>
struct memProc4 : memProc
{
    typedef void (T::*memProcPtr)(P1,P2,P3,P4);
    typedef typename LocVarType<P1>::Type PL1;
    typedef typename LocVarType<P2>::Type PL2;
    typedef typename LocVarType<P3>::Type PL3;
    typedef typename LocVarType<P4>::Type PL4;
    memProcPtr m_pproc;
    T * m_pobj;
    enum {arity=4};
    memProc4(T * pobj, memProcPtr pproc):m_pproc(pproc),m_pobj(pobj){};
    virtual int operator()(const QValueVector<QVariant> & parms, QString & err)
    {
        if (parms.size()!=arity)
        {
            err=QString("Number of provided parameters doesn't match function definition");
            return 1;
        };
        PL1 p1=fromQVariant<PL1>(parms[0]);
        PL2 p2=fromQVariant<PL2>(parms[1]);
        PL3 p3=fromQVariant<PL3>(parms[2]);
        PL4 p4=fromQVariant<PL4>(parms[3]);
        (m_pobj->*m_pproc)(p1,p2,p3,p4);
        return 0;
    };
    virtual int argc()
    {
        return arity;
    };
};

template <class T, class P1, class P2, class P3, class P4, class P5>
struct memProc5 : memProc
{
    typedef void (T::*memProcPtr)(P1,P2,P3,P4,P5);
    typedef typename LocVarType<P1>::Type PL1;
    typedef typename LocVarType<P2>::Type PL2;
    typedef typename LocVarType<P3>::Type PL3;
    typedef typename LocVarType<P4>::Type PL4;
    typedef typename LocVarType<P5>::Type PL5;
    memProcPtr m_pproc;
    T * m_pobj;
    enum {arity=5};
    memProc5(T * pobj, memProcPtr pproc):m_pproc(pproc),m_pobj(pobj){};
    virtual int operator()(const QValueVector<QVariant> & parms, QString & err)
    {
        if (parms.size()!=arity)
        {
            err=QString("Number of provided parameters doesn't match function definition");
            return 1;
        };
        PL1 p1=fromQVariant<PL1>(parms[0]);
        PL2 p2=fromQVariant<PL2>(parms[1]);
        PL3 p3=fromQVariant<PL3>(parms[2]);
        PL4 p4=fromQVariant<PL4>(parms[3]);
        PL5 p5=fromQVariant<PL5>(parms[4]);
        (m_pobj->*m_pproc)(p1,p2,p3,p4,p5);
        return 0;
    };
    virtual int argc()
    {
        return arity;
    };
};

#endif
