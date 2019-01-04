#ifndef _HMACTEST_H_
#define _RAMEXECTEST_H_

#include <PKCS11\CryptokiPAL.h>
#include <tinyhal.h>


class RamExecTest
{
	char largeArray[512];
public:

//--//
	RamExecTest ( );
    BOOL   Execute();

};




#endif //_RAMEXECTEST_H_
