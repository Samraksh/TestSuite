/*
 * InheritanceTestObjectt.h
 *
 *  Created on: Sep 11, 2017
 *      Author: Bora
 */

#ifndef MICROFRAMEWORKPK_V4_3_SOLUTIONS_EMOTEDOTNOW_TINYCLR_INHERITANCETESTOBJECTT_H_
#define MICROFRAMEWORKPK_V4_3_SOLUTIONS_EMOTEDOTNOW_TINYCLR_INHERITANCETESTOBJECTT_H_

#include <tinyhal.h>


namespace A{
class Base1{
public:
	virtual ~Base1();
	void callfoo_virt1(){
		foo_virt1();
	}
	virtual void foo_virt1();
};
};


namespace InheritanceTest{

class Der1 : public A::Base1 {
public:
	virtual ~Der1();
	virtual void foo_virt1() ;
	void foo_virt2();
	void Run();
};

class InheritanceTestObject_t : public Der1{
public:
	InheritanceTestObject_t();
	virtual ~InheritanceTestObject_t();

};

void InheritanceTestTest_InitializeAndRun();

}//End namespace InheritanceTest

#endif /* MICROFRAMEWORKPK_V4_3_SOLUTIONS_EMOTEDOTNOW_TINYCLR_INHERITANCETESTOBJECTT_H_ */
