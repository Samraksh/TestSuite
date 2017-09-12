/*
 * InheritanceTestObjectt.cpp
 *
 *  Created on: Sep 11, 2017
 *      Author: Bora
 */

#include "InheritanceTestObjectt.h"

namespace InheritanceTest{


InheritanceTestObject_t gInheritanceTest;	
Der1 gder1;

void InheritanceTestTest_InitializeAndRun() {
	UINT8 limit = 0;
	limit = 10;

	Der1 der1;
	for(UINT8 i = 0; i < limit; ++i){}

	der1.Run();
	for(UINT8 i = 0; i < limit; ++i){}

	der1.callfoo_virt1();
	for(UINT8 i = 0; i < limit; ++i){}

	Base1* p = &der1;
	p->callfoo_virt1();
	for(UINT8 i = 0; i < limit; ++i){}

	p->foo_virt1();
	for(UINT8 i = 0; i < limit; ++i){}

	p->Base1::foo_virt1();
	for(UINT8 i = 0; i < limit; ++i){}

	gInheritanceTest.foo_virt1();
	for(UINT8 i = 0; i < limit; ++i){}

	gInheritanceTest.Run();
}


} //End namespace InheritanceTest

