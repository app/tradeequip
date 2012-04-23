/****************************************************************************
** $Id: tecashregisterbase.h,v 1.20 2006/12/01 16:45:51 leader Exp $
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
   Abstract cash register interface object
   (c) V.Grazdankin, 1999-2005
*/
#include <tebase.h> // Damn, We should place that 'include' before 'define TECASHREG...', see templexports.h
#ifndef TECASHREGISTERBASE_H
#define TECASHREGISTERBASE_H
#include <qdict.h>
#include <qstring.h>
#include <qvariant.h>
#include <qsqldatabase.h>
#include <qvaluelist.h>
//#include <vector>

//using std::vector;

//#ifdef Q_OS_UNIX
//enum par_code   { COM_NONE, COM_EVEN, COM_ODD, COM_ZERO, COM_ONE };
//void serial_setparams(int port, int speed, int bits, enum par_code parity, int stopbits);
//#endif

//
/*
typedef struct {
   int     KL;          // # control line
   int     NBue;        // # bue
   int     NCBue;       // # bue in check
   int     Dep;         // # departament
   char    AskCode[20]; // asking barcode
   char    Code[20];    // finding barcode
   char    Name[200];   // full name
   char    PName[20];   // printable name
   double  Price;       // price
   double  Quantity;    // quantity
   double  Sum;         // summa
   double  Discount;    // discount
} Bue_Item;
*/


typedef struct {
   char Cmd;
   unsigned long NM;
   unsigned int Addr;
   char Len;
   char data[16];
   char CS;
} Socket_Rec;

//
typedef struct {
   int nbue;
   char code[20];
   char name[60];
} bue_lst;

/*
class CCMConnection : public QObject
{
Q_OBJECT

public:

};
*/

class CCM_CheckItem : public QObject
{
Q_OBJECT

public:
    CCM_CheckItem();
    ~CCM_CheckItem();

private:
    int     KL;          // # control line
    int     NBue;        // # bue
    int     NCBue;       // # bue in check
    int     Dep;         // # departament
    char    AskCode[20]; // asking barcode
    char    Code[20];    // finding barcode
    char    Name[200];   // full name
    char    PName[20];   // printable name
    double  Price;       // price
    double  Quantity;    // quantity
    double  Sum;         // summa
    double  Discount;    // discount
};

struct LIB_EXPORT PurchaseInfo
{
	PurchaseInfo()
	{
		clear();
	}

	PurchaseInfo(const PurchaseInfo & in) :
		m_name(in.m_name),m_dep(in.m_dep),m_price(in.m_price),m_quantity(in.m_quantity),
		m_qdec(in.m_qdec),m_taxes(in.m_taxes),m_dDiscount(in.m_dDiscount),
		m_isDiscountInPercent(in.m_isDiscountInPercent), m_iOperation(in.m_iOperation),
		m_sUnit(in.m_sUnit), m_sComment(in.m_sComment), m_isEmpty(in.m_isEmpty)
	{
	}

	PurchaseInfo(const QString &name, int dep, double price, double quantity, int qdec):
		m_name(name),m_dep(dep),m_price(price),m_quantity(quantity),m_qdec(qdec),m_isEmpty(false)
	{
	}

	void clear()
	{
		m_isEmpty=true;
		m_name="";
		m_dep=0;
		m_price=0;
		m_quantity=0;
		m_qdec=3;
		m_taxes.clear();
		m_dDiscount=0;
		m_isDiscountInPercent=false;
		m_iOperation=-1;
		m_sUnit="";
		m_sComment="";
	}

	bool isEmpty()
	{
		return m_isEmpty;
	}

	QString m_name;
	int m_dep;
	double m_price;
	double m_quantity;
	int m_qdec;
	QValueList<int> m_taxes;
	double m_dDiscount;
	bool m_isDiscountInPercent;
	int m_iOperation;
	QString m_sUnit,m_sComment;
	bool m_isEmpty;
	bool operator < (const PurchaseInfo & in) const
	{
		return m_name<in.m_name;
	}
	bool operator == (const PurchaseInfo & in) const
	{
		return (m_name==in.m_name) && (m_dep==in.m_dep) && (m_price==in.m_price) && (m_quantity==in.m_quantity);
	}
};

/*
Class for database connection and work.
*/
/*
class CCM_Database : public QObject
{
public:
    CCM_Database();
    ~CCM_Database();
private:
    QSqlDatabase *db;
};
*/
//---------------------------------------------------------------------------
class KKM_IO : public QObject
{
Q_OBJECT

public:
    unsigned long MachineNumber;
    int Channal;
    KKM_IO(int Chan, unsigned long nm);
    ~KKM_IO();

private:

};
//---------------------------------------------------------------------------

#define     EVENT_NONE      0
#define     EVENT_CLOSECHECK    1
#define     EVENT_ASKBARCODE    2
#define     EVENT_RESETCHECK    3
#define     EVENT_MANUALSALE    4
#define     EVENT_CONFIRMBUE    5
#define     EVENT_PRINT_ZERO_CHECK 11
#define     EVENT_REQUEST_ZREPORT 12

