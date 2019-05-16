#include <crypto.h>

#include "AesTest.h"
//#include "platform_selector.h"

void PrintHex(CK_BYTE_PTR sig, int size){
	for (int j=0;j<size; j++){
		hal_printf("0x%.2X , ",sig[j]);
	}
	hal_printf("\n");
}

AesTest::AesTest( int seedValue, int numberOfEvents )
{
	memset(pDigest,0,hmacSize);
	memcpy(data,"Samraksh eMote Cryptoki HMAC Example; Plus the wolf is great, but the fox is grey. The lamb is prey, but its a mountain pro!",124);
	pData=data;
	ulDataLen=124;
	ulCryptLen=128;
	GetRGetRandomBytes(IV, 48);
	pDigest=digest;
	mtype=CKM_SHA256_HMAC;
	pkey=(CK_BYTE_PTR)key1;
	kt= CKK_GENERIC_SECRET;
	//CKM_GENERIC_SECRET_KEY_GEN;
	//	CK_KEY_TYPE kt;
	//	CK_BYTE_PTR key;
};

BOOL AesTest::Level_0()
{
	//CK_BYTE_PTR  pData, CK_ULONG ulDataLen, CK_BYTE_PTR pDigest, CK_MECHANISM_TYPE mtype, CK_KEY_TYPE kt, CK_BYTE_PTR key
	//bool ret= ComputeHMAC(pData, ulDataLen,pDigest,mtype,kt,pkey,32);
	hal_printf("Original Text: ");PrintHex(pData,ulDataLen);
	boot ret= Crypto_Encrypt(pkey,32,IV, 48, pData, ulDataLen, pCryptText, ulCryptLen);
	hal_printf("Encrypted Text: ");PrintHex(pCryptText,ulCryptLen);
	boot ret= Crypto_Decrypt(pkey,32,IV, 48, pCryptText, ulCryptLen, pData, 128);
	hal_printf("Decrypted Text: ");PrintHex(pData,ulDataLen);
	hal_printf("\n\n  ");


	if(ret){
		int eq=memcmp(pDigest,hmac1,hmacSize);
		if(eq==0) return TRUE;
	}
	return FALSE;
}

BOOL AesTest::Level_1()
{
	return TRUE;
}


BOOL AesTest::Execute( int testLevel )
{
	if(testLevel == 0)
		return Level_0();
	else if(testLevel == 1)
		return Level_1();
} //Execute

void ApplicationEntryPoint()
{
    BOOL result;

    AesTest test(0,0);

    do
    {
    	if(!test.Execute(0))
    		hal_printf("Error in HMAC Test.");
    	else
    		hal_printf("HMAC Test is Super Success! ");

    } while(FALSE); // run only once!

    while(TRUE){
		::Events_WaitForEvents( 0, 100 );
    }

}
