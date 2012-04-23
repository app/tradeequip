/****************************************************************************
** $Id: teinifile.cpp,v 1.12 2006/07/04 06:07:06 red75 Exp $
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
/*!
    \class TEIniFile teinifile.h
    \brief Supports storing/reading parameters in text configuration files (like .ini).
*/
// TEIniFile 1.1
//
// Supports storing/reading parameters in text configuration files (like .ini)
//
// (c) Racheengel 2004-2005



#include "teinifile.h"
#include <qdict.h>
#include <qapplication.h>
#include <qtextcodec.h>

TEIniFile::TEIniFile(const QString &name)
{
	setFileName(name);
  ts.setEncoding(QTextStream::UnicodeUTF8);
}
void TEIniFile::setFileName(const QString &name)
{
	f.setName(name);
}

TEIniFile::~TEIniFile()
{
	close();
}

////////////////////// Write operations /////////////////////////////////

bool TEIniFile::openWrite()
{
	if (!f.open(IO_WriteOnly)) return false;
	ts.setDevice(&f);
	m_break = false;
	return true;
}

void TEIniFile::close()
{
	if (f.isOpen()) f.close();
}

void TEIniFile::writeBreak()
{
	ts << "\n";
}

void TEIniFile::writeComment(const QString &comment)
{
	ts << "#" << comment << "\n";
}

void TEIniFile::writeSection(const QString &section)
{
//	if (m_break) ts << "\n";
	ts << "[" << section << "]\n";
//	m_break = true;
}

void TEIniFile::writeString(const QString &name, const QString &value)
{
	ts << name << "\t\"" << value << "\"\n";
}

QString qstrFromData(const QString & val)
{
  return QString("data{\n")+val+"}data";
}

void TEIniFile::writeData(const QString &name, const QString &value)
{
	ts << name << "\t" << qstrFromData(value) << "\n"; //"data{\n" << value;
	//if (!value.isEmpty()) ts << "\n";
	//ts << "}data\n";
}

QString qstrFromInt(const int value, const int base=10)
{
  return QString().setNum(value,base);
}

void TEIniFile::writeNumber(const QString &name, const int value, const int base)
{
	//QString val;
	//val.setNum(value, base);
	ts << name << "\t" << qstrFromInt(value,base) << "\n";
}

QString qstrFromBool(const bool value)
{
  return QString().setNum((int)value);
}

void TEIniFile::writeBool(const QString &name, const bool value)
{
	ts << name << "\t" << qstrFromBool(value) << "\n";
}

QString qstrFromColor(const QColor & color)
{
  return color.isValid() ? color.name() : QString::null;
}

void TEIniFile::writeColor(const QString &name, const QColor& color)
{
	ts << name << "\t" << qstrFromColor(color) << "\n";
}

QString qstrFromRect(const QRect & rect)
{
  return QString().setNum(rect.x())+" "+
    QString().setNum(rect.y())+" "+
    QString().setNum(rect.width())+" "+
    QString().setNum(rect.height());
}

void TEIniFile::writeRect(const QString &name, const QRect& rect)
{
	ts << name << "\t" << qstrFromRect(rect) << "\n";
}

// Red75: Excellent! Is is write only function, readStringList isn't implemented!
void TEIniFile::writeStringList(const QString &name, const QStringList &value)
{
	int i = 0;
	QStringList::Iterator it;
	for ( it = ((QStringList&)value).begin(); it != ((QStringList&)value).end(); ++it )
		writeString(QString("%1.%2").arg(name).arg(i++), *it);
}

// Red75: It's same as abovementioned function.
void TEIniFile::writeDataList(const QString &name, const QStringList &value)
{
	int i = 0;
	QStringList::Iterator it;
	for ( it = ((QStringList&)value).begin(); it != ((QStringList&)value).end(); ++it )
		writeData(QString("%1.%2").arg(name).arg(i++), *it);
}


////////////////////// Read operations /////////////////////////////////

typedef enum { P_NONE, P_SECTION, P_DATA } type_Processing;

