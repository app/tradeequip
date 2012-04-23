/****************************************************************************
** $Id: teplugin.h,v 1.3 2006/04/29 10:15:47 red75 Exp $
**
** Extension plugin object header file of
** Ananas application library
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) 2003-2005 Grigory Panov, Yoshkar-Ola.
**
** This file is part of the Designer application of the Ananas
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

#ifndef TEPLUGIN_H
#define TEPLUGIN_H

#ifndef QT_H
#include "qgplugin.h"
#include "qstringlist.h"
#endif // QT_H
#include "teglobal.h"
#include <qobject.h>

#ifndef QT_NO_COMPONENT

class TEBase;
class TEPluginPrivate;

//#define TE_EXPORT_PLUGIN(pluginobjectname) typedef TEPlugin<pluginobjectname> pluginobjectname##Plugin; Q_EXPORT_PLUGIN(pluginobjectname)
#define TE_EXPORT_PLUGIN(pluginobjectname) Q_EXPORT_PLUGIN(pluginobjectname)

/**
 * \en
 * Class for work with extensions
 * \_en
 * \ru
 * \_ru
 */
class  LIB_EXPORT TEPluginBase : public QGPlugin
{
    Q_OBJECT
public:

    TEPluginBase();
    ~TEPluginBase();
    virtual QStringList keys() const = 0;
    virtual TEBase *create( const QString &key ) = 0;

private:
    TEPluginPrivate *d;
};


template<class Type>
class TEPlugin : public TEPluginBase
{
//    Q_OBJECT
public:

    TEPlugin()
    { 
	    Type o;
	    extName = o.name();
    };
    ~TEPlugin(){};
    QStringList keys() const 
    {
      QStringList l;
      l << extName;
      return l;
    };
    TEBase *create( const QString &key ) 
    {
      if (key == extName) return new Type();
      return 0;
    };
private:
    QString extName;
};

#endif // QT_NO_COMPONENT
#endif //TEPLUGIN_H
