/****************************************************************************
** $Id: tereaderbase.h,v 1.12 2006/07/18 06:01:19 red75 Exp $
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

#ifndef TEREADERBASE_H
#define TEREADERBASE_H

#include <tebase.h>
#include <qvaluelist.h>
#include <qstring.h>
#include "templexports.h"

#define EVENT_READER_DATAARRIVED 1
#define EVENT_READER_ERROR 100


class LIB_EXPORT TEReaderBase : public TEBase
{
    Q_OBJECT
    typedef TEBase superclass;

public:
	// public interface
	TEReaderBase();

public slots:
	virtual QString text( int iTrackNumber = 0 ) const; 	// it returns first text data from queue.
						// iTrackNumber = 0 - returns all tracks
						// iTrackNumber = 1..max - returns iTrackNumber track text.
	int next();	// it advances to next barcode in queue, 0-there's no barcode available, 1-ok
			// if there is barcode in buffer, then emit event EVENT_BARCODE.
	void setBuffering(int iBufEnabled); // params: iBufEnabled: 0 - buffering disabled, 1 - enabled
	int buffering() const;

	void setDropStopByte(bool bDSB) {m_bDropStopByte=bDSB;}
	bool dropStopByte() const {return m_bDropStopByte;}

	void setUseStopByte(bool bUSB) {m_bUseStopByte=bUSB;}
	bool useStopByte() const {return m_bUseStopByte;}

	void setMaxLength(int iML) {m_iMaxLength=iML;}
	int maxLength() const {return m_iMaxLength;}

	void setDropBegCnt(int iDBC) {m_iDropBegCnt=iDBC;}
	int dropBegCnt() const {return m_iDropBegCnt;}

	void setDropEndCnt(int iDEC) {m_iDropEndCnt=iDEC;}
	int dropEndCnt() const {return m_iDropEndCnt;}

	int clearBuffer(); // it clears queue of barcodes. Return value: 0 - there was no pending barcodes, 1 - there was pending barcodes

	void setStopByte(int); // sets 'stop byte' - byte value which marks the end of barcode
	int stopByte() const;

	void setMaxDelay(int); // sets maximum allowed wait time in ms for reading single barcode
	int maxDelay() const;

	void setStartSequence( const QString &seq );
	QString startSequence() const {return m_qsStartSequence;}

	void setStopSequence( const QString &seq );
	QString stopSequence() const {return m_qsStopSequence;}

	QString errorText() const;

	virtual int startDriver(); // open port, then start polling it
	virtual int stopDriver(); // stop polling, then closes port

	virtual bool setParameter(const QString &, const QString &); //!< set Scanner parameter for configurable scanners

	int queueSize() const
	{
		return (int)m_qBuffer.size();
	}

  enum ReaderType
  {
    RT_BARCODE,
    RT_CARD
  };

  //!< returns reader type: RT_BARCODE - barcode , RT_CARD - cardreader
  virtual int readerType() const=0; 

protected:
// Interface for descendant classes

	int pushText(const QString &);   // It pushes given barcode to the end of a queue,
                                        // emits event immediatly, or drops barcode.
                                        // Action depends on current driver settings.

	virtual int openPort()=0; // opens port
	virtual int closePort()=0; // closes port
	virtual int readBarcode()=0; // tries to read barcode. It is called by poll()

	virtual void emitSignal(); // emits EVENT_BARCODE_ARRIVED device event, can be replaced by miniport driver
	virtual void reportError(const QString &); // emits EVENT_ERROR device event

	void setErrorText(const QString & err);

	virtual void handleData( const QString &data );

private:
	virtual int poll(); // this function gets called every nth millisecond

	QString m_qsReadBuffer;

	QValueList <QString> m_qBuffer;
	bool m_bClearToChangeBC; // it's clear to change value returned by barcode()
	bool m_bUseStopByte;
	bool m_bDropStopByte;
	int m_iStopByte; // stop byte value
	int m_iMaxDelay;
	int m_iBuffering; // 0-disable buffering, 1-enable buffering
	int m_iMaxLength; // maximal barcode length
	int m_iDropBegCnt; // number of starting characters to drop
	int m_iDropEndCnt; // number of ending characters to drop
	QString m_qsErrorText;
	QString m_qsStartSequence, m_qsStopSequence;
};

#endif
