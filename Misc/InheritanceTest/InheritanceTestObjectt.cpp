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

	gInheritanceTest.Run();
	gInheritanceTest.foo_virt1();
}


} //End namespace InheritanceTest

