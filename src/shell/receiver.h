#include <qobject.h>
//#include <tebase.h>
#include <iostream>

using namespace std;

class Receiver : public QObject
{
    Q_OBJECT
public:
    Receiver()
    {};
public slots:
    void onDeviceEvent(int iCode)
    {
//        if (iCode!=EVENT_BUSY)
//        {
            cerr << "event " << iCode << endl;
//        };
    };
};

