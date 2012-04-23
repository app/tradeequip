/****************************************************************************
** $Id: teglobal.h,v 1.14 2006/12/03 16:23:13 leader Exp $
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

#ifndef TEGLOBAL_H
#define TEGLOBAL_H
#include <qglobal.h>
#include <qgplugin.h>
#include <qstringlist.h>

#define DEBUG
//#define DEMOVERSION

#ifdef Q_OS_WIN32
#	include <windows.h>
#	include <math.h>
#	define _TTY_WIN_
#	define round(x) floor(x+0.55555555)
#endif
#ifdef Q_OS_UNIX
#	include <unistd.h>
#	define Sleep(a) usleep((a)*1000)
#	ifndef _TTY_POSIX_
#		define _TTY_POSIX_
#	endif
#endif

#if defined(Q_OS_WIN32) && !defined(LIB_NO_DLL)
#  define LIB_DLLIMPORT __declspec(dllimport)
#  define LIB_DLLEXPORT __declspec(dllexport)
#else
#  define LIB_DLLIMPORT
#  define LIB_DLLEXPORT
#endif

#if defined (Q_CC_MSVC)
#define TEMPLATE_EXTERN extern
#else
#define TEMPLATE_EXTERN
#endif

#ifdef TRADEEQUIP_EXPORT
#  define LIB_EXPORT LIB_DLLEXPORT
#else
#  define LIB_EXPORT LIB_DLLIMPORT
#endif

#ifdef TRADEEQUIP_EXPORT
#  define LIB_TEMPLATE_EXTERN
#else
#  define LIB_TEMPLATE_EXTERN TEMPLATE_EXTERN
#endif

#if defined(Q_CC_MSVC)
#  define vsnprintf _vsnprintf
#endif

#undef min
#undef max
/*
template <class T>
T & min(T & a, T & b)
{
    if (a<b) return a; else return b;
}

template <class T>
const T & min(const T & a, const T & b)
{
    if (a<b) return a; else return b;
}
*/
template <class U, class V>
U min(const U & a, const V & b)
{
    if (a<=(const U&)b) return a; else return (const U&)b;
}

template <class U, class V>
U max(const U & a, const V & b)
{
    if (a>=(const U&)b) return a; else return (const U&)b;
}
//#define min(a,b) (((a)<(b))?(a):(b))
//#endif

// -- error codes for new check-related functions
#define CRERROR_OK 0
#define CRERROR_NOTSUPPORTED 1
#define CRERROR_DEVICEERROR 2
#define CRERROR_INVALIDCALL 3
#define CRERROR_FATALDEVICEERROR 4
#define CRERROR_COMMERROR 5

/*!
\class TE teglobal.h
\~english
	\brief The TE class is a namespace for miscellaneous
	identifiers that need to be global-like in trade
	equipment objects.
\~russia
\brief Пространство имен trade equipment.
\~

*/
class TE
{
public:

/*! \brief Define connection capture mode for setCaptureMode()
*/
	enum CaptureMode
	{
		CM_Exclusive=0,
		CM_NonExclusive=1
	};

/*!
	Define trade equipment connection types.
*/
	enum EquipmentConnectionType {
		ECT_NOCONNECTION	=0x0,
		ECT_SERIAL		=0x1,	//!<
		ECT_KEYBOARD		=0x2,	//!<
		ECT_TCP			=0x3,	//!<
		ECT_TESERVER		=0x4	//!<
	};
/*!
	Define cash register operations codes.
*/
	enum CashRegisterOperations {
		CRO_SELL		= 0x0,	//!<
		CRO_BUY			= 0x1,	//!<
		CRO_SELLRET		= 0x2,	//!<
		CRO_BUYRET		= 0x3,	//!<
		CRO_SELLSTORNO		= 0x4,	//!<
		CRO_BUYSTORNO		= 0x5,	//!<
		CRO_CASHIN		= 0x6,	//!<
		CRO_CASHOUT		= 0x7,	//!<
		CRO_DISCOUNT		= 0x8,	//!<
		CRO_DISCOUNTPERCENT	= 0x9,	//!<
		CRO_OPMASK		= 0xF	//!<
		//CRO_SIMPLEMODE=0x8,
		//CRO_MODEMASK=0x8,
		//CRO_CASHLESS=0x10,
		//CRO_PAYMENTMASK=0x10
	};
};

class TEBase;

Q_EXTERN_C LIB_EXPORT QStringList te_drivers( const QString &baseClass = QString::null );
Q_EXTERN_C LIB_EXPORT bool te_driverExists( const QString &className, const QString &baseClass = QString::null );
Q_EXTERN_C LIB_EXPORT TEBase *te_createDriverObject( const QString &className );

LIB_EXPORT QString te_MD5( const QByteArray &data );
LIB_EXPORT QString te_MD5( const QString &data );
LIB_EXPORT int te_MD5_selftest();

// Encoding for msgfmt should be UTF-8
extern void cr_message(QString logFile, int msgtype, const char *msgfmt,...);
// Encoding for msgfmt should be UTF-8
extern void cfg_message(int msgtype, const char *msgfmt,...);

#endif
