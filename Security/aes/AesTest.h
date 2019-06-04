#ifndef _AESTEST_H_
#define _AESTEST_H_

#include <PKCS11\CryptokiPAL.h>
#include <tinyhal.h>
#include <crypto.h>

const int hmacSize=32;

const CK_BYTE key1[hmacSize] = {
		0xC6, 0x29, 0x73, 0xE3, 0xC8, 0xD4, 0xFC, 0xB6,
        0x89, 0x36, 0x46, 0xF9, 0x58, 0xE5, 0xF5, 0xE5,
        0x25, 0xC2, 0xE4, 0x1E, 0xCC, 0xA8, 0xC3, 0xEF,
        0xA2, 0x8D, 0x24, 0xDE, 0xFD, 0x19, 0xDA, 0x08
};

const CK_BYTE hmac1[hmacSize] = {
		0, 103, 74, 155, 97, 125, 27, 130,
		83, 111, 216, 226, 156, 45, 100, 50,
		59, 61, 228, 144, 127, 39, 150, 29,
		253, 74, 92, 188, 247, 200, 88, 195
};

class AesTest
{
	CK_BYTE data[128];
	CK_BYTE digest[32];
	CK_BYTE IV[48];
	CK_BYTE_PTR  pData;
	CK_ULONG ulDataLen;
	CK_BYTE  pCryptText[128];
	CK_ULONG ulCryptLen;
	CK_BYTE_PTR pDigest;
	CK_MECHANISM_TYPE mtype;
	CK_KEY_TYPE kt;
	CK_BYTE_PTR pkey;
public:

//--//
	AesTest ( int seedValue, int numberOfEvents );

    BOOL     Execute( int testLevel );

	BOOL 	 Level_0();

	BOOL 	 Level_1();

};




#endif //_AesTest_H_
