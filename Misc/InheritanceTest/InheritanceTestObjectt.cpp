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

	gInheritanceTestPtr->Run();
	gInheritanceTestPtr->callfoo_virt1();
	gInheritanceTestPtr->foo_virt1();
}

void InheritanceTest_InitializeGlobalPointers(){
	static InheritanceTestObject_t gInheritanceTest;
	gInheritanceTestPtr = &gInheritanceTest;
}

} //End namespace InheritanceTest

