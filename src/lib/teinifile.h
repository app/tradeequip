/****************************************************************************
** $Id: teinifile.h,v 1.10 2006/07/07 08:55:50 red75 Exp $
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

// TEIniFile 1.1
//
// Supports storing/reading parameters in text configuration files (like .ini)
//
// (c) Racheengel 2004-2005
#ifndef TEINIFILE_H
#define TEINIFILE_H

#include <qvariant.h>
#include <qwidget.h>
#include <qfile.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qtextstream.h>
#include <qmap.h>
#include <qcolor.h>
#include <qrect.h>
#include "teglobal.h"

typedef QMap<QString, QString> type_ValueList;
typedef QMapIterator<QString, QString> type_ValueListIterator;

#include "templexports.h"

class LIB_EXPORT TEIniFile
{
public:
	TEIniFile(const QString &name = QString::null);
	~TEIniFile();

	void	setFileName(const QString &name);
  QString fileName() const {return f.name();}
	bool	openWrite();
	bool	openRead();
	void	close();

	void	writeBreak();
	void	writeSection(const QString &section);
	void	writeComment(const QString &comment);
	void	writeString(const QString &name, const QString &value);
	void	writeData(const QString &name, const QString &value);
	void	writeNumber(const QString &name, const int value, const int base = 10);
	void	writeStringList(const QString &name, const QStringList &value);
	void	writeDataList(const QString &name, const QStringList &value);
	void	writeBool(const QString &name, const bool value);
	void	writeColor(const QString &name, const QColor& color);
	void	writeRect(const QString &name, const QRect& rect);

	bool	sectionExists(const QString &section)
			{ return SectionList.contains(section); }
	bool	useSection(const QString &section);
	const QString&	readString(const QString &name, const QString &def = QString::null);
	int		readNumber(const QString &name, const int def = 0, const int base = 10);
	bool	readBool(const QString &name, const bool def = false);
	const QColor&	readColor(const QString &name, const QColor& def = QColor());
	const QRect&	readRect(const QString &name, const QRect& def = QRect());

	void	setString(const QString &name, const QString &value);
	void	setData(const QString &name, const QString &value);
	void	setNumber(const QString &name, const int value, const int base = 10);
	void	setStringList(const QString &name, const QStringList &value);
	void	setDataList(const QString &name, const QStringList &value);
	void	setBool(const QString &name, const bool value);
	void	setColor(const QString &name, const QColor& color);
	void	setRect(const QString &name, const QRect& rect);

  /*! Deletes name from current section */
  void deleteName(const QString &name);

  bool isNameExists(const QString &name);
  // These functions return true if readXXX function was called with the same /name/ value already.
  bool getString(const QString & name, QString & value);
//  bool getData(const QString & name, QString & value);
  bool getNumber(const QString & name, int & value, const int base=10);
  bool getBool(const QString & name, bool & value);

  bool update();

  QStringList usedValues();
protected:
	QTextStream ts;
	QFile		f;
	QMap<QString, type_ValueList> SectionList;
  QMap<QString, type_ValueList> SectionListDef;
	QMapIterator<QString, type_ValueList>	mi;
  QString m_sCurSection;
	bool		m_break;
};

#endif
