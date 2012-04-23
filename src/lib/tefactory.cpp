/****************************************************************************
** $Id: tefactory.cpp,v 1.4 2006/07/04 05:41:57 red75 Exp $
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

#include "teinterface_p.h" // up here for GCC 2.7.* compatibility
#include "tefactory.h"
#include <tebase.h>

#include <stdlib.h>
#include <private/qpluginmanager_p.h>
#ifndef QT_NO_COMPONENT
#include <qapplication.h>
#include <qobject.h>

class TEFactoryPrivate : public QObject
{
public:
    TEFactoryPrivate();
    ~TEFactoryPrivate();

    static QPluginManager<TEFactoryInterface> *manager;
};

static TEFactoryPrivate *instance = 0;
QPluginManager<TEFactoryInterface> *TEFactoryPrivate::manager = 0;

TEFactoryPrivate::TEFactoryPrivate()
: QObject( qApp )
{
#ifdef Q_OS_WIN32
  manager = new QPluginManager<TEFactoryInterface>( IID_TEFactory, qApp->applicationDirPath(), "/tradeequip", FALSE );
#else
    manager = new QPluginManager<TEFactoryInterface>( IID_TEFactory, "/usr/lib", "/tradeequip", FALSE );
#endif
}

TEFactoryPrivate::~TEFactoryPrivate()
{
    delete manager;
    manager = 0;

    instance = 0;
}

#endif //QT_NO_COMPONENT

/*!
    \class TEFactory tefactory.h
    \brief The TEFactory class creates TEBase objects.

    The style factory creates a AExtension object for a given key with
    AExtensionFactory::create(key).

    The styles are either built-in or dynamically loaded from a style
    plugin (see \l AExtensionPlugin).

    AExtensionFactory::keys() returns a list of valid keys, typically
    including "XXXX".
*/

/*!
    Creates a AExtension object that matches \a key. This is either a
    built-in extensions, or a extension from a extension plugin.

    \sa keys()
*/
TEBase *TEFactory::create( const QString& key )
{
    TEBase *ret = 0;
    QString extension = key; //key.lower();
#ifndef TE_NO_DRIVER_XXXX
//    if ( extension == "TEBase" )
//        ret = new TEBase(0);
#endif

    { } // Keep these here - they make the #ifdefery above work

#ifndef QT_NO_COMPONENT
    if(!ret) {
	if ( !instance )
	    instance = new TEFactoryPrivate;

	QInterfacePtr<TEFactoryInterface> iface;
	TEFactoryPrivate::manager->queryInterface( extension, &iface );

	if ( iface )
	    ret = iface->create( extension );
    }
    if(ret)
	ret->setName(key);
#endif
    return ret;
}

#ifndef QT_NO_STRINGLIST
/*!
    Returns the list of keys this factory can create styles for.

    \sa create()
*/
QStringList TEFactory::keys()
{
    QStringList list;
#ifndef QT_NO_COMPONENT
    if ( !instance )
	instance = new TEFactoryPrivate;

    list = TEFactoryPrivate::manager->featureList();
#endif //QT_NO_COMPONENT

#ifndef TE_NO_DRIVER_XXXX
//    if ( !list.contains( "TEBase" ) ) list << "TEBase";
#endif

    return list;
}
#endif
