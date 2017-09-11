/*
 * InheritanceTestObjectt.cpp
 *
 *  Created on: Sep 11, 2017
 *      Author: Bora
 */

#include "InheritanceTestObjectt.h"

namespace InheritanceTest{

InheritanceTestObject_t gInheritanceTest;

InheritanceTestObject_t::InheritanceTestObject_t() {
	// TODO Auto-generated constructor stub

}

InheritanceTestObject_t::~InheritanceTestObject_t() {
	// TODO Auto-generated destructor stub
}

void InheritanceTestTest_InitializeAndRun() {
	gInheritanceTest.foo_virt1();

	gInheritanceTest.Run();

}


} //End namespace InheritanceTest

InheritanceTest::Der1::~Der1() {
}

void InheritanceTest::Der1::Run() {
	callfoo_virt1();
}

void InheritanceTest::Der1::foo_virt1() {
	hal_printf("Der1::foo_virt1 \r\n");
}

void InheritanceTest::Der1::foo_virt2()  {
	hal_printf("Der1::foo_virt2 \r\n");
}



//void A::Base1::callfoo_virt1() {
//	this->foo_virt1();
//}

A::Base1::~Base1() {
}

void A::Base1::foo_virt1(){
	hal_printf("ABase1::foo_virt1 \r\n");
};

;
