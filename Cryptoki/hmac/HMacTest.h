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

	BOOL 	 Level_0();

	BOOL 	 Level_1();

};


#endif //_HMACTEST_H_
