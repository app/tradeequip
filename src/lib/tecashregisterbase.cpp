/****************************************************************************
** $Id: tecashregisterbase.cpp,v 1.23 2006/12/05 16:26:26 leader Exp $
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
#define TECASHREGISTER_CPP

#include "tecashregisterbase.h"
#include <stdarg.h>
#include <qdict.h>
//#ifdef Q_OS_UNIX
//    #include "setserial_linux.inc"
//#endif



//=======================================

/*!

*/
CCM_CheckItem::CCM_CheckItem()
{

}


/*!

*/
CCM_CheckItem::~CCM_CheckItem()
{

}


/*!

*/
KKM_IO::KKM_IO(int Chan, unsigned long nm)
{
Q_UNUSED( Chan )
Q_UNUSED( nm )
    Channal = Chan;
    MachineNumber = nm;
}

/*!

*/
KKM_IO::~KKM_IO()
{

}

/*!
    \class TECashRegisterBase tecashregisterbase.h
    \brief The TECashRegisterBase is base class for all cash registers classes.

*/
TECashRegisterBase::TECashRegisterBase( int pn, unsigned long nm )
:TEBase( pn )
{
        vMachineNumber = nm;
	vCashSum=0;
	m_bPollPausing=false;
	m_bPollPaused=false;
	m_bWaitingForConfirmation=false;
	setPollInterval(300);
	clearCheckInfo();
	m_bDriverStarted=false;
//	m_bOnlinePrintMode=false;
}

/*!

*/
TECashRegisterBase::~TECashRegisterBase()
{

}


Q_ULONG
TECashRegisterBase::machineNumber()
{
    return vMachineNumber;
}

void
TECashRegisterBase::setMachineNumber( Q_ULONG nm )
{
    vMachineNumber = nm;
}


Q_ULONG
TECashRegisterBase::readROM( char *buf, Q_ULONG Addr, Q_ULONG Len )
{
    Q_UNUSED( buf )
    Q_UNUSED( Addr )
    Q_UNUSED( Len )
    return 0;
}


Q_ULONG
TECashRegisterBase::readRAM( char *buf, Q_ULONG Addr, Q_ULONG Len )
{
    Q_UNUSED( buf )
    Q_UNUSED( Addr )
    Q_UNUSED( Len )
    return 0;
}


Q_ULONG
TECashRegisterBase::readIRAM( char *buf, Q_ULONG Addr, Q_ULONG Len )
{
    Q_UNUSED( buf )
    Q_UNUSED( Addr )
    Q_UNUSED( Len )
    return 0;
}


Q_ULONG
TECashRegisterBase::readEROM( char *buf, Q_ULONG Addr, Q_ULONG Len )
{
    Q_UNUSED( buf )
    Q_UNUSED( Addr )
    Q_UNUSED( Len )
    return 0;
}


Q_ULONG
TECashRegisterBase::writeRAM( char *buf, Q_ULONG Addr, Q_ULONG Len )
{
    Q_UNUSED( buf )
    Q_UNUSED( Addr )
    Q_UNUSED( Len )
    return 0;
}


Q_ULONG
TECashRegisterBase::writeIRAM( char *buf, Q_ULONG Addr, Q_ULONG Len )
{
    Q_UNUSED( buf )
    Q_UNUSED( Addr )
    Q_UNUSED( Len )
    return 0;
}


Q_ULONG
TECashRegisterBase::writeEROM( char *buf, Q_ULONG Addr, Q_ULONG Len )
{
    Q_UNUSED( buf )
    Q_UNUSED( Addr )
    Q_UNUSED( Len )
    return 0;
}


Q_ULONG
TECashRegisterBase::readMachineNumber()
{
    return 0;
}

int
TECashRegisterBase::readSoftwareVersion()
{
    return 0;
}


Q_ULONG
TECashRegisterBase::netNumber()
{
    return vNetworkNumber;
}


