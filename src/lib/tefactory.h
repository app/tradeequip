/****************************************************************************
** $Id: tefactory.h,v 1.2 2006/02/15 21:57:40 red75 Exp $
**
** ...
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

#ifndef TEFACTORY_H
#define TEFACTORY_H

#ifndef QT_H
#include "qstringlist.h"
#endif // QT_H

class QString;
class TEBase;

class LIB_EXPORT TEFactory
{
public:
#ifndef QT_NO_STRINGLIST
    static QStringList keys();
#endif
    static TEBase *create( const QString& );
};

#endif //TEFACTORY_H
