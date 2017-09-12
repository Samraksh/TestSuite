/*
 * InheritanceTestObjectt.h
 *
 *  Created on: Sep 11, 2017
 *      Author: Bora
 */

#ifndef MICROFRAMEWORKPK_V4_3_SOLUTIONS_EMOTEDOTNOW_TINYCLR_INHERITANCETESTOBJECTT_H_
#define MICROFRAMEWORKPK_V4_3_SOLUTIONS_EMOTEDOTNOW_TINYCLR_INHERITANCETESTOBJECTT_H_

#include <tinyhal.h>

namespace InheritanceTest{
//namespace A{
class Base1{
public:
	Base1() { hal_printf("Base1 constructor\r\n"); }
	~Base1(){ hal_printf("destroying base\r\n"); }
	void callfoo_virt1(){foo_virt1();}
	virtual void foo_virt1() = 0; //{ hal_printf("base1 foo_virt1\r\n"); }
};

class Der1 : public Base1 {
public:
	Der1(){ hal_printf("Der1 constructor\r\n"); }
	~Der1(){ hal_printf("destroying der1\r\n"); }
	virtual void foo_virt1() { hal_printf("Der1 foo virt1\r\n"); }
	void foo_virt2(){hal_printf("Der1::foo_virt2 \r\n");}
	void Run(){ callfoo_virt1(); }
};

class InheritanceTestObject_t : public Der1{
public:
	InheritanceTestObject_t(){ hal_printf("InheritanceTestObject_t constructor\r\n"); }
	void foo_virt1(){ hal_printf("InheritanceTestObject_t::foo_virt1 \r\n"); }
	~InheritanceTestObject_t(){ hal_printf("destroying InheritanceTestObject_t\r\n"); }
};


void InheritanceTestTest_InitializeAndRun();

}//End namespace InheritanceTest

#endif /* MICROFRAMEWORKPK_V4_3_SOLUTIONS_EMOTEDOTNOW_TINYCLR_INHERITANCETESTOBJECTT_H_ */