void
TECashRegisterBase::setNetNumber( Q_ULONG nn )
{
    vNetworkNumber = nn;
}


Q_ULONG
TECashRegisterBase::readNetNumber()
{
    return 0;
}


void
TECashRegisterBase::writeNetNumber( Q_ULONG nn )
{
    Q_UNUSED( nn )
}

void
TECashRegisterBase::setErrorCode( Q_ULONG errcode )
{
    vErrorCode = errcode;
}


Q_ULONG
TECashRegisterBase::errorCode()
{
    return vErrorCode;
}

int
TECashRegisterBase::setMode( enum Mode mode )
{
    vMode = mode;
    return 0;
}


int
TECashRegisterBase::mode()
{
    return vMode;
}


/*!
Return barcode value.
*/
QString
TECashRegisterBase::barcode()
{
    return vBarcode;
}


/*!
Set barcode value.
*/
void
TECashRegisterBase::setBarcode( const QString &barcode )
{
    vBarcode = barcode;
}


QVariant
TECashRegisterBase::value( const QString &name )
{
	return values.find( name )?*values.find( name ):QVariant();
}


void
TECashRegisterBase::setValue(  const QString &name,  const QVariant &val )
{
    values.remove( name );
    if ( val.isValid() ) values.insert( name, new QVariant( val ) );
}


QString
TECashRegisterBase::valueDescription( const QString &name )
{
    Q_UNUSED( name )
    return "";
}


QStringList
TECashRegisterBase::valueNamesList()
{
    QStringList l;
	QDictIterator<QVariant> it(values);
	for(;it.current()!=0;++it)
		l+=it.currentKey();
    return l;
}


/*!
 * Execute command.
 * Object have internal text command interpreter
 * that give access to object functions.
 * \param cmd (in) command text.
 * \return - command execution result.
 */
QStringList
TECashRegisterBase::execCommand( const QString &cmd )
{
//    QStringList l;
//    l << "999;\"not supported command\"";
    return TEBase::execCommand( cmd );
}

int
TECashRegisterBase::eventProcessed()
{
    return 0;
}


int
TECashRegisterBase::eventRejected()
{
    return 0;
}


int
TECashRegisterBase::bueAdd( const QString &name, int dep, double summa, double quantity, int qdec )
{
    Q_UNUSED( name )
    Q_UNUSED( dep )
    Q_UNUSED( summa )
    Q_UNUSED( quantity )
    Q_UNUSED( qdec )
    return 0;
}


int
TECashRegisterBase::bueCount()
{
    return 0;
}


int
TECashRegisterBase::bueRemove( int idx )
{
    Q_UNUSED( idx )
    return 0;
}


int
TECashRegisterBase::EANControl( Q_ULLONG code)
{
  int i, n;
  int c0=0, c1=0;

  for (i=0;i<12;i++) {
      n=(int)(code%10);
      code=code / 10;
      if (i % 2) c1=c1+n;
      else c0=c0+n;
      if (!code) break;
  }
  c0=c0*3;
  n=(c0+c1) % 10;
  if (n) n=10 - n;
  return n;
}

int TECashRegisterBase::readCashSum()
{
    return 0;
};

QString TECashRegisterBase::addEAN13Checksum(const QString & code)
{
	uint i;
	uint odd=0,even=0;
	if (code.length()!=12)
		return code;
	for (i=0;i<12;i++)
		if (code.at(i)<QChar('0') || code.at(i)>QChar('9'))
			return code; // there's non digits in code. It's not EAN
	for (i=0;i<12;i+=2)
	{
		odd+=code.at(i)-QChar('0');
		even+=code.at(i+1)-QChar('0');
	}
	return code+QChar('0'+((10-((odd+3*even)%10))%10));
};

