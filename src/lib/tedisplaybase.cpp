/****************************************************************************
** $Id: tedisplaybase.cpp,v 1.6 2006/07/04 06:07:07 red75 Exp $
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

/* */
#define _TEDISPLAYBASE_CPP
#include "tedisplaybase.h"

/*!
\class SlowWriterThread tedisplaybase.h
\brief writes data to com port with a delay between characters.

 * SlowWriterThread writes data to com port with a delay between characters.
*/
class SlowWriterThread : public QThread
{
	bool m_bQuitRequest;
	bool m_bRunning;
	int m_iIdx;
	int m_iDelay;
	QMutex * m_pBufferMutex;
	QWaitCondition * m_pDataReady;
	QByteArray * m_pBuffer;
	TSerialPort * m_pPort;
public:
	SlowWriterThread(TSerialPort * pPort, QByteArray * pBuffer, QMutex * pBufferMutex, QWaitCondition * pDataReady, int iDelay):
	  QThread(),m_iIdx(0),m_pBufferMutex(pBufferMutex),m_pDataReady(pDataReady)
	{ 
	    m_pPort = pPort; 
	    m_pBuffer = pBuffer;
	    m_iDelay = iDelay;
	    m_bQuitRequest = false;
	    m_bRunning =false;
	};
	void stop()
	{
		m_bQuitRequest=true;
		m_pDataReady->wakeAll();
		wait();
		m_bQuitRequest=false;
	};
	bool isRunning()
	{
		return m_bRunning;
	};
	void setdelay(int iDelay)
	{
		m_iDelay=iDelay;
	};
	void setPort(TSerialPort * pPort)
	{
		m_pBufferMutex->lock();
		m_pPort=pPort;
		m_pBuffer->resize(0);
		m_iIdx=0;
		m_pBufferMutex->unlock();
	};
protected:
	virtual void run()
	{
		m_bRunning=true;
		while(true)
		{
			m_pDataReady->wait();
			char ch;
			while (true)
			{
				if (m_bQuitRequest)
				{
					m_bQuitRequest=false;
					m_bRunning=false;
					return;
				}
				m_pBufferMutex->lock();
				if (m_iIdx<(int)m_pBuffer->size())
				{
					ch=m_pBuffer->at((uint)m_iIdx);
					m_iIdx++;
					m_pPort->putch(ch);
					m_pBufferMutex->unlock();
					if(m_iDelay>0)usleep((unsigned long)m_iDelay);
				}
				else
					break;
			};
			m_pBuffer->resize(0);
			m_iIdx=0;
			m_pBufferMutex->unlock();
		};
	};
};

//=======================================
/*!
    \class TEDisplayBase tedisplaybase.h
    \brief The TEDisplayBase is base class for all customer display classes.

*/

/*!
 * 
 */

TEDisplayBase::TEDisplayBase( int pn )
: TEBase( pn ), m_slowMode(0),m_pBufferMutex(0),m_pDataReady(0),
m_pWriter(0),m_pBuffer(0), m_eCommandSet(TEDisplayBase::CS_DEFAULT)
{
}


/*!
 *
 */
TEDisplayBase::~TEDisplayBase()
{
	if (m_pWriter)
		((SlowWriterThread*)m_pWriter)->stop();
	close();
}

QString TEDisplayBase::modelInfo()
{
	return QString("TEDisplayBase");
}

void TEDisplayBase::initDisplay()
{
}

void TEDisplayBase::setDisplayMode(const QString & /*mode*/)
{
}

void TEDisplayBase::showText(const QString & /*text*/, const QString & /*category*/, 
		const QString & /*mode*/)
{
}

void TEDisplayBase::processEvent( int /*Code*/ )
{
}

/*
QStringList TEDisplayBase::execCommand(const QString & cmd)
{
	QStringList res;
	return res;
}
*/

void TEDisplayBase::setPortNumberAndInit(int pn)
{
	if (m_pWriter)
		((SlowWriterThread*)m_pWriter)->stop();
	setPortNumber(pn);
	initPort();
	if (m_pWriter)
	{
		((SlowWriterThread*)m_pWriter)->setPort(port());
		((SlowWriterThread*)m_pWriter)->start();
	}
};