#define     EVENT_POWEROFF      100
#define     EVENT_POWERON       101
#define     EVENT_BUSY      102
#define     EVENT_READY     103
#define     EVENT_DRIVER_ACTIVATED 104
#define     EVENT_DRIVER_ACTIVATION_FAILED 105
#define     EVENT_CRITICAL_FAIL 106
#define		EVENT_CONFIRMATION 107
#define     EVENT_UNKNOWN 128

enum CROperations
{
	CRO_SELL=0x0,
	CRO_BUY=0x1,
	CRO_SELLRET=0x2,
	CRO_BUYRET=0x3,
	CRO_SELLSTORNO=0x4,
	CRO_BUYSTORNO=0x5,
	CRO_CASHIN=0x6,
	CRO_CASHOUT=0x7,
	CRO_DISCOUNT=0x8,
	CRO_DISCOUNTPERCENT=0x9,
	CRO_OPMASK=0xF
	//CRO_SIMPLEMODE=0x8,
	//CRO_MODEMASK=0x8,
	//CRO_CASHLESS=0x10,
	//CRO_PAYMENTMASK=0x10
};

//template class LIB_EXPORT QDict <QVariant>;
//template class LIB_EXPORT std::vector <PurchaseInfo>;
//template class LIB_EXPORT std::allocator <PurchaseInfo>;
#include <templexports.h>

struct LIB_EXPORT CheckInfo
{
	QValueList<int> m_vTaxes;
	QMap<int,double> m_mPayment;
	int m_DefaultOperation;
	int m_CheckOperation;
};

enum PassType
{
	PTOperator=0,
	PTAdmin=1,
	PTFiscal=2
};

class LIB_EXPORT TECashRegisterBase : public TEBase
{
Q_OBJECT

public:

//  enum eventCode {
//  EVENT_NONE, EVENT_CLOSECHECK, EVENT_ASKBARCODE,
//  EVENT_RESETCHECK, EVENT_MANUALSALE, EVENT_CONFIRMBUE
//  };

    enum Mode { MOD_OFF, MOD_ONLINE, MOD_CHECKONLINE };

    TECashRegisterBase(int pn, unsigned long nm);
    ~TECashRegisterBase();

    static int EANControl(Q_ULLONG code);

    virtual Q_ULONG readROM( char *buf, Q_ULONG Addr, Q_ULONG Len );
    virtual Q_ULONG readRAM( char *buf, Q_ULONG Addr, Q_ULONG Len );
    virtual Q_ULONG readIRAM( char *buf, Q_ULONG Addr, Q_ULONG Len );
    virtual Q_ULONG readEROM( char *buf, Q_ULONG Addr, Q_ULONG Len );
    virtual Q_ULONG writeRAM( char *buf, Q_ULONG Addr, Q_ULONG Len );
    virtual Q_ULONG writeIRAM( char *buf, Q_ULONG Addr, Q_ULONG Len );
    virtual Q_ULONG writeEROM( char *buf, Q_ULONG Addr, Q_ULONG Len );

    virtual Q_ULONG readMachineNumber();
    virtual int readSoftwareVersion();
  	virtual int softwareVersion(){return (int)vSoftwareVersion;};
    virtual Q_ULONG netNumber();
    virtual void setNetNumber( Q_ULONG nn );
    virtual Q_ULONG readNetNumber();
    virtual void writeNetNumber( Q_ULONG nn );
    virtual int readCashSum();
    virtual double cashSum() {return vCashSum;};
    virtual void setCashSum(double cashSum){vCashSum=cashSum;};
    virtual void setPollPausing(bool bPollPausing){m_bPollPausing=bPollPausing;};
    virtual bool pollPausing(){return m_bPollPausing;} // pause polling after particular events
	virtual bool pollPaused() const {return m_bPollPaused;} // unconditionally pause polling
	virtual void setPollPaused(bool pollPaused){m_bPollPaused=pollPaused;}
    virtual double buySum(){return vSum;}; // Sum of buyed/returned position
    virtual void setBuySum(double sum){vSum=sum;};
    virtual double quantity(){return vQuantity;}; 
    virtual void setQuantity(double quantity){vQuantity=quantity;};
    virtual double price(){return vPrice;};
    virtual void setPrice(double price){vPrice=price;};
    virtual QString productDesc() {return vProdDesc;};
    virtual void setProductDesc(const QString & prodDesc){vProdDesc=prodDesc;};
    virtual int precision(){return vQDec;};
    virtual void setPrecision(int dec){vQDec=dec;};
    virtual int checkNum(){return m_iChkNum;};
    virtual void setCheckNum(int iChkNum){m_iChkNum=iChkNum;};
    virtual int prodNum(){return m_iProdNum;};
    virtual void setProdNum(int iProdNum){m_iProdNum=iProdNum;};
    virtual int prodCount(){return m_iProdCnt;};
    virtual void setProdCount(int iProdCnt){m_iProdCnt=iProdCnt;};
    virtual int section(){return m_iSection;};
    virtual void setSection(int iSection){m_iSection=iSection;};
    virtual enum Mode currentMode(){return vMode;};
    virtual void setReturnFlag(bool bReturnFlag){m_bReturnFlag=bReturnFlag;}
    virtual bool returnFlag(){return m_bReturnFlag;}
	