/*! \fn bool TECashRegisterBase::isCheckOpened()
  /ru
  Возвращает true, если чек открыт. 
  Чек открывается функциями TECashRegisterBase::openCheck() и TECashRegisterBase::openCheck(int),
  закрывается функциями TECashRegisterBase::printCheck(bool) и TECashRegisterBase::closeCheck(double &, int), соответственно.
  Драйвер устройства должен переопределить эту функцию.
  /_ru
 */
bool TECashRegisterBase::isCheckOpened()
{
	return false;
}

/*! \fn bool TECashRegisterBase::onlinePrintMode()
  /ru
  Возвращает true, если драйвер находится в режиме печать online, т.е. печатает
  позиции чека по мере их ввода. Позиция считается введенной после явного или неявного
  вызова функции TECashRegisterBase::endAdd() (см. описание TECashRegisterBase::beginAdd(), TECashRegisterBase::endAdd()).
  Драйвер устройства должен переопределить эту функцию.
  /_ru
  /en
 
  /_en
 */
bool TECashRegisterBase::onlinePrintMode()
{
	return false;
}

/*! \fn void TECashRegisterBase::setOnlinePrintMode(bool bOnlinePM)
/ru	Устанавливает или сбрасывает режим печати online (см. TECashRegisterBase::onlinePrintMode() ).
	Смена режима возможна только при закрытом чеке.
	Драйвер устройства должен переопределить эту функцию.
 * /_ru
 * /en
 *
 * /_en
 */
void TECashRegisterBase::setOnlinePrintMode(bool /*bOnlinePM*/)
{
}

/*! \fn bool TECashRegisterBase::isOnlinePrintModeSupported()
/ru	Возвращает true, если устройство поддерживает режим печати online.
	Драйвер устройства должен переопределить эту функцию.
  /_ru
  /en
 
  /_en
  \sa TECashRegisterBase::onlinePrintMode(), TECashRegisterBase::setOnlinePrintMode()
 */
bool TECashRegisterBase::isOnlinePrintModeSupported()
{
	return false;
}

/*! \fn int TECashRegisterBase::openCheck(int eDocumentType, int & iReserved)
/ru Открывает чек одного из типов, определенных в перечислении CROperations.
	Драйвер устройства должен переопределить эту функцию.
	\param iReserved - зарезервировано. Параметр должен быть равен 0.
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
 /_ru
/en
 
 /_en
 */
int TECashRegisterBase::openCheck(int /*eDocumentType*/, int & /*iReserved*/)
{
	setErrorText(tr("This functions isn't supported by driver or hardware."));
	return CRERROR_NOTSUPPORTED;
}

/*! \fn int TECashRegisterBase::beginAdd(int iReserved)
/ru Отмечает начало добавления новой позиции в чек, очищает буфер текущей позиции.
	Информация о позиции чека хранится в буфере драйвера и не передается устройству до вызова endAdd().
	Если эта функция не вызывается явно, то её вызов происходит при первом вызове
	одной из следующих функций: TECashRegisterBase::addTax(), TECashRegisterBase::setCashSum(),
	TECashRegisterBase::setDiscount(), TECashRegisterBase::setDiscountPercent(),
	TECashRegisterBase::setComment(), TECashRegisterBase::setItem(), TECashRegisterBase::setItemSection(),
    TECashRegisterBase::setUnit().
	Драйвер устройства должен переопределить эту функцию.
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
	/_ru
 /en
 /_en
 */
int TECashRegisterBase::beginAdd(int /*iReserved*/)
{
	setErrorText(tr("This functions isn't supported by driver or hardware."));
	return CRERROR_NOTSUPPORTED;
}

/*! \fn int TECashRegisterBase::addTax(int iTax)
/ru Добавляет в текующую позицию налог с кодом iTax.
	Драйвер устройства должен переопределить эту функцию.
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
 /_ru
 /en
 /_en
 */
int TECashRegisterBase::addTax(int /*iTax*/)
{
	setErrorText(tr("This functions isn't supported by driver or hardware."));
	return CRERROR_NOTSUPPORTED;
}

