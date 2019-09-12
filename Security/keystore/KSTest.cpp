#include "KSTest.h"
#include <Keystore.h>

CK_BYTE keysRB[noOfKeys][keySize];

void PrintHex(const CK_BYTE* sig, int size){
	for (int j=0;j<size; j++){
		hal_printf("0x%.2X , ",sig[j]);
	}
	hal_printf("\n");
}

bool storeKeys(){
	hal_printf("==========Storing keys into KS=============\n\n");
	for(int i=0; i< noOfKeys; i++){
		uint8_t ret=KeyStore_StoreKey(UT_App, (EKeySlot)(K_UserStart+1+i), constKeys[i], keySize, keysRB[i]);
		if(ret!=MSS_SYS_SUCCESS){
			hal_printf("Storing Key %d failed, return was %d \n\n", K_UserStart+1+i, ret);
			//return false;
		}

		uint8_t keysEnrolled=0;
		ret=MSS_SYS_puf_get_number_of_keys(&keysEnrolled);
		hal_printf("Number of keys enrolled is: %d \n\n", keysEnrolled);
	}
	return true;
}

bool ReadandTest(){
	hal_printf("==========Reading keys back from KS=============\n\n");
	for(int i=0; i< noOfKeys; i++){
		CK_BYTE* keyPtr;
		uint8_t ret=KeyStore_ReadKey(UT_App, (EKeySlot)(K_UserStart+1+i), &keyPtr);
		if(ret!=MSS_SYS_SUCCESS){
			hal_printf("Reading Key %d failed, return was %d \n\n", K_UserStart+1+i, ret);
			return false;
		}

		int eq=memcmp(constKeys[i],keysRB[i],keySize);
		if(eq==0) {
			hal_printf("Reading back Key %d Success. Read back...\n", i);
			PrintHex(keysRB[i],keySize);
		}else {
			hal_printf("Storing Key %d failed \n\n", i);
			return false;
		}

	}
	return true;
}

bool InitKeyStore(){

	hal_printf("============Reseting Key Store, all previous keys are wipped============ \n\n");
	uint8_t ret=MSS_SYS_puf_create_activation_code();
	if(ret!=MSS_SYS_SUCCESS){
		hal_printf("Creating a new activatio code failed, return was %d \n\n", ret);
		return false;
	}

	uint8_t keysEnrolled;
	ret=MSS_SYS_puf_get_number_of_keys(&keysEnrolled);
	if(ret!=MSS_SYS_SUCCESS){
		hal_printf("Getting number of enrolled keys failed, return was %d \n\n", ret);
		return false;
	}
	hal_printf("Number of keys enrolled is: %d \n\n", keysEnrolled);
}

KSTest::KSTest( int seedValue, int numberOfEvents )
{
	hal_printf("============Initializing Key Store Test============ \n\n");

	memset(keysRB,0,noOfKeys*keySize);
};


BOOL KSTest::Level_0()
{
	bool ret=FALSE;
	hal_printf("Original Key 0: ");
	PrintHex(constKeys[0],keySize);

	//InitKeyStore();

	//ret=ReadandTest();

	ret= storeKeys();
	if(!ret){hal_printf("KSTest::Level_0:: Storing Failed\n"); return FALSE;}

	ret=ReadandTest();
	if(!ret){hal_printf("KSTest::Level_0:: Reading Failed\n"); return FALSE;}

	hal_printf("\n\n  ");

	hal_printf("============Key Store Test Success============ \n");
	return TRUE;
}

BOOL KSTest::Level_1()
{
	return TRUE;
}


BOOL KSTest::Execute( int testLevel )
{
	if(testLevel == 0)
		return Level_0();
	else if(testLevel == 1)
		return Level_1();
} //Execute

void ApplicationEntryPoint()
{
    BOOL result;

    KSTest test(0,0);

    do
    {
    	if(!test.Execute(0))
    		hal_printf("Error in Key Store Test.\n\n");
    	else
    		hal_printf("Key Store Test is Super Success! \n\n");

		hal_printf("\n\n  ");
		hal_printf("\n\n  ");

    } while(FALSE); // run only once!

    while(TRUE){
		::Events_WaitForEvents( 0, 100 );
    }

}
