
#include "OMACTest.h"
#include "OMACTestTimeSync.h"


void OMACTest_Initialize(){
	gOMACTestBaseptr = &gOMACTestTimeSync;
	BOOL ret;
	ret = ((OMACTestTimeSync*)gOMACTestBaseptr)->Initialize();
	ret = ((OMACTestTimeSync*)gOMACTestBaseptr)->StartTest();
}

