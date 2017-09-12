/*
 * InheritanceTestObjectt.cpp
 *
 *  Created on: Sep 11, 2017
 *      Author: Bora
 */

#include "InheritanceTestObjectt.h"

namespace InheritanceTest{


InheritanceTestObject_t gInheritanceTest;	

void InheritanceTestTest_InitializeAndRun() {

	Der1 der1;
	der1.Run();

	gInheritanceTest.foo_virt1();
//	gInheritanceTest.Run();
}


} //End namespace InheritanceTest

