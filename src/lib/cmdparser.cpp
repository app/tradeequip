/****************************************************************************
** $Id: cmdparser.cpp,v 1.6 2006/07/04 06:07:07 red75 Exp $
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

#include "cmdparser.h"
#include <qlocale.h>
/*!
    \class ExecEngine cmdparser.h
    \brief Command interpreter for TEBase classes.

*/


/*! /en Constructs an object, which can be used to bind text commands to arbitrary object's member function call. /_en
 *  /ru Создает объект, позволяющий привязать текстовую команду к вызову функции-члена заданного объекта. /_ru
 */
ExecEngine::ExecEngine() : m_pTextCodec(0)
{
    addProcBinding1<ExecEngine,const QString &>(this,&ExecEngine::setEncoding,"eeSetEncoding");
    addFuncBinding0<ExecEngine,QString,ConstFunc0<ExecEngine,QString> >(this,&ExecEngine::help,"eeHelp");
//    addFuncBinding(*this,this,&ExecEngine::help,"eeHelp");
};

/*! /en  /_en
 *  /ru  /_ru
 */
ExecEngine::~ExecEngine()
{ // clean the map up
    FuncDescToMemFunc::iterator i1;
    for(i1=m_mapMemFunc.begin();i1!=m_mapMemFunc.end();i1++)
        delete (*i1);
    FuncDescToMemProc::iterator i2;
    for(i2=m_mapMemProc.begin();i2!=m_mapMemProc.end();i2++)
        delete (*i2);
};

/*! /en Sets character encoding for nonunicode parse(). /_en
 *  /ru Устанавливает кодировку для неюникодной версии parse(). /_ru
 */
void ExecEngine::setEncoding(const QString & encoding)
{
    m_pTextCodec=QTextCodec::codecForName(encoding.ascii());
};

/*! /en  /_en
 *  /ru Неюникодная версия parse(). Использует кодировку указанную функцией setEncoding(). /_ru
 */
int ExecEngine::parse(const char * cmd, QString & func, QValueVector<QVariant> & parms)
{
    QString ucmd;
    m_error="";
    if (m_pTextCodec)
        ucmd=m_pTextCodec->toUnicode(cmd);
    else
        ucmd=QString(cmd);
    return parse(ucmd,func,parms);
};

/*! /en  /_en
 *  /ru Выполняет синтаксический разбор строки ucmd.
 * /_ru
 * command ::= <function> '(' [ <parameter> ',' ... ] ')'
 * parameter ::= number | string
 * number ::= common number representation
 * string ::= '"' <non-double-quote-chars>|'\"' '"'
 */
int ExecEngine::parse(const QString & ucmd, QString & func, QValueVector<QVariant> & parms)
{
// All chars from begin to first '(' character go into funcname
    int lparapos=ucmd.find('(');
    if (lparapos==-1)
    {
        m_error=QString("Syntax error: '(' wasn't found");
        return 1;
    };
    func=ucmd.mid(0,(uint)lparapos).stripWhiteSpace();
    if (func.isEmpty())
    {
        m_error=QString("Syntax error: function name wasn't found");
        return 1;
    };
//  ucmd=ucmd.mid(lparapos+1);
// ucmd contains comma separated parameters list now.
    uint curchr=(uint)(lparapos+1);
    parms.clear();
    QVariant curval;
    QString curstr;
    QString vl;
    QChar ch;
    QLocale cloc(QLocale::C);
    int delimpos=0;
state1:
    // state1 - skipping whitespaces before parameter
    if (curchr==ucmd.length())
    { // eos is found - error
        m_error=QString("Parameter %1 was not found").arg(parms.size()+1);
        goto stateerror;
    }
    else 
    {
        ch=ucmd.at(curchr);
        if (ch==QChar(')') && parms.size()==0)
            goto stateend; // There are no parameters.
        if (ch.isSpace())
        {
            curchr++;
            goto state1;
        }
        else if (ch.isNumber()||ch==QChar('-')||ch==QChar('+'))
            goto state2; // numeric parameter 
        else if (ch==QChar('"'))
            goto state3; // string parameter
    };
    m_error=QString("Unrecognized parameter type at %1").arg(curchr);
    goto stateerror;
state2:
    // state2 - parse numeric constant
    delimpos=ucmd.find(',',(int)curchr);
    if (delimpos==-1)
        delimpos=ucmd.find(')',(int)curchr);
    if (delimpos==-1)
    {
        m_error=QString("Parameter's delimeter was not found at %1").arg(curchr);
        goto stateerror;
    };
    vl=ucmd.mid(curchr,delimpos-curchr).stripWhiteSpace();
    bool ok;
    curval=cloc.toDouble(vl,&ok);
    if (!ok)
    {
        curval=cloc.toLongLong(vl,&ok);
        if (!ok)
        {
            m_error=QString("Parameter cannot be converted to numerical value at %1").arg(curchr);
        };
    };
    curchr=(uint)delimpos;
    goto statenext;
state3:
    // state3 - parse string constant
    curchr++;
    curstr="";
    while(curchr<ucmd.length())
    {
        ch=ucmd.at(curchr);
        if (ch=='\\')
        {
            curchr++;
            if (curchr>=ucmd.length())
            {
                m_error=QString("Backslash at the end of line.");
                goto stateerror;
            };
            ch=ucmd.at(curchr);
            curstr+=ch;
        }
        else if (ch=='"')
        {
            curval=curstr;
            delimpos=ucmd.find(',',(int)curchr);
            if (delimpos==-1)
                delimpos=ucmd.find(')',(int)curchr);
            if (delimpos==-1)
            {
                m_error=QString("Cannot find parameters' delimeter after position %1").arg(curchr);
                goto stateerror;
            };
            curchr=(uint)delimpos;
            goto statenext;
        }
        else
            curstr+=ch;
        curchr++;
    };
    goto stateerror;
statenext:
    parms.push_back(curval);
    ch=ucmd.at(curchr);
    if (ch==QChar(','))
    {
        curchr++;
        goto state1;
    }
    else if (ch==QChar(')'))
        goto stateend;
    m_error=QString("Invalid parameters' delimeter at %1").arg(curchr);
    goto stateerror;
stateend:
    return 0;
stateerror:
    return 1;
};

