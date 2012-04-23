#include <cmdparser.h>
#include <iostream>
#include <qstring.h>
#include <qvaluevector.h>
#include <qvariant.h>
#include <tebase.h>
#include <qstringlist.h>
#include <qobject.h>
#include <qapplication.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qthread.h>
#include <qeventloop.h>
#include <qmutex.h>
#include <qptrqueue.h>
#include <qwaitcondition.h>
#include <qevent.h>
#include <string>
#include "receiver.h"
#include <tefactory.h>

#ifdef DEBUG
#  define debprintf printf
#else
#  define debprintf
#endif

using namespace std;

QStringList g_buf;
volatile bool g_cmdavail;
QMutex g_bufmut;
Receiver rcv;

string quote(const char * str)
{
	string ret;
	const char * pc=str;
	if (!str)
		return ret;
	while(*pc)
	{
		if (*pc=='\\')
			ret+="\\\\";
		else if (*pc=='"')
			ret+="\\\"";
		else
			ret+=*pc;
		pc++;
	}
    return ret;
}

class MyThread : public QThread 
{
public:
    MyThread(){};
    virtual void run()
    {
		bool bTerminate=false;
        while (true)
        {
            QTextCodec * ibm866=QTextCodec::codecForName("IBM866");
            char buf[1024];
            cin.getline(buf,1023);
            g_bufmut.lock();
            if (cin.fail() || cin.eof())
			{
                g_buf.push_back("exit");
				bTerminate=true;
			}
            else
			{
                g_buf.push_back(ibm866->toUnicode(buf));
				if (strcmp(buf,"exit")==0)
					bTerminate=true;
			}
            g_bufmut.unlock();
            g_cmdavail=true;
            QApplication::postEvent(&rcv,new QCustomEvent(5434));
			if (bTerminate)
				break;
        };
	debprintf("Thread terminated itsels...\n");
    };
};

bool createObject(TEBase *&treq, const QString & name)
{
	delete treq;
	treq=TEFactory::create(name);
	if (treq)
	    rcv.connect(treq,SIGNAL(deviceEvent(int)),&rcv,SLOT(onDeviceEvent(int)));
	return treq!=0;
}

int main(int argc, char *argv[])
{
    QApplication app(argc,argv,false);
    QTextCodec * ibm866=QTextCodec::codecForName("IBM866");
    TEBase *treq = 0;

#ifdef _Windows
	qApp->addLibraryPath( qApp->applicationDirPath() );
#else
	qApp->addLibraryPath( "/usr/lib/" );
#endif
	if (te_MD5_selftest()) {
	    puts("md5 self-test error.");
	    return 1;
	}
	printf("drivers avalable: \n%s\n",( const char *) TEFactory::keys().join("\n") );	
	printf("Enter driver name or 'list'\n");
    MyThread thr;
    thr.start();

    while(true)
    {
        g_bufmut.lock();
//        app.eventLoop()->processEvents(QEventLoop::AllEvents);
        while (g_cmdavail)
        {
	    debprintf("command available\n");
            QString cmd=g_buf.front();
            g_buf.pop_front();
            g_cmdavail=!g_buf.empty();
            if (cmd==QString("exit"))
            {
				g_bufmut.unlock();
				debprintf("after g_bufmut.unlock() before stopPoll\n");
//				if (treq) treq->stopPoll();
				debprintf("after stopPoll() before close\n");
				//treq.setModeInt(0);
//				if (treq) treq->close();
				debprintf("after close before thr.running()\n");
				if (thr.running())
				{
					debprintf("after thr.running before thr.terminate()\n");
					thr.terminate();
					debprintf("after thr.terminate() before thr.wait()\n");
					thr.wait();
				}
				debprintf("after thr.running() before return 0\n");
    		        goto exitcommand;
            };
//			try
			{
				if (treq)
				{
					if (cmd==QString("close"))
					{
						delete treq;
						treq=0;
						printf("Object was destroyed.\n");
					}
					else
					{
				    	debprintf("before treq->execCommand(\"%s\")\n",cmd.ascii());
						QStringList ll=treq->execCommand(cmd);
						for(uint i=0;i<ll.size();i++)
							cout << (ibm866->fromUnicode(ll[i])).data() << endl;
					}
				}
				else
				{
					if (cmd==QString("list"))
					{
						printf("drivers avalable: \n%s\n",( const char *) TEFactory::keys().join("\n") );	
						printf("Enter driver name or 'list'\n");
					}
					else if (createObject(treq,cmd))
					{
						printf("OK. Enter 'eeHelp()' for object's commands, or 'close' to destroy object\n");
					}
					else
						printf("Object creation failed.\n");
				}
			}
//			catch(exception & ex)
//			{
//				cout << "100;" << quote(ex.what()) << endl;
//			}
        };
        g_bufmut.unlock();
        app.eventLoop()->processEvents(QEventLoop::AllEvents | QEventLoop::WaitForMore);
    };
exitcommand:
//    debprintf("before disconnect.\n");
//    rcv.disconnect(treq,SIGNAL(deviceEvent(int)),&rcv,SLOT(onDeviceEvent(int)));
//    debprintf("after disconnect.\n");
    delete treq;
    return 0;
};
