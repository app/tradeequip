/****************************************************************************
** $Id: teconnecttcp.h,v 1.1 2006/07/12 09:12:32 leader Exp $
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
/*
   TCP trade equipment data connection object
*/
#ifndef TECONNECTTCP_H
#define TECONNECTTCP_H

#include <teglobal.h>
#include <teconnect.h>
#include <templexports.h>

//#include <qobject.h>
#include <qdict.h>
#include <cmdparser.h>

//---------------------------------------------------------------------------
class LIB_EXPORT TEConnectTCP : public TEConnect
{
    Q_OBJECT

public:

    TEConnectTCP( int pn );
    ~TEConnectTCP();
};
//---------------------------------------------------------------------------

#endif
