/****************************************************************************
** $Id: templexports.h,v 1.7 2006/06/21 05:35:58 red75 Exp $
**
** Trade equipment common portable library project
**
** Copyright (C) 1999-2006 Leader InfoTech.  All rights reserved.
**
** This file is part of the Library of the 
** Portable trade equipment applications project.
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

/*
	This header file makes instantiated templates dll-exported or
	imports some of them from qt-mt.dll.
	NB: DO NOT include this header file in lib.pro! 
	MOC cannot process template exports sometimes.

	Этот заголовочный файл экспортирует инстанциированные шаблоны из dll или
	импортирует шаблоны уже инстанциированные в других dll.
	ВНИМАНИЕ: НЕ включайте этот заголовочный файл в файл проекта lib.pro!
	MOC иногда не может обработать операторы экспорта инстанциированных
	шаблонов и вылетает с сообщением о синтаксической ошибке.
*/

#ifdef Q_OS_WIN32

#ifdef TECASHREGISTERBASE_H
#ifndef TECASHREGISTER_BASE_EXPORTED
#define TECASHREGISTER_BASE_EXPORTED
LIB_TEMPLATE_EXTERN template class LIB_EXPORT QDict <QVariant>;
LIB_TEMPLATE_EXTERN template class LIB_EXPORT QValueList <PurchaseInfo>;
LIB_TEMPLATE_EXTERN template class LIB_EXPORT QMap <int, double>;
#endif
#endif

#ifdef TESCANNER_BASE_H
#ifndef TESCANNER_BASE_EXPORTED
#define TESCANNER_BASE_EXPORTED
TEMPLATE_EXTERN template class LIB_DLLIMPORT QValueList<QString>; 
#endif
#endif

#ifdef QGETOPT_H
#ifndef GETOPT_EXPORTED
#define GETOPT_EXPORTED
LIB_TEMPLATE_EXTERN template class LIB_EXPORT QValueList<GetOpt::Option>; 
#endif
#endif

#ifdef PREFERENCES_H
#ifndef PREF_EXPORTED
#define PREF_EXPORTED
LIB_TEMPLATE_EXTERN template class LIB_EXPORT QMap<QString, Preferences::PrefMap>;
#endif
#endif

#ifdef TEINIFILE_H
#ifndef TDATAFILE_EXPORTED
#define TDATAFILE_EXPORTED
LIB_TEMPLATE_EXTERN template class LIB_EXPORT QMap<QString, type_ValueList>;
LIB_TEMPLATE_EXTERN template class LIB_EXPORT QMapIterator<QString, type_ValueList>;
#endif
#endif

#ifdef TEBASE_H
#ifndef TEBASE_EXPORTED
#define TEBASE_EXPORTED
LIB_TEMPLATE_EXTERN template class LIB_DLLIMPORT QValueList<int>;
#endif
#endif

#endif
