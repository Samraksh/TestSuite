/*
 * InheritanceTestObjectt.cpp
 *
 *  Created on: Sep 11, 2017
 *      Author: Bora
 */

#include "InheritanceTestObjectt.h"

namespace InheritanceTest{

Base1::Base1(){ hal_printf("Base1 constructor\r\n"); }
Base1::~Base1(){ hal_printf("destroying base\r\n"); }
void Base1::callfoo_virt1(){foo_virt1();}
//void Base1::foo_virt1(){ hal_printf("base1 foo_virt1\r\n"); }

Der1::Der1(){ hal_printf("Der1 constructor\r\n"); }
Der1::~Der1(){ hal_printf("destroying der1\r\n"); }
void Der1::foo_virt1() { hal_printf("Der1 foo virt1\r\n"); }
void Der1::foo_virt2(){hal_printf("Der1::foo_virt2 \r\n");}
void Der1::Run(){ callfoo_virt1(); }

InheritanceTestObject_t::InheritanceTestObject_t(){ hal_printf("InheritanceTestObject_t constructor\r\n"); }
void InheritanceTestObject_t::foo_virt1(){ hal_printf("InheritanceTestObject_t::foo_virt1 \r\n"); }
InheritanceTestObject_t::~InheritanceTestObject_t(){ hal_printf("destroying InheritanceTestObject_t\r\n"); }

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

//	p->Base1::foo_virt1();
	for(UINT8 i = 0; i < limit; ++i){}

	gInheritanceTest.foo_virt1();
	for(UINT8 i = 0; i < limit; ++i){}

//	gInheritanceTest.Run(); //BK: Segfaults in this line
}


} //End namespace InheritanceTest

