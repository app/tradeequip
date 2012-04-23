/****************************************************************************
** $Id: teplugin.cpp,v 1.1 2006/02/14 12:05:25 leader Exp $
**
** Extension plugin class implementation file of
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

#include "teplugin.h"

#ifndef QT_NO_COMPONENT

#include "teinterface_p.h"

/*!
    \class TEPlugin teplugin.h
    \brief The TEPlugin class provides an abstract base for custom TEBase plugins.

    \ingroup plugins
    \mainclass

    The Trade equipment driver plugin is a simple plugin interface that makes it
    easy to create your own Trade equipment drivers plugins that can be loaded
    dynamically by Ananas and Qt.

    Writing a Trade equipment plugin is achieved by subclassing this base class,
    reimplementing the pure virtual functions keys() and create(), and
    exporting the class with the \c TE_EXPORT_PLUGIN macro. See the 
    Ananas extensions plugins that come with Ananas for example 
    implementations (in the
    \c{extensions} subdirectory of the source
    distribution). Read the \link plugins-howto.html plugins
    documentation\endlink for more information on plugins.
*/

/*!
    \fn QStringList TEPlugin::keys() const

    Returns the list of extensions (keys) this plugin supports.

    These keys are usually the class names of the custom extension that
    are implemented in the plugin.

    \sa create()
*/

/*!
    \fn TEBase* TEPlugin::create( const QString& key )

    Creates and returns a TEBase object for the driver key \a key.
    The driver key is usually the class name of the required driver.

    \sa keys()
*/

class TEPluginPrivate : public TEFactoryInterface
{
public:
    TEPluginPrivate( TEPluginBase *p )
	: plugin( p )
    {
    }
    virtual ~TEPluginPrivate();

    QRESULT queryInterface( const QUuid &iid, QUnknownInterface **iface );
    Q_REFCOUNT;

    QStringList featureList() const;
    TEBase *create( const QString &key );

private:
    TEPluginBase *plugin;
};

TEPluginPrivate::~TEPluginPrivate()
{
    delete plugin;
}

QRESULT TEPluginPrivate::queryInterface( const QUuid &iid, QUnknownInterface **iface )
{
    *iface = 0;

    if ( iid == IID_QUnknown )
	*iface = this;
    else if ( iid == IID_QFeatureList )
	*iface = this;
    else if ( iid == IID_TEFactory )
	*iface = this;
    else
	return QE_NOINTERFACE;

    (*iface)->addRef();
    return QS_OK;
}

QStringList TEPluginPrivate::featureList() const
{
    return plugin->keys();
}

TEBase *TEPluginPrivate::create( const QString &key )
{
    return plugin->create( key );
}

/*!
    Constructs a Ananas extension plugin. This is invoked automatically by
    the \c Q_EXPORT_PLUGIN macro.
*/

TEPluginBase::TEPluginBase()
    : QGPlugin( d = new TEPluginPrivate( this ) )
{
}

/*!
    Destroys the Ananas extension plugin.

    You never have to call this explicitly. Qt destroys a plugin
    automatically when it is no longer used.
*/
TEPluginBase::~TEPluginBase()
{
    // don't delete d, as this is deleted by d
}

#endif // QT_NO_COMPONENT