bool TEIniFile::openRead()
{
	if (!f.open(IO_ReadOnly)) return false;
	ts.setDevice(&f);
	// prepare
	SectionList.clear();
//  SectionListDef.clear(); // default values should persist
	mi = NULL;

	// scan form sections
	QString line, sec;
	type_Processing pr = P_NONE;	// nothing done yet
	type_ValueList ValueList;

	while (!((line = ts.readLine()).isNull())) {
		if ((pr == P_NONE) || (pr == P_SECTION)) {
			// trim line
			line = line.stripWhiteSpace();
			// skip if empty
			if (line.isEmpty()) continue;
			// skip if comment
			if (line.startsWith("#")) continue;
			// check if section
			if (line.startsWith("[") && line.endsWith("]")) {
				if (pr == P_SECTION) {
					// update previous section
					SectionList[sec] = ValueList;
				}
				// section found!
				pr = P_SECTION;
				sec = line.mid(1, line.length()-2).stripWhiteSpace();
				ValueList.clear();
				continue;
			}
			if (pr == P_SECTION) {
				// read name
				QString nam;
				int j = 0, l = (int)line.length();
				QChar c;
				while ( (j < l) && !((c = line.ref((uint)j)).isSpace()) ) {
					nam += c;
					j++;
				}
				// read value
				QString val;
				val = line.mid((uint)j).stripWhiteSpace();
				if (val == "data{") {
					// read raw data...
					val = "";
					while (!((line = ts.readLine()).isNull())) {
						if (line == "}data") {
							ValueList[nam] = val;
							break;
						}
						val += line + "\n";
					}
				} else {
					// test for ""
					if (val.startsWith("\"") && val.endsWith("\""))
						val = val.mid(1, val.length()-2).stripWhiteSpace();
					ValueList[nam] = val;
				}
			}
			// else some shit has been read... skip this.
		}
	}

	if (pr == P_SECTION) {
		// update last section
		SectionList[sec] = ValueList;
	}

	return true;
}

// makes section active...
bool TEIniFile::useSection(const QString &section)
{
  m_sCurSection=section;
	mi = SectionList.find(section);
	if(mi == SectionList.end())
  {
    type_ValueList empty;
    SectionList[section]=empty;
    mi=SectionList.find(section);
  }
  return true;
}

// reads string
const QString& TEIniFile::readString(const QString &name, const QString &def)
{
	if (mi == NULL) return (QString&)def;
  SectionListDef[m_sCurSection][name]=def;
	return (mi.data().contains(name)) ? mi.data()[name] : def;
}

bool TEIniFile::getString(const QString & name, QString & value)
{
  if (SectionListDef[m_sCurSection].find(name)==SectionListDef[m_sCurSection].end())
  {
    value=readString(name,"");
    return false;
  }
  value=readString(name,SectionListDef[m_sCurSection][name]);
  return true;
}

// reads integer number
int TEIniFile::readNumber(const QString &name, const int def, const int base)
{
	bool ok;
	int d = readString(name).toInt( &ok, base );
  SectionListDef[m_sCurSection][name]=qstrFromInt(def,base);
  //if (!ok && mi.data().contains(name))
  //  cfg_message(2,"%s",(const char *)qApp->translate("Config","Config file: section [%1] value of %2 is supposed to be integer").arg(m_sCurSection).arg(name).utf8());
	return ok ? d : def;
}

bool TEIniFile::getNumber(const QString & name, int & value, const int base)
{
  if (SectionListDef[m_sCurSection].find(name)==SectionListDef[m_sCurSection].end())
  {
    value=readNumber(name,0,base);
    return false;
  }
  value=readNumber(name,SectionListDef[m_sCurSection][name].toInt(0,base),base);
  return true;
}


// reads bool number
bool TEIniFile::readBool(const QString &name, const bool def)
{
	bool ok;
	int d = readString(name).toInt( &ok );
  //if ((!ok || d<0 || d>1) && mi.data().contains(name)) // If value is invalid AND is in .ini file.
  //  cfg_message(2,"%s",(const char *)qApp->translate("Config","Config file: section [%1] value of %2 is supposed to be boolean (0 or 1)").arg(m_sCurSection).arg(name).utf8());
  SectionListDef[m_sCurSection][name]=qstrFromBool(def);
	return ok ? (bool)d : def;
}

bool TEIniFile::getBool(const QString & name, bool & value)
{
  if (SectionListDef[m_sCurSection].find(name)==SectionListDef[m_sCurSection].end())
  {
    value=readBool(name,false);
    return false;
  }
  value=readBool(name,SectionListDef[m_sCurSection][name].toInt()!=0);
  return true;
}

/*! Check if name exists in config file or as default value.
*/
bool TEIniFile::isNameExists(const QString &name)
{
  return mi.data().find(name)!=mi.data().end() || 
    SectionListDef[m_sCurSection].find(name)!=SectionListDef[m_sCurSection].end();
}

void TEIniFile::deleteName(const QString &name)
{
  mi.data().erase(name);
  SectionListDef[m_sCurSection].erase(name);
}

