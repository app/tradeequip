#include "teglobal.h"
#include <tebase.h>
#include <tefactory.h>
#include <md5.h>

/*!
\en
\brief Function return list of avalable drivers.

\_en \ru
\brief .

\_ru
*/
QStringList
te_drivers( const QString &baseClass )
{
    QStringList l, li;

    li = TEFactory::keys();
    if (baseClass.isNull()) return li;
    for ( unsigned int i=0; i<li.size(); i++ ){
	if ( te_driverExists( li[i], baseClass ) ) l << li[i];
    }
    return l;
}



/*!
\en
\brief Function check for existens driver className, inherits baseClass.

\_en \ru
\brief .

\_ru
*/
bool
te_driverExists( const QString &className, const QString &baseClass )
{
	bool rc = false;
	TEBase *d = te_createDriverObject( className );
	if ( d ) {
	    if ( !baseClass.isEmpty() ) {
		if ( d->inherits( baseClass ) ) rc = true;
	    }
	    else rc = true;
	}
	delete d;
	return rc;
}



/*!
\en
\brief Function creates driver object by className.

\_en \ru
\brief .

\_ru
*/
TEBase *
te_createDriverObject( const QString &className )
{
    return TEFactory::create( className );
}


/*!
\en
\brief Function calculate MD5 summ of the byte array.

\_en \ru
\brief .

\_ru
*/
QString
te_MD5( const QByteArray &data )
{
	QString md5digest = "", dh;
	md5_state_t state;
	md5_byte_t digest[16];
	char hex_output[16*2 + 1];
	int di;

	md5_init(&state);
	if (!data.isNull()) {
		md5_append(&state, (const md5_byte_t *) data.data(), (int)data.size());
	}
	md5_finish(&state, digest);
	for (di = 0; di < 16; ++di)
	    sprintf(hex_output + di * 2, "%02x", digest[di]);
	md5digest = hex_output;
//	printf(">%s %s\n", ( const char *) data.data(), hex_output );
	return md5digest;
}



/*!
\en
\brief Function calculate MD5 summ of the QString.

\_en \ru
\brief .

\_ru
*/
QString
te_MD5( const QString &data )
{
    QByteArray bdata = data.utf8();
    bdata.truncate( bdata.size() -1 );

    return te_MD5( bdata );
}


/* Run the MD5 functions self-test. */
int
te_MD5_selftest(void)
{
    static const char *const test[7*2] = {
	"", "d41d8cd98f00b204e9800998ecf8427e",
	"a", "0cc175b9c0f1b6a831c399e269772661",
	"abc", "900150983cd24fb0d6963f7d28e17f72",
	"message digest", "f96b697d7cb7938d525a2f31aaf161d0",
	"abcdefghijklmnopqrstuvwxyz", "c3fcd3d76192e4007dfb496cca67e13b",
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
				"d174ab98d277d9f5a5611c2c9f419d9f",
	"12345678901234567890123456789012345678901234567890123456789012345678901234567890", "57edf4a22be3c955ac49da2e2107b67a"
    };
    int i;
    int status = 0;
    QString md5;

    for (i = 0; i < 7*2; i += 2) {
	md5 = te_MD5( QString(test[i]) );
	if (  md5 != QString(test[i + 1])) {
	    printf("MD5 (\"%s\") = ", test[i]);
	    puts( ( const char *) md5 );
	    printf("**** ERROR, should be: %s\n", test[i + 1]);
	    status = 1;
	}
    }
//    if (status == 0)
//	puts("md5 self-test completed successfully.");
    return status;
}
