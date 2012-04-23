/****************************************************************************
** $Id: teconnect.h,v 1.1 2006/07/12 09:12:32 leader Exp $
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
   Abstract trade equipment data provider object
*/
#ifndef TECONNECT_H
#define TECONNECT_H

#include <teglobal.h>
#include <templexports.h>

//#include <qobject.h>
#include <qdict.h>
#include <tserialport.h>
#include <cmdparser.h>

//---------------------------------------------------------------------------
class LIB_EXPORT TEConnect : public QObject, public TE
{
    Q_OBJECT
    Q_PROPERTY( QString ActivationKey READ activationKey WRITE setActivationKey )
    Q_PROPERTY( int PortNumber READ portNumber WRITE setPortNumber )
    Q_PROPERTY( int PortBaudRate READ portBaudRate WRITE setPortBaudRate )

public:

    TEConnect( int pn );
    ~TEConnect();

    // Interface functions
    virtual void setActivationKey( const QString &key );
    virtual QString activationKey() const;

    int connectionType();

    virtual int setPortNumber( int pn );
    int portNumber() const;
    void setPollInterval( int pint );
    int pollInterval();

    virtual int isOpen();
    int timeout();
    bool isTimeout();
    int debugLevel();
    void setDebugLevel( int level );

    virtual QCString utf8ToDevice( const QString &text );
    virtual QString deviceToUtf8( const QCString &text );
    virtual int isActivationKeyValid();
    QString productSuffix() const
    {
      return m_sProductSuffix;
    }
    void setProductSuffix(const QString & sProductSuffix)
    {
      m_sProductSuffix=sProductSuffix;
    }

    void setPortBaudRate(int);
    int portBaudRate() const {return m_iPBaudRate;}

    void setPortFlowControl(FlowType);
    FlowType portFlowControl() const {return m_PFlow;}

    void setPortParity(ParityType);
    ParityType portParity() const {return m_PParity;}

    void setPortDataBits(DataBitsType);
    DataBitsType portDataBits() const {return m_PDataBits;}

    void setPortStopBits(StopBitsType);
    StopBitsType portStopBits() const {return m_PStopBits;}


    virtual QValueList<int> supportedBaudRates(); // Non const, just in case.

    virtual int setConnectionType( int ct );

  virtual int open();
  virtual int close();
public slots:
	virtual void processEvent( int Code );
	virtual QStringList execCommand( const QString &cmd );
	virtual QString errorText() const {return m_sError;}

signals:
	void deviceEvent( int Code );

protected:
// Polling control functions
  void startPoll(  int PollInterval = 500, bool defaultProc = true );
  void startPoll(  bool defaultProc = true );
  void stopPoll();
  void startIOTransaction();
  void stopIOTransaction();
  virtual int poll();

// serial port control functions
  TSerialPort *port();
  virtual int putch( int b );
  virtual int getch();
  virtual Q_ULONG writeBlock( const char *data, Q_ULONG len );
  virtual Q_ULONG readBlock( char *data, Q_ULONG maxlen = 1 );
  virtual Q_ULONG readCount();
  void setTimeout( int timeout );
  void clearPPP(); // clears persistent port parameters

  virtual void setErrorText(const QString & err){m_sError=err;}
	void timerEvent( QTimerEvent * );
	ExecEngine m_ee;
private:
    int vPortNumber, vDebugLevel, vTimeout, vPollInterval;
    bool vTimeoutFlag;
    int m_eConnectionType;
    TSerialPort *Port;
    QString vActivationKey;
    QString m_sProductSuffix; // product suffix such as 'drv','1cdrv','srv', etc.

    int m_iPBaudRate;
    FlowType m_PFlow;
    ParityType m_PParity;
    DataBitsType m_PDataBits;
    StopBitsType m_PStopBits;
    QString m_sError; // error description
//    static QDict <TSerialPort> PortsList;
};
//---------------------------------------------------------------------------

#endif
