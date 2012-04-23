/****************************************************************************
** $Id: tserialport.cpp,v 1.6 2006/06/21 12:04:17 red75 Exp $
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
#include "tserialport.h"

#ifdef Q_OS_LINUX
	#include <linux/serial.h>
#endif
#ifdef Q_OS_WIN32

#endif

/*!
    \class TSerialPort tserialport.h
    \brief The TSerialPort class for work with serial ports from TEBase classes.

*/

QStringList TSerialPort::m_ports;

TSerialPort::TSerialPort(const QString &portName, const PortSettings *settings) : 
	QextSerialPort("")
{
	vBaudRate = 0;
	setupPort(portName, settings);
}

TSerialPort::~TSerialPort()
{
}

const QStringList& TSerialPort::generatePortList()
{
#ifdef	_TTY_WIN_
		#define _START_CHAR_	1
		#define _END_CHAR_		256
#elif defined	_TTY_IRIX_
		#define _START_CHAR_	1
		#define _END_CHAR_		256
#elif defined	_TTY_HPUX_
		#define _START_CHAR_	1
		#define _END_CHAR_		256
#elif defined	_TTY_SUN_
		#define _START_CHAR_	'a'
		#define _END_CHAR_		'z'
#elif defined	_TTY_DIGITAL_
		#define _START_CHAR_	1
		#define _END_CHAR_		99
#else
		#define _START_CHAR_	0
		#define _END_CHAR_		255
#endif

	m_ports.clear();

	int i;
	QString s;
	QextSerialPort port;

	for (i = _START_CHAR_; i <= _END_CHAR_; i++) {
    s=portName(i);
		port.setName(s);
		if (port.open()) { // ok!
			port.close();
			m_ports << s;
		}
	}
	return m_ports;
}

QValueList<int> TSerialPort::generatePortNumberList()
{
	int i;
	QValueList<int> ports;
	QextSerialPort port;

	for (i = _START_CHAR_; i <= _END_CHAR_; i++) {
		port.setName(portName(i));
		if (port.open()) { // ok!
			port.close();
			ports << i;
		}
	}
	return ports;
}

QString TSerialPort::portName(int iPortNum)
{
#ifdef	_TTY_WIN_
		#define _START_CHAR_	1
		#define _END_CHAR_		256
		#define _PREFIX_		"COM"
		#define _SUFFIX_		""

#elif defined	_TTY_IRIX_
		#define _START_CHAR_	1
		#define _END_CHAR_		256
		#define _PREFIX_		"/dev/ttyf"
		#define _SUFFIX_		""

#elif defined	_TTY_HPUX_
		#define _START_CHAR_	1
		#define _END_CHAR_		256
		#define _PREFIX_		"/dev/tty"
		#define _SUFFIX_		"p0"

#elif defined	_TTY_SUN_
		#define _START_CHAR_	'a'
		#define _END_CHAR_		'z'
		#define _PREFIX_		"/dev/tty"
		#define _SUFFIX_		""

#elif defined	_TTY_DIGITAL_
		#define _START_CHAR_	1
		#define _END_CHAR_		99
		#define _PREFIX_		"/dev/tty"
		#define _SUFFIX_		""

#else
		#define _START_CHAR_	0
		#define _END_CHAR_		255
		#define _PREFIX_		"/dev/ttyS"
		#define _SUFFIX_		""
#endif

  int i=iPortNum;
  QString s;
	#ifdef _TTY_SUN_
		s = QString(_PREFIX_) + QChar((char)i);
	#elif defined _TTY_DIGITAL_
		s = QString(_PREFIX_) + (i < 10 ? QString("0") : QString::null) + QString::number(i);
	#else
		s = QString(_PREFIX_) + QString::number(i);
	#endif
  return s;
}


bool TSerialPort::setupPort(const QString &portName, const PortSettings *settings)
{
	// close current port
	if (isOpen()) close();

	// change name
	if (!portName.isEmpty())
		setName(portName.ascii());
	// change settings
	if (settings) {
		setBaudRate(settings->BaudRate);
		setFlowControl(settings->FlowControl);
		setDataBits(settings->DataBits);
		setParity(settings->Parity);
		setStopBits(settings->StopBits);
	}

	//return open();
	return 1;
}

void TSerialPort::getPortSettings(QString *portName, PortSettings *settings)
{
		if (portName)
			portName->setAscii(name());

		if (settings) {
			settings->BaudRate = baudRate();
			settings->FlowControl = flowControl();
			settings->DataBits = dataBits();
			settings->Parity = parity();
			settings->StopBits = stopBits();
		}
}


Q_ULONG 
TSerialPort::customBaudRate()
{
    return vBaudRate;
}

void 
TSerialPort::setCustomBaudRate( Q_ULONG baudrate )
{
#ifdef Q_OS_LINUX
#ifdef NOQFILE
    int fdFile=m_fdFile;
#else
    int fdFile=Posix_File->handle();
#endif
	struct termios stty;
        struct serial_struct serinfo;
//       	if (ioctl(Posix_File->handle(), TIOCGSERIAL, &serinfo) < 0) return;
       	ioctl(fdFile, TIOCGSERIAL, &serinfo);
       	serinfo.baud_base = 115200;
       	serinfo.custom_divisor = 115200/baudrate;
       	serinfo.flags = (serinfo.flags & ~ASYNC_SPD_MASK) | ASYNC_SPD_CUST;
       	if (ioctl(fdFile, TIOCSSERIAL, &serinfo) < 0) return;
	tcgetattr(fdFile, &stty);
	cfsetospeed(&stty, B38400);
	cfsetispeed(&stty, B38400);
	tcsetattr(fdFile, TCSANOW, &stty);
#endif
#ifdef _TTY_WIN_
//        DCB   dcb;
	int ok = false;
/*   ok=GetCommState(p->hCom, &dcb);
   if (!ok) {

       //printf("err GetCommState\n");

       GetLastError();

   }
*/
//	if ( GetCommState( Win_Handle, &dcb) ){
	//    GetCommState( Win_Handle, &dcb); // there's no need in reading setting, they are read already.
	    Win_CommConfig.dcb.BaudRate=baudrate;
	    ok=SetCommState( Win_Handle, &Win_CommConfig.dcb );
//	};
	if (!ok) GetLastError();
#endif
//	printf("baudrate=%li\n", baudrate );
	vBaudRate = baudrate;
	Settings.BaudRate=BAUD38400;
}


bool 
TSerialPort::open(int mode) {
    bool res=superclass::open(mode);
    if (!res)
	return res;
    if (vBaudRate!=0)
        setCustomBaudRate(vBaudRate);
    return res;
}


void TSerialPort::setBaudRate(BaudRateType brt)
{
    vBaudRate=0;
    superclass::setBaudRate(brt);
}
