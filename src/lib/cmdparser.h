/****************************************************************************
** $Id: cmdparser.h,v 1.10 2006/03/03 13:11:04 red75 Exp $
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

#ifndef _CMDPARSER_H_
#define _CMDPARSER_H_

#include "teglobal.h"
#include <qstring.h>
#include <qstringlist.h>
#include <qpair.h>
#include <qtextcodec.h>
#include <qvariant.h>
#include <qvaluevector.h>
#include "conv.h"

// This class is intended to parse command lines and to handle different encodings,
// along with call forwarding.

LIB_TEMPLATE_EXTERN template struct LIB_EXPORT QPair<QString,int>;
LIB_TEMPLATE_EXTERN template class LIB_EXPORT QMap<QPair<QString,int>,memFunc*>;
LIB_TEMPLATE_EXTERN template class LIB_EXPORT QMap<QPair<QString,int>,memProc*>;

class LIB_EXPORT ExecEngine
{
    QTextCodec * m_pTextCodec;
    QString m_error;
    QString m_result;
    // function name, arity
    typedef QPair<QString,int> FuncDesc; 

    // This class maps function description to functor
    typedef QMap<FuncDesc,memFunc*> FuncDescToMemFunc;
    typedef QMap<FuncDesc,memProc*> FuncDescToMemProc;

    FuncDescToMemFunc m_mapMemFunc;
    FuncDescToMemProc m_mapMemProc;

public:
    ExecEngine();
    virtual ~ExecEngine();

    int execute(const char * cmd);
    int execute(const QString & ucmd);
    QString result() const {return m_result;};
    QString error() const {return m_error;};
    QString help() const; // it returns list of all functions

    void setEncoding(const QString & encoding);
    int parse(const char * cmd, QString & func, QValueVector<QVariant> & parms);
    int parse(const QString & ucmd, QString & func, QValueVector<QVariant> & parms);

    template <class T, class R>
        int addFuncBinding(T * pobj, R (T::*pfunc)(void), const QString & name)
    { // if there was no value, this string evaluates to 'delete 0', which is acceptable
        delete m_mapMemFunc[FuncDesc(name,0)];
        m_mapMemFunc[FuncDesc(name,0)]=new memFunc0<T,R>(pobj,pfunc);
        return 0;
    };

    template <class T, class R>
        int addFuncBinding(T * pobj, R (T::*pfunc)(void) const, const QString & name)
    { // if there was no value, this string evaluates to 'delete 0', which is acceptable
        delete m_mapMemFunc[FuncDesc(name,0)];
        m_mapMemFunc[FuncDesc(name,0)]=new memFunc0<T,R, ConstFunc0<T,R> >(pobj,pfunc);
        return 0;
    };

    template <class T, class R, class P1>
        int addFuncBinding(T * pobj, R (T::*pfunc)(P1), const QString & name)
    { // if there was no value, this string evaluates to 'delete 0', which is acceptable
        delete m_mapMemFunc[FuncDesc(name,1)];
        m_mapMemFunc[FuncDesc(name,1)]=new memFunc1<T,R,P1>(pobj,pfunc);
        return 0;
    };

    template <class T, class R, class P1>
        int addFuncBinding(T * pobj, R (T::*pfunc)(P1) const, const QString & name)
    { // if there was no value, this string evaluates to 'delete 0', which is acceptable
        delete m_mapMemFunc[FuncDesc(name,1)];
        m_mapMemFunc[FuncDesc(name,1)]=new memFunc1<T,R,P1, ConstFunc1<T,R,P1> >(pobj,pfunc);
        return 0;
    };

    template <class T, class R, class P1, class P2>
        int addFuncBinding(T * pobj, R (T::*pfunc)(P1,P2), const QString & name)
    { // if there was no value, this string evaluates to 'delete 0', which is acceptable
        delete m_mapMemFunc[FuncDesc(name,2)];
        m_mapMemFunc[FuncDesc(name,2)]=new memFunc2<T,R,P1,P2>(pobj,pfunc);
        return 0;
    };

    template <class T, class R, class P1, class P2>
        int addFuncBinding(T * pobj, R (T::*pfunc)(P1,P2) const, const QString & name)
    { // if there was no value, this string evaluates to 'delete 0', which is acceptable
        delete m_mapMemFunc[FuncDesc(name,2)];
        m_mapMemFunc[FuncDesc(name,2)]=new memFunc2<T,R,P1,P2, ConstFunc2<T,R,P1,P2> >(pobj,pfunc);
        return 0;
    };

    template <class T, class R, class P1, class P2, class P3>
        int addFuncBinding(T * pobj, R (T::*pfunc)(P1,P2,P3), const QString & name)
    { // if there was no value, this string evaluates to 'delete 0', which is acceptable
        delete m_mapMemFunc[FuncDesc(name,3)];
        m_mapMemFunc[FuncDesc(name,3)]=new memFunc3<T,R,P1,P2,P3>(pobj,pfunc);
        return 0;
    };

    template <class T, class R, class P1, class P2, class P3>
        int addFuncBinding(T * pobj, R (T::*pfunc)(P1,P2,P3) const, const QString & name)
    { // if there was no value, this string evaluates to 'delete 0', which is acceptable
        delete m_mapMemFunc[FuncDesc(name,3)];
        m_mapMemFunc[FuncDesc(name,3)]=new memFunc3<T,R,P1,P2,P3, ConstFunc3<T,R,P1,P2,P3> >(pobj,pfunc);
        return 0;
    };

    template <class T, class R, class P1, class P2, class P3, class P4>
        int addFuncBinding(T * pobj, R (T::*pfunc)(P1,P2,P3,P4), const QString & name)
    { // if there was no value, this string evaluates to 'delete 0', which is acceptable
        delete m_mapMemFunc[FuncDesc(name,4)];
        m_mapMemFunc[FuncDesc(name,4)]=new memFunc4<T,R,P1,P2,P3,P4>(pobj,pfunc);
        return 0;
    };

    template <class T, class R, class P1, class P2, class P3, class P4>
        int addFuncBinding(T * pobj, R (T::*pfunc)(P1,P2,P3,P4) const, const QString & name)
    { // if there was no value, this string evaluates to 'delete 0', which is acceptable
        delete m_mapMemFunc[FuncDesc(name,4)];
        m_mapMemFunc[FuncDesc(name,4)]=new memFunc4<T,R,P1,P2,P3,P4, ConstFunc4<T,R,P1,P2,P3,P4> >(pobj,pfunc);
        return 0;
    };

    template <class T, class R, class P1, class P2, class P3, class P4, class P5>
        int addFuncBinding(T * pobj, R (T::*pfunc)(P1,P2,P3,P4,P5), const QString & name)
    { // if there was no value, this string evaluates to 'delete 0', which is acceptable
        delete m_mapMemFunc[FuncDesc(name,5)];
        m_mapMemFunc[FuncDesc(name,5)]=new memFunc5<T,R,P1,P2,P3,P4,P5>(pobj,pfunc);
        return 0;
    };

    template <class T, class R, class P1, class P2, class P3, class P4, class P5>
        int addFuncBinding(T * pobj, R (T::*pfunc)(P1,P2,P3,P4,P5) const, const QString & name)
    { // if there was no value, this string evaluates to 'delete 0', which is acceptable
        delete m_mapMemFunc[FuncDesc(name,5)];
        m_mapMemFunc[FuncDesc(name,5)]=new memFunc5<T,R,P1,P2,P3,P4,P5, ConstFunc5<T,R,P1,P2,P3,P4,P5> >(pobj,pfunc);
        return 0;
    };

    template <class T, class R>
        int addFuncBinding0(T * pobj, typename memFunc0<T,R>::memFunPtr pfunc, const QString & name)
    { // if there was no value, this string evaluates to 'delete 0', which is acceptable
        delete m_mapMemFunc[FuncDesc(name,0)];
        m_mapMemFunc[FuncDesc(name,0)]=new memFunc0<T,R>(pobj,pfunc);
        return 0;
    };

    template <class T, class R, class FuncType >
        int addFuncBinding0(T * pobj, typename memFunc0<T,R,FuncType>::memFunPtr pfunc, const QString & name)
    { // if there was no value, this string evaluates to 'delete 0', which is acceptable
        delete m_mapMemFunc[FuncDesc(name,0)];
        m_mapMemFunc[FuncDesc(name,0)]=new memFunc0<T,R,FuncType>(pobj,pfunc);
        return 0;
    };

    template <class T, class R, class P1>
        int addFuncBinding1(T * pobj, typename memFunc1<T,R,P1>::memFunPtr pfunc, const QString & name)
    {
        delete m_mapMemFunc[FuncDesc(name,1)];
        m_mapMemFunc[FuncDesc(name,1)]=new memFunc1<T,R,P1>(pobj,pfunc);
        return 0;
    };

    template <class T, class R, class P1, class P2>
        int addFuncBinding2(T * pobj, typename memFunc2<T,R,P1,P2>::memFunPtr pfunc, const QString & name)
    {
        delete m_mapMemFunc[FuncDesc(name,2)];
        m_mapMemFunc[FuncDesc(name,2)]=new memFunc2<T,R,P1,P2>(pobj,pfunc);
        return 0;
    };

    template <class T, class R, class P1, class P2, class P3>
        int addFuncBinding3(T * pobj, typename memFunc3<T,R,P1,P2,P3>::memFunPtr pfunc, const QString & name)
    {
        delete m_mapMemFunc[FuncDesc(name,3)];
        m_mapMemFunc[FuncDesc(name,3)]=new memFunc3<T,R,P1,P2,P3>(pobj,pfunc);
        return 0;
    };
    template <class T, class R, class P1, class P2, class P3, class P4>
        int addFuncBinding4(T * pobj, typename memFunc4<T,R,P1,P2,P3,P4>::memFunPtr pfunc, const QString & name)
    {
        delete m_mapMemFunc[FuncDesc(name,4)];
        m_mapMemFunc[FuncDesc(name,4)]=new memFunc4<T,R,P1,P2,P3,P4>(pobj,pfunc);
        return 0;
    };

    template <class T, class R, class P1, class P2, class P3, class P4, class P5>
        int addFuncBinding5(T * pobj, typename memFunc5<T,R,P1,P2,P3,P4,P5>::memFunPtr pfunc, const QString & name)
    {
        delete m_mapMemFunc[FuncDesc(name,5)];
        m_mapMemFunc[FuncDesc(name,5)]=new memFunc5<T,R,P1,P2,P3,P4,P5>(pobj,pfunc);
        return 0;
    };

    template <class T, class P1, class P2, class P3, class P4, class P5>
        int addProcBinding5(T * pobj, typename memProc5<T,P1,P2,P3,P4,P5>::memProcPtr pfunc, const QString & name)
    {
        delete m_mapMemProc[FuncDesc(name,5)];
        m_mapMemProc[FuncDesc(name,5)]=new memProc5<T,P1,P2,P3,P4,P5>(pobj,pfunc);
        return 0;
    };

    template <class T, class P1, class P2, class P3, class P4>
        int addProcBinding4(T * pobj, typename memProc4<T,P1,P2,P3,P4>::memProcPtr pfunc, const QString & name)
    {
        delete m_mapMemProc[FuncDesc(name,4)];
        m_mapMemProc[FuncDesc(name,4)]=new memProc4<T,P1,P2,P3,P4>(pobj,pfunc);
        return 0;
    };

    template <class T, class P1, class P2, class P3>
        int addProcBinding3(T * pobj, typename memProc3<T,P1,P2,P3>::memProcPtr pfunc, const QString & name)
    {
        delete m_mapMemProc[FuncDesc(name,3)];
        m_mapMemProc[FuncDesc(name,3)]=new memProc3<T,P1,P2,P3>(pobj,pfunc);
        return 0;
    };

    template <class T, class P1, class P2>
        int addProcBinding2(T * pobj, typename memProc2<T,P1,P2>::memProcPtr pfunc, const QString & name)
    {
        delete m_mapMemProc[FuncDesc(name,2)];
        m_mapMemProc[FuncDesc(name,2)]=new memProc2<T,P1,P2>(pobj,pfunc);
        return 0;
    };

    template <class T, class P1>
        int addProcBinding1(T * pobj, typename memProc1<T,P1>::memProcPtr pfunc, const QString & name)
    {
        delete m_mapMemProc[FuncDesc(name,1)];
        m_mapMemProc[FuncDesc(name,1)]=new memProc1<T,P1>(pobj,pfunc);
        return 0;
    };

    template <class T>
        int addProcBinding0(T * pobj, typename memProc0<T>::memProcPtr pfunc, const QString & name)
    {
        delete m_mapMemProc[FuncDesc(name,0)];
        m_mapMemProc[FuncDesc(name,0)]=new memProc0<T>(pobj,pfunc);
        return 0;
    };

    template <class T, class FT>
        int addProcBinding0(T * pobj, typename memProc0<T,FT>::memProcPtr pfunc, const QString & name)
    {
        delete m_mapMemProc[FuncDesc(name,0)];
        m_mapMemProc[FuncDesc(name,0)]=new memProc0<T,FT>(pobj,pfunc);
        return 0;
    };
};

// Helper functions, which are intended to simplify function binding code

/*
template <class T>
int addProcBinding(ExecEngine & ee, T * pObj, void (T::*func)() const, const QString & name)
{
	return ee.addProcBinding0<T, ConstProc0<T> >(pObj, func, name);
}

template <class T>
int addProcBinding(ExecEngine & ee, T * pObj, void (T::*func)(), const QString & name)
{
	return ee.addProcBinding0<T>(pObj,func,name);
}

template <class Ty, class R>
int addFuncBinding(ExecEngine & ee, Ty * pObj, R (Ty::*func)() const, const QString & name)
{
	return ee.addFuncBinding0<Ty,R,ConstFunc0<Ty,R> >(pObj,func,name);
}

template <class T, class R>
int addFuncBinding(ExecEngine & ee, T * pObj, R (T::*func)(), const QString & name)
{
	return ee.addFuncBinding0<T,R>(pObj,func,name);
}

template <class T, class R, class P1>
int addFuncBinding(ExecEngine & ee, T * pObj, R (T::*func)(P1), const QString & name)
{
	return ee.addFuncBinding1<T,R,P1>(pObj,func,name);
}

template <class T, class R, class P1>
int addFuncBinding(ExecEngine & ee, T * pObj, R (T::*func)(P1) const, const QString & name)
{
	return ee.addFuncBinding1<T,R,P1,ConstFunc1<T,R,P1> >(pObj,func,name);
}

template <class T, class R, class P1, class P2>
int addFuncBinding(ExecEngine & ee, T * pObj, R (T::*func)(P1,P2), const QString & name)
{
	return ee.addFuncBinding2<T,R,P1,P2>(pObj,func,name);
}

template <class T, class R, class P1, class P2>
int addFuncBinding(ExecEngine & ee, T * pObj, R (T::*func)(P1,P2) const, const QString & name)
{
	return ee.addFuncBinding2<T,R,P1,P2,ConstFunc1<T,R,P1,P2> >(pObj,func,name);
}
*/

#endif
