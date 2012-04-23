/****************************************************************************
** $Id: teinterface_p.h,v 1.1 2006/02/14 12:05:25 leader Exp $
**
** Definition of AExtensionInterface class
**
** Created : 20060209
**
** Copyright (C) 2003-2006 Leader InfoTech.  All rights reserved.
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
** See http://www.leaderit.ru/page=ananas or email sales@leaderit.ru
** See http://www.leaderit.ru/gpl/ for GPL licensing information.
**
** Contact org@leaderit.ru if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#ifndef TEINTERFACE_H
#define TEINTERFACE_H

#include <teglobal.h>
#ifndef QT_H
#include <private/qcom_p.h>
#endif // QT_H

#ifndef QT_NO_COMPONENT

// {EDDD5AD8-DF3C-400c-A713-163B72FE5F61}
#ifndef IID_TEFactory
#define IID_TEFactory QUuid(0xeddd5ad8, 0xdf3c, 0x400c, 0xa7, 0x13, 0x16, 0x3b, 0x72, 0xfe, 0x5f, 0x61)
#endif

class TEBase;

struct LIB_EXPORT TEFactoryInterface : public QFeatureListInterface
{
    virtual TEBase* create( const QString& name ) = 0;
};

#endif //QT_NO_COMPONENT
#endif // TEINTERFACE_P_H
