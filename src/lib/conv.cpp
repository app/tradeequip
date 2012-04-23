/****************************************************************************
** $Id: conv.cpp,v 1.2 2006/03/03 13:11:04 red75 Exp $
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

#include <conv.h>

template <>
bool fromQVariant<bool>(const QVariant & s)
{
	return s.toInt()!=0;
};

template <>
int fromQVariant<int>(const QVariant & s)
{
    return s.toInt();
};

template <>
uint fromQVariant<uint>(const QVariant & s)
{
    return s.toUInt();
};

template <>
float fromQVariant<float>(const QVariant & s)
{
    return (float)s.toDouble();
};

template <>
double fromQVariant<double>(const QVariant & s)
{
    return s.toDouble();
};

template <>
Q_LLONG fromQVariant<Q_LLONG>(const QVariant & s)
{
    return s.toLongLong();
};

template <>
Q_ULLONG fromQVariant<Q_ULLONG>(const QVariant & s)
{
    return s.toULongLong();
};

template <>
Q_ULONG fromQVariant<Q_ULONG>(const QVariant & s)
{
	return s.toUInt();
};

template <>
Q_LONG fromQVariant<Q_LONG>(const QVariant & s)
{
	return s.toInt();
};

template <>
QString fromQVariant<QString>(const QVariant & s)
{
    return s.toString();
};

template <>
QCString fromQVariant<QCString>(const QVariant & s)
{
    return s.toCString();
};

template <>
QString toString<double>(double v)
{
    return QString("%1").arg(v,15,'f').stripWhiteSpace();
};

template <>
QString toString<QVariant>(QVariant v)
{
    return v.toString();
};

template <>
QString toString<float>(float v)
{
    return QString("%1").arg(v,15,'f').stripWhiteSpace();
};