int TEDisplayBase::portBaudRate()
{
	return (int)m_baudRate;
};

void TEDisplayBase::setPortBaudRate(int br)
{
	BaudRateType baudrate[]={BAUD50, BAUD75, BAUD110, BAUD134, BAUD150, BAUD200, BAUD300, 
		BAUD600, BAUD1200, BAUD1800, BAUD2400, BAUD4800, BAUD9600, BAUD14400, 
		BAUD19200, BAUD38400, BAUD56000, BAUD57600, BAUD76800, BAUD115200, 
		BAUD128000, BAUD256000};
	int intrate[]={50,75,110,134,150,200,300,600,1200,1800,2400,4800,
			9600,14400,19200,38400,56000,57600,76800,115200,128000,256000,0};

	if (port())
	{
		BaudRateType brt=BAUD256000;
		int brnum=0;
		while(intrate[brnum])
		{
			if (br<=intrate[brnum])
			{
				brt=baudrate[brnum];
				break;
			}
			brnum++;
		}
		m_baudRate=(Q_ULONG)intrate[brnum];
		if (m_pBufferMutex) m_pBufferMutex->lock();
		port()->setBaudRate(brt);
		if (m_pBufferMutex) m_pBufferMutex->unlock();

	}
};

Q_ULONG TEDisplayBase::writeBlockSlow( const char *data, Q_ULONG len )
{
	if (m_pWriter==0)
	{
		return writeBlock(data,len);
	}
	else
	{
		if (m_pBuffer->size()<100)
			addToBuffer(data,len);
		return len;
	};
};

void TEDisplayBase::setSlowPortMode(int delayus)
{
	if (m_pWriter==0 && delayus>0)
	{
		m_pBuffer=new QByteArray(0);
		m_pBufferMutex=new QMutex();
		m_pDataReady=new QWaitCondition();
		m_pWriter=new SlowWriterThread(port(),m_pBuffer,m_pBufferMutex,m_pDataReady,delayus);
		m_pWriter->start(QThread::TimeCriticalPriority);
		qWarning("Slow writer thread has been started!!!");
	}
	else if (m_pWriter!=0)
	{
		((SlowWriterThread*)m_pWriter)->setdelay(delayus);
	}
	else
	{ // there's no writer thread and we are being asked to set delay to zero. Do nothing.
	};
};

void TEDisplayBase::addToBuffer(const char * data, Q_ULONG len)
{
	if (m_pBuffer==0)
	{
		qWarning("addToBuffer: Slow mode is not initialized.");
		return;
	}
	m_pBufferMutex->lock();
	uint oldsize=m_pBuffer->size();
	m_pBuffer->resize(oldsize+len);
	for(uint i=oldsize;i<oldsize+len;i++)
		m_pBuffer->at(i)=data[i-oldsize];
	m_pBufferMutex->unlock();
	m_pDataReady->wakeAll();
};

int TEDisplayBase::setCommandSet(int eCommandSet)
{
  if (eCommandSet==CS_DEFAULT)
    return 0;
  setErrorText(tr("Unsupported command set."));
  return 100;
}

QString TEDisplayBase::commandSetName(int eCS)
{
  QString name;
  switch(eCS)
  {
    case TEDisplayBase::CS_AEDEX:
      name=tr("AEDEX");
      break;
    case TEDisplayBase::CS_DEFAULT:
      name=tr("DEFAULT","Default command set for device");
      break;
    case TEDisplayBase::CS_DSP800:
      name=tr("DSP800");
      break;
    case TEDisplayBase::CS_EPSON:
      name=tr("EPSON");
      break;
    case TEDisplayBase::CS_FIRICH:
      name=tr("FIRICH");
      break;
// Insert new command set names here
    default:
      name=tr("Unknown command set #%1").arg(eCS);
      break;
  }
  return name;
}

bool TEDisplayBase::isCommandSetSupported(int eCS)
{
  if (eCS==CS_DEFAULT)
    return true;
  return false;
}
