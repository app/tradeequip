/****************************************************************************
** $Id: teconnect.cpp,v 1.1 2006/07/12 09:12:32 leader Exp $
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
    Abstract trade equipment object.
   (c) V.Grazdankin, 1999-2005
*/
#ifndef TECONNECT_CPP
#define TECONNECT_CPP
#include "teconnect.h"
#include <tserialport.h>
#include <stdarg.h>

/*!
    \class TEConnect teconnect.h
    \brief The TEConnect is base class to connect with trade equipment.

    Abstract trade equipment connection object.
    Object have internal text command interpreter
    that give access to object functions.
    \param cmd (in) command text. Command format: <function> '(' <parm1> ',' <parm2> ',' ... <parmN> ')'
    \return - command execution result.
*/


/*!
 * Abstract trade equipment object.
 * Object have internal text command interpreter
 * that give access to object functions.
 * \param cmd (in) command text. Command format: <function> '(' <parm1> ',' <parm2> ',' ... <parmN> ')'
 * \return - command execution result.
 */
TEConnect::TEConnect( int pn )
{
    m_ee.addFuncBinding1<TEConnect,int,int>(
        this, &TEConnect::setPortNumber, "setPortNumber");
    m_ee.addFuncBinding0<TEConnect,int, ConstFunc0<TEConnect,int> >(
        this, &TEConnect::portNumber, "portNumber");
    m_ee.addProcBinding1<TEConnect,int>(
        this,&TEConnect::setPollInterval,"setPollInterval");
    m_ee.addFuncBinding0<TEConnect,int>(
        this,&TEConnect::pollInterval,"pollInterval");
    m_ee.addFuncBinding0<TEConnect,QString, ConstFunc0<TEConnect,QString> >(this,&TEConnect::activationKey,"activationKey");
//    addFuncBinding(m_ee,this,&TEConnect::activationKey,"activationKey");
    m_ee.addProcBinding1<TEConnect,const QString &>(
        this,&TEConnect::setActivationKey,"setActivationKey");
    m_ee.addFuncBinding0<TEConnect,int>(
        this,&TEConnect::isActivationKeyValid,"isActivationKeyValid");
    m_ee.addFuncBinding0<TEConnect,int>(
        this,&TEConnect::open,"open");
    m_ee.addFuncBinding0<TEConnect,int>(
        this,&TEConnect::close,"close");
    m_ee.addProcBinding1<TEConnect,int>(
        this,&TEConnect::setDebugLevel,"setDebugLevel");
    m_ee.addProcBinding1<TEConnect,bool>(
        this,&TEConnect::startPoll,"startPoll");
    m_ee.addProcBinding0<TEConnect>(
        this,&TEConnect::stopPoll,"stopPoll");

    clearPPP();

    vPortNumber = pn;
    vDebugLevel = 0;
    vTimeout = 150;
    vTimeoutFlag = false;
    Port = new TSerialPort();
}

/*!
*/
TEConnect::~TEConnect()
{
//    close();
    delete Port;
}


int
TEConnect::setConnectionType( int ct )
{
	switch (ct) {
	case ECT_SERIAL:
	  m_eConnectionType = ct;
		break;
	case ECT_KEYBOARD:
	  m_eConnectionType = ct;
		break;
	default:
		setErrorText(tr("Unknown connection type."));
		return 1;
		break;
	}
	return 0;
}


int
TEConnect::connectionType()
{
	return m_eConnectionType;
}


int
TEConnect::setPortNumber( int pn )
{
    bool o = isOpen();
    if ( o ) o = close();
    vPortNumber = pn;
    if ( o ) o = open();
    return o;
}



int
TEConnect::portNumber() const
{
    return vPortNumber;
}



int
TEConnect::open()
{
	QString pname;
#ifdef Q_OS_WIN32
    pname=QString("COM%1:").arg( portNumber() );
#else
    pname=QString("/dev/ttyS%1").arg( portNumber() - 1);
#endif
    Port->setName( pname );
    if (!Port->open())
	{
		setErrorText(tr("Cannot open serial port '%1'.").arg(pname));
		return 0;
	}
    if (m_iPBaudRate!=0)
        Port->setCustomBaudRate((Q_ULONG)m_iPBaudRate);
    if (m_PFlow!=FLOW_NOTSET)
        Port->setFlowControl(m_PFlow);
    if (m_PParity!=PAR_NOTSET)
        Port->setParity(m_PParity);
    if (m_PDataBits!=DATA_NOTSET)
        Port->setDataBits(m_PDataBits);
    if (m_PStopBits!=STOP_NOTSET)
        Port->setStopBits(m_PStopBits);
   	Port->setTimeout(0,(unsigned long)vTimeout);
    return 1;
}



int
TEConnect::close()
{
    if (Port->isOpen())
        Port->close();
    return 1;
}


/*!
Check comunication port connection.
*/
int
TEConnect::isOpen()
{
    return Port->isOpen();
}


int
TEConnect::timeout()
{
    return vTimeout;
}

void
TEConnect::setTimeout( int timeout )
{
    vTimeout = timeout;
    port()->setTimeout(0,(unsigned long)timeout);
}



int
TEConnect::pollInterval()
{
    return vPollInterval;
}



void
TEConnect::setPollInterval( int pint )
{
    vPollInterval = pint;
}


bool
TEConnect::isTimeout()
{
    return vTimeoutFlag;
}


int
TEConnect::debugLevel()
{
    return vDebugLevel;
}

void
TEConnect::setDebugLevel( int level )
{
    vDebugLevel = level;
}