	void setCheckHeader(const QString & sCheckHeader){m_sCheckHeader=sCheckHeader;}
	QString checkHeader() const {return m_sCheckHeader;}

	// this function puts driver in wait for user action state and returns
	// When user enters specified command sequense, driver prints check filled with previous call to bueAdd
	virtual bool printCheck(bool returnFlag)=0; // returns true on ok, false on error
	virtual bool cancelPrint()=0;
	virtual bool openCheck()=0;

//
  virtual int openCashbox(int n); // open cash box number n
  virtual int ZReport(); 
  virtual int XReport();

// ----------------- New check related functions. ------------------------
	virtual bool isCheckOpened();
	virtual bool onlinePrintMode();
	virtual void setOnlinePrintMode(bool bOnlinePM);
	virtual bool isOnlinePrintModeSupported();

	virtual int openCheck(int eDocumentType, int & iReserved);
	virtual int beginAdd(int iReserved);
	virtual int addTax(int iTax);
	virtual int setDiscount(double dDiscount); // absolute value of a discount
	virtual int setDiscountPercent(double dDiscProcent);
	virtual int setItemSection(int iSection);
	virtual int setOperation(int eOperationType);
	virtual int setUnit(const QString & sUnit);
	virtual int setComment(const QString & sComment);
	virtual int setItem(const QString & sName, double dPrice, double dQuantity);
	virtual int endAdd(int iReserved);
	virtual int cancelAdd(int iReserved);
	virtual int setPayment(double dSum, int iType=0); 
	virtual int addCheckTax(int iTax); 
	virtual int cancelCheck(int iReserved);
	virtual int closeCheck(double & dChange, int iReserved);
  virtual int curDocumentNumber(int & iDocnum);
  virtual int curControlLineNumber(int & iLinenum);
	virtual double summaTotal();	
  
	virtual int printItemOut(const PurchaseInfo &);
    virtual int payment(double dSum); // Выплата
    virtual int payingin(double dSum); // Внесение
// ----------------- End of check related functions. --------------------

// General device status functions
//!< Is Checks for device readiness for action. 
  virtual int checkDeviceReady()=0;  
// End

    virtual void setErrorCode( Q_ULONG errcode );
    virtual Q_ULONG errorCode();

    virtual int mode();
    virtual int setMode( enum Mode mod );
    virtual QString barcode();
    virtual void setBarcode( const QString &barcode );
    virtual int controlLineNumber(){ return value("CLNUMBER").toInt();};
    virtual void setControlLineNumber( int clnumber ){ setValue("CLNUMBER", clnumber);};
    virtual int eventProcessed();
    virtual int eventRejected();

    virtual int bueAdd( const QString &name, int dep, double summa, double quantity, int qdec );
    virtual int bueCount();
    virtual int bueRemove( int idx );

    static QString addEAN13Checksum(const QString &);

    void setModeInt(int mode){
        switch(mode){case 0:setMode(TECashRegisterBase::MOD_OFF);break;
            case 1:setMode(TECashRegisterBase::MOD_ONLINE);break;
            case 2:setMode(TECashRegisterBase::MOD_CHECKONLINE);break;
            default:break;//{char buf[]={0x2,0x12,(char)mode};SendCommand(buf,sizeof(buf),false,0);break;};
        };
    }

	virtual int setPassword(int pt, const QString & pass);

public slots:
    Q_ULONG machineNumber();
    void setMachineNumber( Q_ULONG nm );

    virtual QVariant value( const QString &name );
    virtual void setValue(  const QString &name,  const QVariant &val );
    virtual QString valueDescription( const QString &name );
    virtual QStringList valueNamesList();
    virtual QStringList execCommand( const QString &cmd );

private:
    Q_ULONG vMachineNumber, vNetworkNumber, vErrorCode, vSoftwareVersion;
    enum Mode vMode;
    QString vBarcode;
    int vEvent;
    QDict <QVariant> values;
    double vCashSum; // sum of cash in hand
    bool m_bPollPausing;

    double vSum; // cash sum for sale/return without barcode
    double vQuantity;
    double vPrice;
    QString vProdDesc;
    int vQDec;
    int Channel;
    int m_iChkNum; // current check number
    int m_iProdNum; // product number
    int m_iProdCnt; // total number of products in check
    int m_iSection; // section in which current product was buyed
    bool m_bReturnFlag; //
    bool m_bPollPaused;
    QString m_sCheckHeader;

protected:
    QValueList<PurchaseInfo> m_vCheckBuffer;
	CheckInfo m_CheckInfo;
    bool m_bWaitingForConfirmation;

	void clearCheckInfo();
};


#endif


