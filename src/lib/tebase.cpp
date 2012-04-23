/****************************************************************************
** $Id: tebase.cpp,v 1.16 2006/12/03 16:23:13 leader Exp $
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
#ifndef TEBASE_CPP
#define TEBASE_CPP
#include "tebase.h"
#include <tserialport.h>
#include <stdarg.h>

/*!
    \class TEBase tebase.h
/en
    \brief The TEBase is base class for all trade equipment classes.

    Abstract trade equipment object.
    Object have internal text command interpreter
    that give access to object functions.
    \param cmd (in) command text. Command format: <function> '(' <parm1> ',' <parm2> ',' ... <parmN> ')'
    \return - command execution result.
/_en
/ru
    \brief TEBase - базовый класс для всех классов торгового оборудования.

   Работа с торговым оборудованием состоит из нескольких шагов:
 * 1. Создание объекта для работы с соответствующим оборудованием, например:
 * 	TEReaderBarcode *barcodereader = new TEReaderBarcode();
 * 2. Установка параметров подключения оборудования.
 * 3. Соединение сигнала deviceEvent( int Event ) объекта работы с
 * 	оборудованием с обработчиком события от устройства.
 * 	Например, при считывании штрихкода генерируется сигнал
 * 	EVENT_READER_DATAARRIVED.
 * 	Если ваш обработчик подключен к сигналам от нескольких объектов,
 * 	необходимо проверять базовый класс объекта, пославшего событие
 * 	до того, как проверять и обрабатывать код события, так как
 * 	коды событий для разных классов могут совпадать.
 * 	Например:
 * 		if ( sender()->inherits("TEReaderBase") ){
 * 		// Проверяем события от считывателя
 * 			switch ( Event ) {
 * 			case EVENT_READER_DATAARRIVED: // обрабатываем
 * 				break;
 * 			...
 * 			}
 * 		}
 * 4. Старт работы драйвера:
 * 	barcodereader->startDriver();
 * 5. Работа с оборудованием.
 * 	Работа осуществляется вызовом методов объекта,
 * 	обработкой событий от устройства.
 * 6. Остановка драйвера:
 * 	barcodereader->stopDriver();
 * 7. Уничтожение обекта.
/_ru
*/


/*!
 * Abstract trade equipment object.
 * Object have internal text command interpreter
 * that give access to object functions.
 * \param cmd (in) command text. Command format: <function> '(' <parm1> ',' <parm2> ',' ... <parmN> ')'
 * \return - command execution result.
 */
TEBase::TEBase( int pn )
{
    m_ee.addFuncBinding1<TEBase,int,int>(
        this, &TEBase::setPortNumber, "setPortNumber");
    m_ee.addFuncBinding0<TEBase,int, ConstFunc0<TEBase,int> >(
        this, &TEBase::portNumber, "portNumber");
    m_ee.addProcBinding1<TEBase,int>(
        this,&TEBase::setPollInterval,"setPollInterval");
    m_ee.addFuncBinding0<TEBase,int>(
        this,&TEBase::pollInterval,"pollInterval");
    m_ee.addFuncBinding0<TEBase,QString, ConstFunc0<TEBase,QString> >(this,&TEBase::activationKey,"activationKey");
//    addFuncBinding(m_ee,this,&TEBase::activationKey,"activationKey");
    m_ee.addProcBinding1<TEBase,const QString &>(
        this,&TEBase::setActivationKey,"setActivationKey");
    m_ee.addFuncBinding0<TEBase,int>(
        this,&TEBase::isActivationKeyValid,"isActivationKeyValid");
    m_ee.addFuncBinding0<TEBase,int>(
        this,&TEBase::open,"open");
    m_ee.addFuncBinding0<TEBase,int>(
        this,&TEBase::close,"close");
    m_ee.addProcBinding1<TEBase,int>(
        this,&TEBase::setDebugLevel,"setDebugLevel");
    m_ee.addProcBinding1<TEBase,bool>(
        this,&TEBase::startPoll,"startPoll");
    m_ee.addProcBinding0<TEBase>(
        this,&TEBase::stopPoll,"stopPoll");

    m_bDriverStarted=false;
    clearPPP();

    vPortNumber = pn;
    vDebugLevel = 0;
    vTimeout = 150;
    vTimeoutFlag = false;
    Port = new TSerialPort();
}

