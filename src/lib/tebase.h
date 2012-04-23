/****************************************************************************
** $Id: tebase.h,v 1.17 2006/12/03 16:23:13 leader Exp $
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
   Abstract trade equipment interface object
*/
#ifndef TEBASE_H
#define TEBASE_H

#include <teglobal.h>
#include <templexports.h>
#include <qdict.h>
#include <qintdict.h>
#include <tserialport.h>
#include <cmdparser.h>
#include <qstring.h>


/*! \ru \class TEConnectionSettingsInterface */
class LIB_EXPORT TEConnectionSettingsInterface
{
public:
  //! \ru Перечисление (типы допустимых значений параметра)
  /*! */
  enum ParameterDomain
  {
	  PT_Invalid, /*!< - возвращается для несуществующего параметра */
	  PT_IntRangeList, /*!< - допустимые значения представляют собой набор диапазонов целых чисел */
	  PT_IntList, /*!< - допустимые значения представляют собой набор целых чисел */
	  PT_String, /*!< - допускается произвольная строка */
	  PT_Double, /*!< - допускается произвольное вещественное число */
	  PT_Bool /*!< - значение булевского типа */
  };

  //! \ru Перечисление (предпочтительный тип представления значения)
  /*! */
  enum PreferredUIControl
  {
	  PUC_LineEdit, /*!< - строка ввода */
	  PUC_Slider, /*!< - слайдер */
	  PUC_DropDown, /*!< - выпадающий список */
	  PUC_CheckBox, /*!< - чекбокс */
    PUC_RadioButtons /*!< - */
  };

  //! \ru Количество настраиваемых параметров
  /*! \ru \return Количество настраимаемых параметров */
  virtual int numberOfParameters()=0;

  //! \ru Тип допустимых значений
  /*! \ru
   \param num номер параметра (0-based).
   \return одно из значений перечисления ParameterDomain. */
  virtual int parameterDomain(int num)=0;

  //! \ru Предпочитаемый тип представления значения
  /*! \ru
   \param num номер параметра (0-based).
   \return одно из значений перечисления PreferredUIControl. */
  virtual int preferredUIControl(int num)=0;

  //! \ru Описание назначения параметра
  /*! \ru
   \param num номер параметра (0-based).
   \return Описание назначения параметра. */
  virtual QString parameterDescription(int num)=0;

  //! \ru Количество элементов в списке допустимых значений/диапазонов
  /*! \ru
   \param num номер параметра (0-based).
   \return Количество элементов в списке допустимых значений/диапазонов. */
  virtual int parameterItemsCount(int num)=0;

  //! \ru Описание элемента item из списка допустимых значений/диапазонов параметра num
  /*! \ru
   \param num номер параметра (0-based).
   \param item номер элемента (0-based).
   \return Описание элемента item из списка допустимых значений/диапазонов параметра num. */
  virtual QString parameterItemDescription(int num, int item)=0;

  //! \ru Получить допустимый интервал номер item
  /*! \ru
   \param num номер параметра (0-based).
   \param item номер элемента (0-based).
   \param low (out) нижняя граница интервала.
   \param hi (out) верхняя граница интервала (инклюзивно).
   \return Код ошибки. */
  virtual int getItemRange(int num, int item, int & low, int & hi)=0;

  //! \ru Получить допустимое значение номер item
  /*! \ru
   \param num номер параметра (0-based).
   \param item номер элемента (0-based).
   \param val (out) допустимое значение.
   \return Код ошибки. */
  virtual int getItemValue(int num, int item, int & val)=0;

  //! \ru Установить параметр номер num
  /*! \ru
   \param num номер параметра (0-based).
   \param val устанавливаемое значение.
   \return Код ошибки. */
  virtual int setParameter(int num, int val)=0;
  virtual int setParameter(int num, const QString & val)=0;
  virtual int setParameter(int num, double val)=0;
  virtual int setParameter(int num, bool val)=0;

  //! \ru Получить параметр номер num
  /*! \ru
   \param num номер параметра (0-based).
   \param val (out) значение параметра.
   \return Код ошибки. */
  virtual int getParameter(int num, int & val)=0;
  virtual int getParameter(int num, QString & val)=0;
  virtual int getParameter(int num, double & val)=0;
  virtual int getParameter(int num, bool &val)=0;

  //! \ru Описание последней ошибки
  /*! \ru
   \return Описание последней ошибки. */
  virtual QString errorText()=0;
};

//---------------------------------------------------------------------------
class LIB_EXPORT TEBase : public QObject, public TE
{
    Q_OBJECT
    Q_PROPERTY( QString ActivationKey READ activationKey WRITE setActivationKey )
    Q_PROPERTY( int PortNumber READ portNumber WRITE setPortNumber )
    Q_PROPERTY( int PortBaudRate READ portBaudRate WRITE setPortBaudRate )

public:

    TEBase( int pn );
    ~TEBase();

    // Interface functions
    virtual void setActivationKey( const QString &key );
    virtual QString activationKey() const;

    int connectionType();
    virtual int setConnectionType( int ct );

    virtual int setPortNumber( int pn );
    int portNumber() const;

    void setPollInterval( int pint );
    int pollInterval();

    virtual int isOpen();

    int timeout();
    void setTimeout( int timeout );
    bool isTimeout();

    int debugLevel();
    void setDebugLevel( int level );

    virtual QCString utf8ToDevice( const QString &text );
    virtual QString deviceToUtf8( const QCString &text );

	virtual int isActivationKeyValid();

    int errorCode() const { return iErrorCode; }
    void setErrorCode( const int &code ) { iErrorCode = code; }

    virtual QString errorText() const;// {return m_sError;}
    virtual void setErrorText(const QString & err){m_sError=err;}

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
    virtual QValueList<int> supportedConnectionTypes(); // Non const, just in case.


  virtual int open();
  virtual int close();

public slots:
	virtual int startDriver();
	virtual int stopDriver();
	virtual int setCaptureMode(int iMode); // port capture mode (one of CaptureMode enum), returns error code
	virtual int captureMode() {return m_iCaptureMode;};
	virtual void processEvent( int Code );
	virtual QStringList execCommand( const QString &cmd );
	virtual bool isDriverRunning() {return m_bDriverStarted;}


signals:
	void deviceEvent( int Code );

protected:

    void abstract();
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
  void clearPPP(); // clears persistent port parameters

	void timerEvent( QTimerEvent * );
	ExecEngine m_ee;
  int m_iCaptureMode; //
  bool m_bDriverStarted;

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

    int		iErrorCode;	// error code. 0 - if no error.
    QString 	m_sError; 	// error description
};
//---------------------------------------------------------------------------

#endif