/*!
Put character
*/
int
TEConnect::putch( int b )
{
    int res=Port->putch( b );
#ifdef DEBUG
    if ( vDebugLevel ){
	    printf(">%02X ", b & 0xff );
    	    fflush(stdout);
	}
#endif
    return res;
}


/*!
Get character.
*/
int
TEConnect::getch()
{
//    int t = vTimeout;
    int i;

//    vTimeoutFlag = true;
//    while ( readCount()<=0 && (t--)>0) Sleep(1);
//    if (readCount()>0)
//    {
        i = (Port->getch());
        vTimeoutFlag = i==-1;
//    }
//    else
//        i=-1;
#ifdef DEBUG
    if ( vDebugLevel )
    {
	if (i==-1) printf("<XX "); printf("<%02X ", i & 0xff );
	fflush(stdout);
    }
#endif
    return i;
}


/*!
Write data block to device.
*/
Q_ULONG
TEConnect::writeBlock( const char *data, Q_ULONG len )
{
    Q_ULONG i;
#ifdef DEBUG
    if ( vDebugLevel ) printf("\n-");
#endif
    for (i=0;i<len; i++) {
        putch( data[i] );
    }
    return i;
}


/*!
Read data block from device.
*/
Q_ULONG
TEConnect::readBlock( char *data, Q_ULONG maxlen )
{
    Q_ULONG offs=0;
    int b = -1;
#ifdef DEBUG
    if ( vDebugLevel ) printf("\n-");
#endif
//  if ( readCount()< maxlen )
    while (offs< maxlen ) {
        b = getch() & 0xff;
        if ( isTimeout() ) break;
        //  if ( b<0 ) break;
        data[offs] = (char)b; // & 0xff;
        offs++;
    }
    return offs;
}


Q_ULONG
TEConnect::readCount()
{
    int c = Port->bytesWaiting();
    if ( c < 0 ) c = 0;
    return (Q_ULONG)c;
}


/*!
.
*/
void
TEConnect::startPoll(  int PollInterval, bool defaultProc )
{
    if ( defaultProc ) connect( this, SIGNAL( deviceEvent( int ) ), SLOT( processEvent( int ) ) );
    startTimer( PollInterval );
}




/*!
.
*/
void
TEConnect::startPoll( bool defaultProc )
{
    startPoll( pollInterval(), defaultProc );
}



/*!
.
*/
void
TEConnect::stopPoll()
{
    killTimers();
}


/*!
*/
void
TEConnect::processEvent( int )
{
}


/*!
*/
int
TEConnect::poll()
{
    return 0;
}


/*!
Poll device on timer event.
*/
void
TEConnect::timerEvent( QTimerEvent * )
{
    poll();
}


/*!
Return pointer to device serial port object.
*/
TSerialPort *
TEConnect::port()
{
    return Port;
}


/*!
 * Execute command.
 * Object have internal text command interpreter
 * that give access to object functions.
 * \param cmd (in) command text. Command format: <function> '(' <parm1> ',' <parm2> ',' ... <parmN> ')'
 * \return - command execution result.
 */
QStringList
TEConnect::execCommand( const QString &cmd )
{
//    Q_UNUSED( cmd )
    QStringList l;
    int res=m_ee.execute(cmd);
    if (res)
        l << QString("999;\"")+m_ee.error()+QString("\"");
    else
    {
        l << "0;\"Ok\"";
        l << m_ee.result();
    };
    return l;
}


QCString
TEConnect::utf8ToDevice( const QString &text )
{
    return ( const char *) text;
}

QString
TEConnect::deviceToUtf8( const QCString &text )
{
    return text;
}


void
TEConnect::startIOTransaction()
{
}


void
TEConnect::stopIOTransaction()
{
}

QString
TEConnect::activationKey() const
{
    return vActivationKey;
}

void
TEConnect::setActivationKey( const QString &key )
{
    vActivationKey = key;
    isActivationKeyValid();
}

int
TEConnect::isActivationKeyValid()
{
    return 1;
}

void TEConnect::setPortBaudRate(int iBaudRate)
{
    m_iPBaudRate=iBaudRate;
    if (m_iPBaudRate!=0 && Port->isOpen())
        Port->setCustomBaudRate((Q_ULONG)m_iPBaudRate);
}

void TEConnect::setPortFlowControl(FlowType PFlow)
{
    m_PFlow=PFlow;
    if (m_PFlow!=FLOW_NOTSET && Port->isOpen())
        Port->setFlowControl(m_PFlow);
}

void TEConnect::setPortParity(ParityType PPar)
{
    m_PParity=PPar;
    if (m_PParity!=PAR_NOTSET && Port->isOpen())
        Port->setParity(m_PParity);
}

void TEConnect::setPortDataBits(DataBitsType PDB)
{
    m_PDataBits=PDB;
    if (m_PDataBits!=DATA_NOTSET && Port->isOpen())
        Port->setDataBits(m_PDataBits);
}

void TEConnect::setPortStopBits(StopBitsType PSB)
{
    m_PStopBits=PSB;
    if (m_PStopBits!=STOP_NOTSET && Port->isOpen())
        Port->setStopBits(m_PStopBits);
}

void TEConnect::clearPPP()
{
    m_iPBaudRate=0;
    m_PFlow=FLOW_NOTSET;
    m_PParity=PAR_NOTSET;
    m_PDataBits=DATA_NOTSET;
    m_PStopBits=STOP_NOTSET;
}

QValueList<int> TEConnect::supportedBaudRates()
{
  QValueList<int> ret;
  
  return ret;
}

#endif // TECONNECT_CPP
