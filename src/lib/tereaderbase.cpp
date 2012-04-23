/****************************************************************************
** $Id: tereaderbase.cpp,v 1.10 2006/07/18 06:01:19 red75 Exp $
**
** Trade equipment common portable library project
**
** Copyright (C) 1999-2006 Leader InfoTech.  All rights reserved.
** Copyright (C) 1999-2006 Valery Grazdankin.  All rights reserved.
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

#include "tereaderbase.h"


/*!
    \class TEReaderBase tereadebase.h
    \brief The TEReaderBase is base class for all reader classes.

*/

TEReaderBase::TEReaderBase() : TEBase(0)
{
    m_iStopByte=0x0d;
    m_bUseStopByte=true;
    m_bDropStopByte=true;
    m_bClearToChangeBC=true;
    m_iMaxDelay=100;
    m_iBuffering=1;
    m_iMaxLength=128;
    m_iDropBegCnt=0;
    m_iDropEndCnt=0;
    m_qsStartSequence=QString::null;
    m_qsStopSequence=QString::null;
    setPollInterval(200);
}

QString TEReaderBase::text( int LineNumber ) const // it returns first barcode from queue
{
    if (m_qBuffer.empty())
        return QString::null;
    if ( LineNumber == 0 )
	return m_qBuffer.back();
// always return all lines
// will be corrected
    return m_qBuffer.back();
}

int TEReaderBase::next() // it advances to next barcode in queue, 0-there's no barcode available, 1-ok
                       // if there is barcode in buffer, then emit event EVENT_BARCODE.
{
    if (m_iBuffering==0)
    {
        clearBuffer();
        return 0;
    }
    else
    {
        if (!m_qBuffer.isEmpty())
        	m_qBuffer.pop_back();
        if (m_qBuffer.size()>0)
            emitSignal();
        return 1;
    }
    return 0;
}

void TEReaderBase::setBuffering(int iBufEnabled) // params: iBufEnabled: 0 - buffering disabled, 1 - enabled
{
    m_iBuffering=iBufEnabled;
}

int TEReaderBase::buffering() const
{
    return m_iBuffering;
}

int TEReaderBase::clearBuffer() // it clears queue of barcodes. Return value: 0 - there was no pending barcodes, 1 - there was pending barcodes
{
    m_bClearToChangeBC=true;
    int ret=m_qBuffer.empty()?0:1;
    while (!m_qBuffer.empty())
        m_qBuffer.pop_back();
    return ret;
}

int TEReaderBase::pushText(const QString & barcode) // it pushes given barcode to the end of a queue. returns 1 on success, 0 on barcode drop
{
    if (m_iBuffering==0 && m_bClearToChangeBC)
    { // There's no buffering, and previos barcode was received by client
        clearBuffer(); // if buffering is disabled, then ensure that there's only one item in the queue
        m_qBuffer.push_front(barcode);
        m_bClearToChangeBC=false;
        emitSignal();
    }
    else if (m_iBuffering!=0)
    { // There's buffering
        bool bufempty=m_qBuffer.empty();
        m_qBuffer.push_front(barcode);
        if (bufempty)
            emitSignal();
    }
    else if (m_iBuffering==0)
    { // There's no buffering and previous barcode was not processed by client.
      // We should drop barcode.
        return 0;
    }
    return 1;
}

void TEReaderBase::setStopByte(int iStopByte) // sets 'stop byte' - byte value which marks the end of barcode
{
    m_iStopByte=iStopByte;
}

int TEReaderBase::stopByte() const
{
    return m_iStopByte;
}

void TEReaderBase::setMaxDelay(int iMaxDelay) // sets maximum allowed wait time in ms for reading single barcode
{
    m_iMaxDelay=iMaxDelay;
}

int TEReaderBase::maxDelay() const
{
    return m_iMaxDelay;
}

void TEReaderBase::emitSignal()
{
    emit deviceEvent(EVENT_READER_DATAARRIVED);
}

void TEReaderBase::reportError(const QString & err)
{
    setErrorText(err);
    emit deviceEvent(EVENT_READER_ERROR);
}

void TEReaderBase::setErrorText(const QString & err)
{
    m_qsErrorText=err;
}

QString TEReaderBase::errorText() const
{
    return m_qsErrorText;
}

int TEReaderBase::startDriver() // open port, then start polling it
{
    if (!openPort())
    {
        setErrorText(tr("Cannot open port COM%1").arg(portNumber()));
        return CRERROR_COMMERROR;
    }
    startPoll(false);
    return CRERROR_OK;
}

int TEReaderBase::stopDriver() // stop polling, then closes port
{
    stopPoll();
    if (!closePort())
    {
        setErrorText(tr("Cannot close port COM%1. This can be a sign of serious hardware or software problem.").arg(portNumber()));
        return CRERROR_COMMERROR;
    }
    return CRERROR_OK;
}

int TEReaderBase::poll()
{
    return readBarcode();
}

QString substLF(const QString & str)
{
  QString ret=str;
  QString x;
  for(uint i=0;i<ret.length();++i)
  {
    x=ret.mid(i,2);
    if (x=="\\\\")
      ret.replace(i,2,"\\");
    else if (x=="\\r")
      ret.replace(i,2,"\r");
  }
  return ret;
}

/*! sets start sequence, replacing \r with (char)10 */
void TEReaderBase::setStartSequence( const QString &seq )
{
  m_qsStartSequence=substLF(seq);
}

/*! sets stop sequence, replacing \r with (char)10 */
void TEReaderBase::setStopSequence( const QString &seq )
{
  m_qsStopSequence=substLF(seq);
}

/*!
  \brief set Scanner parameter for configurable scanners.
*/
bool
TEReaderBase::setParameter(const QString &, const QString &)
{
	setErrorText(tr("Generic reader driver doesn't support configuration parameters."));
	return false;
}



/*!
  \brief handle next data string getted from reader.
*/
void
TEReaderBase::handleData( const QString &data )
{
  Q_UNUSED(data);
}

