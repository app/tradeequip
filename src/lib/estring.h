#ifndef _estring_h_
#define _estring_h_

#include <qstring.h>
#include "teglobal.h"

enum SEffect
{
	se_scrollleft,
	se_scrollright,
	se_wobble
};

QString deControl(const QString & sin);

class LIB_EXPORT EString
{
public:
	EString():wwidth(20),pos(0),dir(1),delay(4){};
	EString(QString _text, int windowwidth):wwidth((uint)windowwidth),pos(0),dir(1),delay(4)
	{
		text=deControl(_text);
	};
	EString(const EString & in):text(in.text),wwidth(in.wwidth),pos(0),dir(1),delay(4){};
	~EString(){};


	QString update();
	QString current();
	void setEffect(SEffect _eff);
	uint width()
	{
		return wwidth;
	};
protected:
	QString text,_current;
	uint wwidth;
	int pos,dir,delay;
	SEffect eff;
};

#endif