/*! \fn int TECashRegisterBase::setDiscount(double dDiscount)
/ru Устанавливает скидку или надбавку на текущую позицию.
	Драйвер устройства должен переопределить эту функцию.
	\param dDiscount положительное значение - скидка, отрицательное - надбавка.
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
 /_ru
 /en
 /_en
 */
int TECashRegisterBase::setDiscount(double /*dDiscount*/)
{
	setErrorText(tr("This functions isn't supported by driver or hardware."));
	return CRERROR_NOTSUPPORTED;
}

/*! \fn int TECashRegisterBase::setDiscountPercent(double dDiscount)
/ru Устанавливает скидку или надбавку на текущую позицию в процентах.
	Драйвер устройства должен переопределить эту функцию.
	\param dDiscount положительное значение - скидка, отрицательное - надбавка.
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
 /_ru
 /en
 /_en
 */
int TECashRegisterBase::setDiscountPercent(double)
{
	setErrorText(tr("This functions isn't supported by driver or hardware."));
	return CRERROR_NOTSUPPORTED;
}

/*! \fn int TECashRegisterBase::setItemSection(int iSection)
/ru Устанавливает номер отдела для текущей позиции.
	Драйвер устройства должен переопределить эту функцию.
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
 /_ru
 /en
 /_en
 */
int TECashRegisterBase::setItemSection(int /*iSection*/)
{
	setErrorText(tr("This functions isn't supported by driver or hardware."));
	return CRERROR_NOTSUPPORTED;
}

/*! \fn int TECashRegisterBase::setOperation(int eOperationType)
/ru Устанавливает операцию для текущей позиции. 
	Драйвер устройства должен переопределить эту функцию.
	\param eOperationType одно из значений перечисления CROperations.
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
 /_ru
/en
 /_en
 */
int TECashRegisterBase::setOperation(int /*eOperationType*/)
{
	setErrorText(tr("This functions isn't supported by driver or hardware."));
	return CRERROR_NOTSUPPORTED;
}

/*! \fn int TECashRegisterBase::setUnit(const QString & sUnit)
/ru Устанавливает единицу измерения для текущей позиции. 
	Драйвер устройства должен переопределить эту функцию.
	\param sUnit название единицы измерения.
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
 /_ru
/en
 /_en
 */
int TECashRegisterBase::setUnit(const QString & /*sUnit*/)
{
	setErrorText(tr("This functions isn't supported by driver or hardware."));
	return CRERROR_NOTSUPPORTED;
}

/*! \fn int TECashRegisterBase::setComment(const QString & sComment)
/ru Устанавливает дополнительный печатаемый текст для текущей позиции. 
	Драйвер устройства должен переопределить эту функцию.
	\param sUnit название единицы измерения.
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
 /_ru
/en
 /_en
 */
int TECashRegisterBase::setComment(const QString & /*sComment*/)
{
	setErrorText(tr("This functions isn't supported by driver or hardware."));
	return CRERROR_NOTSUPPORTED;
}

/*! \fn int TECashRegisterBase::setItem(const QString & sName, double dPrice, double dQuantity)
/ru Устанавливает наименование товара, цену и количество.
	В случае, если функция beginAdd() не была вызвана явно, вызывает функцию endAdd(0).
	Драйвер устройства должен переопределить эту функцию.
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
 /_ru
/en
 /_en
 */
int TECashRegisterBase::setItem(const QString & /*sName*/, double /*dPrice*/, double /*dQuantity*/)
{
	setErrorText(tr("This functions isn't supported by driver or hardware."));
	return CRERROR_NOTSUPPORTED;
}

/*! \fn int TECashRegisterBase::endAdd(int iReserved)
/ru Завершает заполнение позиции и передает данные в устройство, или в буфер чека в зависимости
	от режима печати (см. TECashRegisterBase::onlinePrintMode() ).
	Драйвер устройства должен переопределить эту функцию.
	\param sUnit название единицы измерения.
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
 /_ru
/en
 /_en
 */
