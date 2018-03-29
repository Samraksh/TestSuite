#ifndef _HMACTEST_H_
#define _HMACTEST_H_

#include <PKCS11\CryptokiPAL.h>
#include <tinyhal.h>

class HmacTest
{

public:

//--//
	HmacTest ( int seedValue, int numberOfEvents );

    BOOL     Execute( int testLevel );

	BOOL 	 Level_0A();

	BOOL 	 Level_0E();

};


#endif //_HMACTEST_H_