/*!
*/
TEBase::~TEBase()
{
//    close();
    delete Port;
}



/*! \fn int TEBase::errorText()
/ru Возвращает описание ошибки, соответствующее последнему коду ошибки.
 /_ru
/en
 /_en
 */
QString
TEBase::errorText() const
{

	return m_sError;
	switch ( errorCode() ){
	case CRERROR_OK:
		return tr("NO ERROR");
		break;
	case CRERROR_NOTSUPPORTED:
		return tr("This functions isn't supported by driver or hardware.");
		break;
	default:
		return tr("UNKNOWN ERROR");
		break;
	}
	return "";
}



/*! \fn int TEBase::setConnectionType()
/ru Устанавливает тип подключения торгового оборудования.
 /_ru
/en
 /_en
 */
int
TEBase::setConnectionType( int ct )
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
TEBase::connectionType()
{
	return m_eConnectionType;
}


int
TEBase::setPortNumber( int pn )
{
    bool o = isOpen();
    if ( o ) o = close();
    vPortNumber = pn;
    if ( o ) o = open();
    return o;
}



int
TEBase::portNumber() const
{
    return vPortNumber;
}



int
TEBase::open()
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
TEBase::close()
{
    if (Port->isOpen())
        Port->close();
    return 1;
}


/*!
Check comunication port connection.
*/
int
TEBase::isOpen()
{
    return Port->isOpen();
}


int
TEBase::timeout()
{
    return vTimeout;
}

void
TEBase::setTimeout( int timeout )
{
    vTimeout = timeout;
    port()->setTimeout(0,(unsigned long)timeout);
}



int
TEBase::pollInterval()
{
    return vPollInterval;
}



void
TEBase::setPollInterval( int pint )
{
    vPollInterval = pint;
}


bool
TEBase::isTimeout()
{
    return vTimeoutFlag;
}


int
TEBase::debugLevel()
{
    return vDebugLevel;
}

void
TEBase::setDebugLevel( int level )
{
    vDebugLevel = level;
}


/*!
Put character
*/
int
TEBase::putch( int b )
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
TEBase::getch()
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
TEBase::writeBlock( const char *data, Q_ULONG len )
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
TEBase::readBlock( char *data, Q_ULONG maxlen )
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
TEBase::readCount()
{
    int c = Port->bytesWaiting();
    if ( c < 0 ) c = 0;
    return (Q_ULONG)c;
}


/*!
.
*/
void
TEBase::startPoll(  int PollInterval, bool defaultProc )
{
    if ( defaultProc ) connect( this, SIGNAL( deviceEvent( int ) ), SLOT( processEvent( int ) ) );
    startTimer( PollInterval );
}




/*!
.
*/
void
TEBase::startPoll( bool defaultProc )
{
    startPoll( pollInterval(), defaultProc );
}



/*!
.
*/
void
TEBase::stopPoll()
{
    killTimers();
}


/*!
*/
void
TEBase::processEvent( int )
{
}


/*!
*/
int
TEBase::poll()
{
    return 0;
}


/*!
Poll device on timer event.
*/
void
TEBase::timerEvent( QTimerEvent * )
{
    poll();
}


/*!
Return pointer to device serial port object.
*/
TSerialPort *
TEBase::port()
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
TEBase::execCommand( const QString &cmd )
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
TEBase::utf8ToDevice( const QString &text )
{
    return ( const char *) text;
}

QString
TEBase::deviceToUtf8( const QCString &text )
{
    return text;
}


void
TEBase::startIOTransaction()
{
}


void
TEBase::stopIOTransaction()
{
}

QString
TEBase::activationKey() const
{
    return vActivationKey;
}