int TECashRegisterBase::endAdd(int /*iReserved*/)
{
	setErrorText(tr("This functions isn't supported by driver or hardware."));
	return CRERROR_NOTSUPPORTED;
}

/*! \fn int TECashRegisterBase::cancelAdd(int iReserved)
/ru Очищает буфер текущей позиции.
	Драйвер устройства должен переопределить эту функцию.
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
 /_ru
/en
 /_en
 */
int TECashRegisterBase::cancelAdd(int /*iReserved*/)
{
	setErrorText(tr("This functions isn't supported by driver or hardware."));
	return CRERROR_NOTSUPPORTED;
}


/*! \fn int TECashRegisterBase::setPayment(double dSum, int iPaymentType)
/ru Устанавливает сумму оплаты dSum, для вида оплаты iPaymentType.
	Драйвер устройства должен переопределить эту функцию.
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
 /_ru
/en
 /_en
 */
int TECashRegisterBase::setPayment(double /*dSum*/, int /*iPaymentType*/)
{
	setErrorText(tr("This functions isn't supported by driver or hardware."));
	return CRERROR_NOTSUPPORTED;
}

/*! \fn int TECashRegisterBase::payment(double dSum)
/ru Выплата денег из кассы.
	Драйвер устройства должен переопределить эту функцию.
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
 /_ru
/en
 /_en
 */
int TECashRegisterBase::payment(double /*dSum*/)
{
	setErrorText(tr("This functions isn't supported by driver or hardware."));
	return CRERROR_NOTSUPPORTED;
}

/*! \fn int TECashRegisterBase::payingin(double dSum)
/ru Внесение денег в кассу.
	Драйвер устройства должен переопределить эту функцию.
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
 /_ru
/en
 /_en
 */
int TECashRegisterBase::payingin(double /*dSum*/)
{
	setErrorText(tr("This functions isn't supported by driver or hardware."));
	return CRERROR_NOTSUPPORTED;
}

/*! \fn int TECashRegisterBase::addCheckTax(int iTax)
/ru Добавляет в чек налог с кодом iTax.
	Драйвер устройства должен переопределить эту функцию.
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
 /_ru
 /en
 /_en
 */
int TECashRegisterBase::addCheckTax(int /*iTax*/)
{
	setErrorText(tr("This functions isn't supported by driver or hardware."));
	return CRERROR_NOTSUPPORTED;
}

/*! \fn int TECashRegisterBase::cancelCheck(int iReserved)
/ru Отменяет текущий чек.
	Драйвер устройства должен переопределить эту функцию.
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
 /_ru
/en
 /_en
 */
int TECashRegisterBase::cancelCheck(int /*iReserved*/)
{
	setErrorText(tr("This functions isn't supported by driver or hardware."));
	return CRERROR_NOTSUPPORTED;
}

/*! \fn int TECashRegisterBase::closeCheck(double &dChange, int iReserved)
/ru Закрывает текущий чек. Сдача возвращается в переменной dChange.
	Драйвер устройства должен переопределить эту функцию.
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
 /_ru
/en
 /_en
 */
int TECashRegisterBase::closeCheck(double &/*dChange*/, int /*iReserved*/)
{
	setErrorText(tr("This functions isn't supported by driver or hardware."));
	return CRERROR_NOTSUPPORTED;
}

/*! \fn int TECashRegisterBase::printItemOut(PurchaseInfo &)
/ru Если драйвер устройства поддерживает печать online, 
		он должен переопределять эту функцию.
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
 /_ru
/en
 /_en
 */
int TECashRegisterBase::printItemOut(const PurchaseInfo &)
{
	setErrorText(tr("This functions isn't supported by driver or hardware."));
	return CRERROR_NOTSUPPORTED;
}

