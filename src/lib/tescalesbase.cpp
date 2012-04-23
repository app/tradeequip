/****************************************************************************
** $Id: tescalesbase.cpp,v 1.5 2006/07/18 03:28:32 red75 Exp $
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
#define TESCALES_CPP
#include "tescalesbase.h"
#include <stdarg.h>


//=======================================
/*!
    \class TEScalesBase tescalesbase.h
    \brief The TEScalesBase is base class for all scales classes.

*/

/*!
  
*/
TEScalesBase::TEScalesBase( int pn )
: TEBase( pn )
{
	//vPrice = 0.0;
	//vSumma = 0.0;
	//vWeight = 0.0;
	setPrice(0);
	setTareWeight(0);
	setPLU(0);
	setProdCode("");
	setGroupCode("");
	setProdName("");
	setUseByDate(QDateTime());
	setSumma(0);
	setWeight(0);
}

/*!
*/
TEScalesBase::~TEScalesBase()
{
	close();
}


//int 
//TEScalesBase::readDisplay()
//{
//	return 0;
//}


//void 
//TEScalesBase::setPrice( double Price )
//{
//	vPrice = Price;
//}

//void 
//TEScalesBase::setWeight( double Weight )
//{
//	vWeight = Weight;
//}


//void 
//TEScalesBase::setSumma( double Summa )
//{
//	vSumma = Summa;
//}


//double 
//TEScalesBase::price()
//{
//	return vPrice;
//}

//double 
//TEScalesBase::summa()
//{
//	return vSumma;
//}


//double 
//TEScalesBase::weight()
//{
//	return vWeight;
//}


/*!
*/
//void 
//TEScalesBase::processEvent( int )
//{
//}


/*!
*/
//int 
//TEScalesBase::poll()
//{
//	double oPrice=price(), oWeight=weight(), oSumma=summa();
//	int evt = 0;
//	if ( readDisplay() ) {
//		if ( price()!=oPrice ) evt = 1;
//		if ( weight()!=oWeight ) evt = 2;
//		if ( summa()!=oSumma ) evt = 3;
//	}
//	if ( evt ) emit deviceEvent( evt );
//	return evt;
//}

void TEScalesBase::setPrice( double Price )
{
	m_dPrice=Price;
}



int 
TEScalesBase::readDisplay()
{
	return 0;
}

QString TEScalesBase::productCode() const
{
  return "";
}
