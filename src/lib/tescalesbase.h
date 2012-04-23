/****************************************************************************
** $Id: tescalesbase.h,v 1.6 2006/07/18 03:28:33 red75 Exp $
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
   Abstract Scales interface object
   (c) V.Grazdankin, 1999-2005
*/
#ifndef TESCALES_H
#define TESCALES_H
#include <tebase.h>

#define EVENT_SCALES_WEIGHT_READ 1
#define EVENT_SCALES_COMM_ERROR 100
#define EVENT_SCALES_QUERY_INFO 2

struct PLUInfo {
    unsigned int  PLU,
         ICode;
    char Name[56];
    int  Price,
         SLife,
         Tare,
         GCode,
         MsgNo;
};

//---------------------------------------------------------------------------
class LIB_EXPORT TEScalesBase : public TEBase
{
    Q_OBJECT
    
public:
    TEScalesBase( int pn );
    ~TEScalesBase();

  enum PLUCapsEnum
  {
    PCAP_BCFORMAT=1,
    PCAP_BCDATE=2,
    PCAP_USEBYDATE=4,
    PCAP_USEBYTIME=8,
    PCAP_SELLBYDATE=16,
    PCAP_SELLBYTIME=32,
    PCAP_PACKDATE=64,
    PCAP_PACKTIME=128,
    PCAP_TARE=256,
    PCAP_PICTURENUM=512,
    PCAP_MAX=0xffffffff
  };

  enum GenCapsEnum
  {
    GCAP_GETWEIGHT=1, // Can read current value of weight.
    GCAP_GETPRICE=2, // Can read current product's price.
    GCAP_GETPLU=4,
    GCAP_CANQUERYPLU=8, // Scales can query PLU data. Driver will emit deviceEvent(EVENT_SCALES_QUERY_INFO) in responce.
    GCAP_MAX=0xffffffff
  };

public slots:
  virtual uint PLUCaps() const {return 0;} // Returns supported PLU fields (PLUCapsEnum members or'ed together)
  virtual uint GenCaps() const {return 0;} // Returns driver/scale capabilities.
  
  virtual int writePLU(){return 0;} // Writes PLU and all its attributes set by setXXX functions.
  virtual int detelePLU(){return 0;} // deletes PLU set by setPLU function from scales memory
  virtual int deletePLU(uint uiPLU){return 0;} // deletes uiPLU from scales memory

  QString productCode() const; // Can be used after EVENT_SCALES_QUERY_INFO. 
                                 //Returns product code for which information was requested.
    
    
	double price() const {return m_dPrice;}
	virtual void setPrice( double Price );
	
	double tareWeight() const {return m_dTareWeight;}
	virtual void setTareWeight( double dTareWeight ) {m_dTareWeight=dTareWeight;}

	unsigned int PLU() const {return m_uiPLU;}
	virtual void setPLU(unsigned int uiPLU) {m_uiPLU=uiPLU;}

	QString prodCode() const {return m_sProdCode;}
	virtual void setProdCode(const QString & sProdCode) {m_sProdCode=sProdCode;}

	QString groupCode() const {return m_sGroupCode;}
	virtual void setGroupCode(const QString & sGroupCode) {m_sGroupCode=sGroupCode;}

	QString prodName() const {return m_sProdName;}
	virtual void setProdName(const QString & sProdName) {m_sProdName=sProdName;}

	QDateTime useByDate() const {return m_dUseByDate;}
	virtual void setUseByDate(const QDateTime & dUseByDate) {m_dUseByDate=dUseByDate;}
	
	double weight() const {return 0;}
	double summa() const {return 0;}

public slots:
	virtual int readDisplay();

protected:
	void setSumma(double dSum){ Q_UNUSED( dSum );};
	void setWeight(double dWeight){ Q_UNUSED( dWeight ); };

//    virtual void processEvent( int Code );
//    int poll();
private:
	unsigned int m_uiPLU;
	QString m_sProdCode;
	QString m_sGroupCode;
	double m_dPrice;
	double m_dTareWeight;
	QString m_sProdName;
	QDateTime m_dUseByDate;
};
//---------------------------------------------------------------------------

#endif


