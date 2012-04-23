#include "estring.h"

/*!
 *
 */
QString EString::update()
{
	switch(eff)
	{
	case se_wobble:
		{
			if (text.length()<=wwidth)
				_current=text.leftJustify(wwidth);
			else
			{
				if (delay--<0)
				{
					pos+=dir;
					if (pos==0 && dir<0)
					{
						delay=4;
						dir=-dir;
					}
					if (pos==(int)text.length()-(int)wwidth && dir>0)
					{
						delay=4;
						dir=-dir;
					}
				}
				_current=text.mid((uint)pos,wwidth);
			}
		};
		break;
	case se_scrollleft:
		{
			if (text.length()<=wwidth)
				_current=text.leftJustify(wwidth);
			else
			{
				pos++;
				if (pos>(int)text.length())
					pos=0;
				_current=(text+" "+text).mid((uint)pos,wwidth);
			}
		};
		break;
	case se_scrollright:
		{
			if (text.length()<=wwidth)
				_current=text.leftJustify(wwidth);
			else
			{
				pos--;
				if (pos<0)
					pos=(int)text.length();
				_current=(text+" "+text).mid((uint)pos,wwidth);
			}
		};
		break;
	default:
		_current=text.left(wwidth).leftJustify(wwidth);
		break;

	};
	return _current;
};

/*!
 *
 */
QString EString::current()
{
	return _current;
};

/*!
 *
 */
void EString::setEffect(SEffect _eff)
{
	eff=_eff;
};

QString deControl(const QString & sin)
{
	QString ret="";
	for(uint i=0;i<sin.length();i++)
		if (sin.at(i).isPrint())
			ret+=sin.at(i);
		else if (sin.at(i).isSpace())
			ret+=' ';
	return ret;
};
