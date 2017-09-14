/*
 * InheritanceTestObjectt.h
 *
 *  Created on: Sep 11, 2017
 *      Author: Bora
 */

#ifndef MICROFRAMEWORKPK_V4_3_SOLUTIONS_EMOTEDOTNOW_TINYCLR_INHERITANCETESTOBJECTT_H_
#define MICROFRAMEWORKPK_V4_3_SOLUTIONS_EMOTEDOTNOW_TINYCLR_INHERITANCETESTOBJECTT_H_

#include <tinyhal.h>

//
extern "C" void __cxa_pure_virtual() { while (true); }
//void* __dso_handle;
void operator delete(void *, unsigned int)
{
	while (true);
}

namespace InheritanceTest{
//namespace A{
class Base1{
public:
	Base1();
	virtual ~Base1();
	void callfoo_virt1();
	virtual void foo_virt1() = 0;
};

class Base2{
public:
	Base2();
	virtual ~Base2();
	void callfoo_virt2();
	virtual void foo_virt2() = 0;
};


class Der1 : public Base1, public Base2 {
public:
	Der1();
	virtual ~Der1();
	virtual void foo_virt1();
	void foo_virt2();
	void Run();
};

class InheritanceTestObject_t : public Der1{
public:
	InheritanceTestObject_t();
	void foo_virt1();
	~InheritanceTestObject_t();
};


void InheritanceTestTest_InitializeAndRun();

void InheritanceTest_InitializeGlobalPointers();

}//End namespace InheritanceTest

#endif /* MICROFRAMEWORKPK_V4_3_SOLUTIONS_EMOTEDOTNOW_TINYCLR_INHERITANCETESTOBJECTT_H_ */