// reads color value
const QColor&	TEIniFile::readColor(const QString &name, const QColor& def)
{
	static QColor c;
	c.setNamedColor(readString(name));
  SectionListDef[m_sCurSection][name]=qstrFromColor(def);
  //if (!c.isValid() && mi.data().contains(name))
  //  cfg_message(2,"%s",(const char *)qApp->translate("Config","Config file: section [%1] value of %2 is supposed to be color (#XXXXXX)").arg(m_sCurSection).arg(name).utf8());
	return c.isValid() ? c : def;
}

// reads rectangle
const QRect& TEIniFile::readRect(const QString &name, const QRect& def)
{
	QString s = readString(name);
  SectionListDef[m_sCurSection][name]=qstrFromRect(def);
	if (s.isEmpty()) return (QRect&)def;
	QTextStream ts(&s, IO_ReadOnly);
	int x,y,w,h;
	ts >> x >> y >> w >> h;
	static QRect r(x,y,w,h);
	return r.isValid() ? r : def;
}

void TEIniFile::setString(const QString &name, const QString &value)
{
  if (mi==NULL || mi==SectionList.end()) // It checks if iterator is not initialized.
    return;
  mi.data()[name]=value;
}

void TEIniFile::setData(const QString &name, const QString &value)
{
  if (mi==NULL || mi==SectionList.end()) // It checks if iterator is not initialized.
    return;
  mi.data()[name]=qstrFromData(value);
}

void TEIniFile::setNumber(const QString &name, const int value, const int base)
{
  if (mi==NULL || mi==SectionList.end()) // It checks if iterator is not initialized.
    return;
  mi.data()[name]=qstrFromInt(value,base);
}

void	TEIniFile::setBool(const QString &name, const bool value)
{
  if (mi==NULL || mi==SectionList.end()) // It checks if iterator is not initialized.
    return;
  mi.data()[name]=qstrFromBool(value);
}

void	TEIniFile::setColor(const QString &name, const QColor& color)
{
  if (mi==NULL || mi==SectionList.end()) // It checks if iterator is not initialized.
    return;
  mi.data()[name]=qstrFromColor(color);
}

void	TEIniFile::setRect(const QString &name, const QRect& rect)
{
  if (mi==NULL || mi==SectionList.end()) // It checks if iterator is not initialized.
    return;
  mi.data()[name]=qstrFromRect(rect);
}

