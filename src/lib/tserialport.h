/****************************************************************************
** $Id: tserialport.h,v 1.5 2006/06/21 12:04:17 red75 Exp $
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

	TSerialPort v.1.1b

	///////////////////////////////////////////////

	Multiplatform Serial Port Extension
	based on Wayne Roth's QExtSerialPort

	(c) Racheengel 2004-2005

*/

#ifndef _TSERIALPORT_H_
#define _TSERIALPORT_H_

#include <qobject.h>
#include <qvaluelist.h>
#include <qstringlist.h>
#include <qcstring.h>
#include <qextserialport.h>
#include <teglobal.h>

#ifdef _WINDOWS
#define _TTY_WIN_
#endif
#ifdef WIN32
#define _TTY_WIN_
#endif

class LIB_EXPORT TSerialPort : public QextSerialPort
{
    typedef QextSerialPort superclass;
public:
	TSerialPort(const QString &portName = QString::null, const PortSettings *settings = 0);
	virtual ~TSerialPort();

	bool	setupPort(const QString &portName = QString::null, const PortSettings *settings = 0);
	void	getPortSettings(QString *portName = 0, PortSettings *settings = 0);

//	virtual bool open(int mode=0);
	Q_ULONG customBaudRate();
	void setCustomBaudRate( Q_ULONG baudrate );
	
	virtual void setBaudRate(BaudRateType);
	
	virtual bool open(int mode=0);
	
	static const QStringList&	portList() { return m_ports; }
	static const QStringList&	generatePortList();
  static QString portName(int iPortNum);
  static QValueList<int> generatePortNumberList();

protected:
	static QStringList m_ports;
	Q_ULONG vBaudRate;
};

#endif