/*! \fn void TECashRegisterBase::clearCheckInfo()
/ru Очищает m_CheckInfo - информацию о текущем чеке.
 /_ru
/en
 /_en
 */
void TECashRegisterBase::clearCheckInfo()
{
	m_CheckInfo.m_DefaultOperation=-1;
	m_CheckInfo.m_mPayment.clear();
	m_CheckInfo.m_vTaxes.clear();
	m_vCheckBuffer.clear();
}

/*! \fn int TECashRegisterBase::setPassword(int pt, const QString & pass)
/ru Устанавливает пароль оператора, администратора или налогового инспектора.
	\param pt - тип пароля: PTOperator, PTAdmin, PTFiscal.
	\param pass - пароль.
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
 /_ru
/en
 /_en
 */
int TECashRegisterBase::setPassword(int /*pt*/, const QString & /*pass*/)
{
	setErrorText(tr("This functions isn't supported by driver or hardware."));
	return CRERROR_NOTSUPPORTED;
}

/*! \fn int TECashRegisterBase::curDocumentNumber(int & OUT iDocNum)
/ru Устанавливает значение iDocNum в номер текущего открытого или последнего закрытого документа.
  При исполнении этой функции возможно обращение к устройству.
	\param OUT iDocNum - номер текущего открытого или  документа.
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
 /_ru
/en
 /_en
 */
int TECashRegisterBase::curDocumentNumber(int & /*iDocNum*/)
{
	setErrorText(tr("This functions isn't supported by driver or hardware."));
	return CRERROR_NOTSUPPORTED;
}



/*! \fn int TECashRegisterBase::curControlLineNumber(int & OUT iLineNum)
/ru Устанавливает значение iDocNum в номер текущего открытого или последнего закрытого документа.
  При исполнении этой функции возможно обращение к устройству.
	\param OUT iDocNum - номер текущего открытого или  документа.
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
 /_ru
/en
 /_en
 */
int TECashRegisterBase::curControlLineNumber(int & /*iLineNum*/)
{
	setErrorText(tr("This functions isn't supported by driver or hardware."));
	return CRERROR_NOTSUPPORTED;
}



/*! \fn int TECashRegisterBase::curControlLineNumber(int & OUT iLineNum)
/ru Устанавливает значение iDocNum в номер текущего открытого или последнего закрытого документа.
  При исполнении этой функции возможно обращение к устройству.
	\param OUT iDocNum - номер текущего открытого или  документа.
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
 /_ru
/en
 /_en
 */
double TECashRegisterBase::summaTotal()
{
	abstract();
	return 0.0;
}




/*! \fn int TECashRegisterBase::openCashbox(int n)
/ru Открыть денежный ящик номер n, нумерация ящиков начинается с 1.
  При исполнении этой функции возможно обращение к устройству.
	\param n - номер денежного ящика (начиная с 1)
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
 /_ru
/en
 /_en
 */
int TECashRegisterBase::openCashbox(int n)
{
	setErrorText(tr("This functions isn't supported by driver or hardware."));
	return CRERROR_NOTSUPPORTED;
}

/*! \fn int TECashRegisterBase::ZReport()
/ru Запускает вывод Z-отчёта на устройстве.
  При исполнении этой функции возможно обращение к устройству.
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
 /_ru
/en
 /_en
 */
int TECashRegisterBase::ZReport()
{
	setErrorText(tr("This functions isn't supported by driver or hardware."));
	return CRERROR_NOTSUPPORTED;
}

/*! \fn int TECashRegisterBase::XReport()
/ru Запускает вывод X-отчёта на устройстве.
  При исполнении этой функции возможно обращение к устройству.
	\return Ф-ция возвращает CRERROR_OK при успешном выполнении, или один из кодов ошибки CRERROR_XXX.
 /_ru
/en
 /_en
 */
int TECashRegisterBase::XReport()
{
	setErrorText(tr("This functions isn't supported by driver or hardware."));
	return CRERROR_NOTSUPPORTED;
}