/*! This function updates .ini file in two stages. It copies original .ini file
  into temporary one line by line, replacing/deleting/adding lines as necessary.
  Then it copies temporary file into original one and deletes temporary file.
*/
bool TEIniFile::update()
{
  close();
	if (!f.open(IO_ReadWrite)) return false;
  QString tempfn=f.name()+".tmp";
  QFile tempf(tempfn);
  if (!tempf.open(IO_ReadWrite | IO_Truncate)) return false;
  QTextStream tsorg;
  bool skipsec=false;

  tsorg.setEncoding(QTextStream::UnicodeUTF8);
  tsorg.setDevice(&f);
	ts.setDevice(&tempf); // writeXXX function hardwired to write into ts. So we set it appropriatedly.
	// prepare
  QMap<QString,bool> secProcd, valueProcd; // processedp flag's maps
  QMapIterator<QString,type_ValueList> it1;
  for(it1=SectionList.begin();it1!=SectionList.end();++it1)
  {
    secProcd[it1.key()]=false;
  }
	// scan form sections
	QString line, sec;
	type_Processing pr = P_NONE;	// nothing done yet
	type_ValueList ValueList;

	while (!((line = tsorg.readLine()).isNull())) {
		if ((pr == P_NONE) || (pr == P_SECTION)) {
			// trim line
			line = line.stripWhiteSpace();
			// skip if empty
			if (line.isEmpty())
      {
        writeBreak();
        continue;
      }
			// skip if comment
			if (line.startsWith("#"))
      {
        writeComment(line.mid(1));
        continue;
      }
			// check if section
			if (line.startsWith("[") && line.endsWith("]")) {
				if (pr == P_SECTION) {
					// update previous section
          // write new values
          type_ValueList::iterator it;
          type_ValueList & curSec=SectionList[sec];
          type_ValueList & curSecDef=SectionListDef[sec];
          for(it=curSec.begin();it!=curSec.end();++it)
          {
            if (!valueProcd[it.key()])
            { // this value was not processed yet, hence it's new or just default.
              if (curSecDef.find(it.key())==curSecDef.end() || curSecDef[it.key()]!=it.data()) // if it equals default value, skip it.
                ts << it.key() << "\t" << it.data() << "\n";
              valueProcd[it.key()]=true; // it is for completeness' sake, so it don't really necessary (yet?).
            }
          }
					secProcd[sec]=true;
				}
				// section found!
        valueProcd.clear(); // get ready for next section
				pr = P_SECTION;
				sec = line.mid(1, line.length()-2).stripWhiteSpace();
        writeSection(sec);
        skipsec=SectionList.find(sec)==SectionList.end() || secProcd[sec]==true; // Skip deleted or already processed section
        type_ValueList & curSec=SectionList[sec];
        type_ValueList::iterator it;
        for(it=curSec.begin();it!=curSec.end();++it)
        {
          valueProcd[it.key()]=false;
        }
				ValueList.clear();
				continue;
			}
			if (pr == P_SECTION) {
				// read name
        if (skipsec)
          continue;
				QString nam;
				int j = 0, l = (int)line.length();
				QChar c;
				while ( (j < l) && !((c = line.ref((uint)j)).isSpace()) ) {
					nam += c;
					j++;
				}
				// read value
        bool rawData=false;
        bool doubleQuotes=false;
				QString val;
				val = line.mid((uint)j).stripWhiteSpace();
				if (val == "data{") {
          rawData=true;
					// read raw data...
					val = "";
					while (!((line = ts.readLine()).isNull())) {
						if (line == "}data") {
							ValueList[nam] = val;
							break;
						}
						val += line + "\n";
					}
				} else {
					// test for ""
					if (val.startsWith("\"") && val.endsWith("\""))
          {
            doubleQuotes=true;
						val = val.mid(1, val.length()-2).stripWhiteSpace();
          }
					ValueList[nam] = val;
				}
        if (SectionList[sec].find(nam)!=SectionList[sec].end() && valueProcd[nam]==false)
        { // write modified value
          if (rawData)
            writeData(nam,SectionList[sec][nam]);
          else if (doubleQuotes)
            writeString(nam,SectionList[sec][nam]);
          else
            ts << nam << "\t" << SectionList[sec][nam] << "\n";
          valueProcd[nam]=true;
        }
			}
      else
        writeComment(line);
		}
	}

	if (pr == P_SECTION) {
		// update last section
    // write new values
    type_ValueList::iterator it;
    type_ValueList & curSec=SectionList[sec];
    type_ValueList & curSecDef=SectionListDef[sec];
    for(it=curSec.begin();it!=curSec.end();++it)
    {
      if (!valueProcd[it.key()])
      { // this value was not processed yet, hence it's new.
        if (curSecDef.find(it.key())==curSecDef.end() || curSecDef[it.key()]!=it.data())
          ts << it.key() << "\t" << it.data() << "\n";
        valueProcd[it.key()]=true; // it is for completeness' sake, so it don't really necessary (yet?).
      }
    }
		secProcd[sec]=true;
	}
  QMapIterator<QString,bool> it;
  for(it=secProcd.begin();it!=secProcd.end();++it)
  {
    QString sec=it.key();
    if (!it.data())
    {
      writeSection(sec);
      type_ValueList & curSec=SectionList[sec];
      type_ValueList & curSecDef=SectionListDef[sec];
      type_ValueList::iterator it1;
      for(it1=curSec.begin();it1!=curSec.end();++it1)
        if (curSecDef.find(it1.key())==curSecDef.end() || curSecDef[it1.key()]!=it1.data())
          ts << it1.key() << "\t" << it1.data() << "\n";
    }
  }
  tempf.flush();
  if (!tempf.reset())
    return false;
  f.close();
  if (!f.open(IO_WriteOnly | IO_Truncate))
    return false;
  const int N=64*1024;
  char * buf=new char[N];
  Q_LONG len;
  while(true)
  {
    len=tempf.readBlock(buf,N);
    if (len<=0)
      break;
    f.writeBlock(buf,(Q_ULONG)len);
  }
  delete [] buf;
  f.close();
  tempf.close();
  tempf.remove();
  ts.setDevice(&f);
	return len>=0;
}

/*!
  Lists values used during initialization
*/
QStringList TEIniFile::usedValues()
{
  QStringList sl;
  QMapIterator<QString, type_ValueList> it;
  for(it=SectionListDef.begin();it!=SectionListDef.end();++it)
  {
    type_ValueListIterator it1;
    for(it1=(*it).begin();it1!=(*it).end();++it1)
      sl << it.key()+"->"+it1.key();
  }
  return sl;
}