/*! /en  /_en
 *  /ru Неюникодная версия execute() /_ru
 */
int ExecEngine::execute(const char * cmd)
{
    QString func;
    QValueVector<QVariant> parms;
    int res=parse(cmd,func,parms);
    if (res)
        return res;
    FuncDescToMemFunc::iterator it;
    it=m_mapMemFunc.find(FuncDesc(func,(int)parms.size()));
    if (it==m_mapMemFunc.end())
    {
        FuncDescToMemProc::iterator it;
        it=m_mapMemProc.find(FuncDesc(func,(int)parms.size()));
        if (it==m_mapMemProc.end())
        { // function description doesn't match anything
            m_error=QString("Implementation for function '%1' with %2 parameters isn't found").arg(func).arg(parms.size());
            return 1;
        };
        m_result="";
        return (**it)(parms,m_error);
    };
    return (**it)(parms,m_result,m_error);
};

/*! /en  /_en
 *  /ru Вызывает функцию связанную с именем полученным после разбора строки ucmd. /_ru
 */
int ExecEngine::execute(const QString & ucmd)
{
    QString func;
    QValueVector<QVariant> parms;
    int res=parse(ucmd,func,parms);
    if (res)
        return res;
    FuncDescToMemFunc::iterator it;
    it=m_mapMemFunc.find(FuncDesc(func,(int)parms.size()));
    if (it==m_mapMemFunc.end())
    {
        FuncDescToMemProc::iterator it;
        it=m_mapMemProc.find(FuncDesc(func,(int)parms.size()));
        if (it==m_mapMemProc.end())
        { // function description doesn't match anything
            m_error=QString("Implementation for function '%1' with %2 parameters isn't found").arg(func).arg(parms.size());
            return 1;
        };
        m_result="";
        return (**it)(parms,m_error);
    };
    return (**it)(parms,m_result,m_error);
};

/*! /en  /_en
 *  /ru Возвращает список зарегистрированных имен функций (в скобках количество параметров), разделенных запятыми. /_ru
 */
QString ExecEngine::help() const
{
	QString ret;
 	bool nonempty=false;
	FuncDescToMemFunc::const_iterator i1;
	i1=m_mapMemFunc.begin();
	if (i1!=m_mapMemFunc.end())
	{
		nonempty=true;
		ret.append(QString("%1 (%2)").arg(i1.key().first).arg(i1.key().second));
		i1++;
		for(;i1!=m_mapMemFunc.end();i1++)
			ret.append(QString(", %1 (%2)").arg(i1.key().first).arg(i1.key().second));
	};
	FuncDescToMemProc::const_iterator i2;
	i2=m_mapMemProc.begin();
	if (i2!=m_mapMemProc.end())
	{
		if (nonempty)
			ret.append(QString(", "));
		ret.append(QString("%1 (%2)").arg(i2.key().first).arg(i2.key().second));
		i2++;
		for(;i2!=m_mapMemProc.end();i2++)
			ret.append(QString(", %1 (%2)").arg(i2.key().first).arg(i2.key().second));
	};
	return ret;
};
