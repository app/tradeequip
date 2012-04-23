/****************************************************************************
** $Id: tedisplaybase.h,v 1.3 2006/06/20 06:55:18 red75 Exp $
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

#ifndef _TEDISPLAYBASE_H
#define _TEDISPLAYBASE_H
#include <tebase.h>
#include <qstringlist.h>
#include <qthread.h>
#include <qmutex.h>
#include <qwaitcondition.h>

//---------------------------------------------------------------------------
class LIB_EXPORT TEDisplayBase : public TEBase
{
	Q_OBJECT
	
public:
	TEDisplayBase( int pn );
	~TEDisplayBase();
 
  enum CommandSet
  {
    CS_DEFAULT=0,
    CS_EPSON,
    CS_DSP800,
    CS_AEDEX,
    CS_FIRICH
  };
  virtual QString commandSetName(int);
  virtual bool isCommandSetSupported(int);

	virtual QString modelInfo();
	virtual int scrollSpeed()=0;
	virtual int portBaudRate();
  int getCommandSet() const {return m_eCommandSet;}
public slots:
	virtual void setPortBaudRate(int);
	virtual void setScrollSpeed(int)=0;
  virtual int setCommandSet(int);
	virtual void initDisplay();
	virtual void setPortNumberAndInit(int pn);
	virtual void setDisplayMode(const QString & mode);
	virtual void showText(const QString & text, 
		const QString & category="default", 
		const QString & mode="scroll/left");
	virtual void processEvent( int Code );
//	virtual QStringList execCommand(const QString & cmd);

protected:
	Q_ULONG m_baudRate;
	int m_slowMode;
	void setSlowPortMode(int delayus);
	Q_ULONG writeBlockSlow( const char *data, Q_ULONG len );
	void addToBuffer(const char * data, Q_ULONG len);

	QMutex * m_pBufferMutex;
	QWaitCondition * m_pDataReady;
	QThread * m_pWriter;
	QByteArray * m_pBuffer;
  int m_eCommandSet;

	virtual void initPort()=0; 
};
//---------------------------------------------------------------------------

#endif