void
TEBase::setActivationKey( const QString &key )
{
    vActivationKey = key;
    isActivationKeyValid();
}

int
TEBase::isActivationKeyValid()
{
    return 1;
}

void TEBase::setPortBaudRate(int iBaudRate)
{
    m_iPBaudRate=iBaudRate;
    if (m_iPBaudRate!=0 && Port->isOpen())
        Port->setCustomBaudRate((Q_ULONG)m_iPBaudRate);
}

void TEBase::setPortFlowControl(FlowType PFlow)
{
    m_PFlow=PFlow;
    if (m_PFlow!=FLOW_NOTSET && Port->isOpen())
        Port->setFlowControl(m_PFlow);
}

void TEBase::setPortParity(ParityType PPar)
{
    m_PParity=PPar;
    if (m_PParity!=PAR_NOTSET && Port->isOpen())
        Port->setParity(m_PParity);
}

void TEBase::setPortDataBits(DataBitsType PDB)
{
    m_PDataBits=PDB;
    if (m_PDataBits!=DATA_NOTSET && Port->isOpen())
        Port->setDataBits(m_PDataBits);
}

void TEBase::setPortStopBits(StopBitsType PSB)
{
    m_PStopBits=PSB;
    if (m_PStopBits!=STOP_NOTSET && Port->isOpen())
        Port->setStopBits(m_PStopBits);
}

void TEBase::clearPPP()
{
    m_iPBaudRate=0;
    m_PFlow=FLOW_NOTSET;
    m_PParity=PAR_NOTSET;
    m_PDataBits=DATA_NOTSET;
    m_PStopBits=STOP_NOTSET;
}

/*! \ru
 \return - Список поддерживаемых драйвером скоростей COM-порта
 */
QValueList<int> TEBase::supportedBaudRates()
{
  QValueList<int> ret;

  return ret;
}



/*! \ru
 \return - Список поддерживаемых драйвером типов подключений устройства
 */
QValueList<int> TEBase::supportedConnectionTypes()
{
	QValueList<int> ret;

	return ret;
}


/*! \ru
 Запускает драйвер. При выполнении этой функции если установлен режим захвата порта
 (setCaptureMode(CM_Exclusive) ), драйвер пытается захватить необходимые
 ресурсы (COM-порт, сокет и т.п.), в противном случае захват ресурсов происходит во время
 обращения к устройству. Затем переходит в рабочее состояние.
 Если ресурсы не удалось захватить возвращается код ошибки (один из CRERROR_XXX).
 Описание ошибки может быть получено функцией errorText().
 После запуска драйвера не допускается изменение параметров подключения (номер COM-порта,
 битрейт и т.п.).
 \return - код ошибки
 */
int TEBase::startDriver()
{
  return CRERROR_OK;
}

/*! \ru
 Останавливает драйвер. При выполнении этой функции драйвер освобождает все
 занятые ресурсы и переходит в состояние останова.
 Если устройство не может быть остановлено возвращается код ошибки (один из CRERROR_XXX).
 Описание ошибки может быть получено функцией errorText().
 В остановленом состоянии драйвер допускает изменение параметров подключения (номер COM-порта,
 битрейт и т.п.).
 \return - код ошибки
 */
int TEBase::stopDriver()
{
  return CRERROR_OK;
}

/*! \ru
 Устанавливает режим захвата порта: CM_Exclusive - захват порта после вызова startDriver(),
 CM_NonExclusive - захват порта только на время обращения к устройству.
 \return - код ошибки
 */
int TEBase::setCaptureMode(int iMode)
{
  return CRERROR_OK;
}


/*! \fn int TEBase::abstract()
/ru Устанавливает значение iDocNum в номер текущего открытого или последнего закрытого документа.
  При исполнении этой функции возможно обращение к устройству.
 /_ru
/en
 /_en
 */
void TEBase::abstract()
{
	setErrorText(tr("This functions isn't supported by driver or hardware."));
	setErrorCode( CRERROR_NOTSUPPORTED );
}


#endif // TEBASE_CPP
