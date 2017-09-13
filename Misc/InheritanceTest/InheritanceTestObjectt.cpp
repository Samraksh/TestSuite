/*
 * InheritanceTestObjectt.cpp
 *
 *  Created on: Sep 11, 2017
 *      Author: Bora
 */

#include "InheritanceTestObjectt.h"

namespace InheritanceTest{

InheritanceTestObject_t* gInheritanceTestPtr;
void InheritanceTestTest_InitializeAndRun() {
	
	static InheritanceTestObject_t gInheritanceTest;
	gInheritanceTestPtr = &gInheritanceTest;

	gInheritanceTestPtr->Run();
	gInheritanceTestPtr->callfoo_virt1();
	gInheritanceTestPtr->foo_virt1();
}


} //End namespace InheritanceTest

