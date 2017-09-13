////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) The Samraksh Company.  All rights reserved.
// This is an auto generated file by the test create tool
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define __regtest

#include <tinyhal.h>

#include "VirtualTimerTest.h"

void call_f1_external(void);

class B1 {
public:
	B1() {hal_printf("B1 constructor\r\n"); my_int = 45;}
	~B1() {hal_printf("B1 destroyed\r\n");}
	void f0() { hal_printf("B1 f0 %d\r\n", my_int); }
	void call_f1(){ call_f1_external(); hal_printf("call_f1 external %d\r\n", my_int); }
	virtual void f1() { hal_printf("B1 f1 %d\r\n", my_int); }
	int my_int;
};

class B2 : public B1 {
public:
	B2() {hal_printf("B2 constructor\r\n");}
	~B2() {hal_printf("B2 destroyed\r\n");}
	virtual void f1() { hal_printf("B2 f1\r\n"); }
	void Run() { call_f1_external(); }
};

class D : public B2 {
public:
	D() {hal_printf("D constructor\r\n");}
	~D() {hal_printf("D destroyed\r\n");}
	void f1() { hal_printf("D f1\r\n"); }
};


void Keep_Linker_Happy() {
	BootstrapCode();
	BootEntry();
}


HAL_DECLARE_NULL_HEAP();

void PostManagedEvent( UINT8 category, UINT8 subCategory, UINT16 data1, UINT32 data2 )
{
}

D dGlobal;
void call_f1_external(void) {
	dGlobal.f1();
}


void test(void){
	dGlobal.f0();
	dGlobal.f1();
}
void test2(void){
	dGlobal.Run();
	dGlobal.call_f1();
}
void ApplicationEntryPoint()
{
    BOOL result;

	test();
	test2();
	
    VirtualTimerTest virtualTimerTest(0,0);

    do
    {

    	virtualTimerTest.Execute(0);
	  //if(halTimerTest.Execute(0))
		//break;
    } while(FALSE); // run only once!

    while(TRUE){
		::Events_WaitForEvents( 0, 100 );
	}
}
