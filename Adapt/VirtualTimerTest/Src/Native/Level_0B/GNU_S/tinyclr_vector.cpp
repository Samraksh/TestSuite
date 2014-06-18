//
// The Samrkash Company
//
// Author : Nived.Sivadas@samraksh.com
//
//


#include <D:\AnanthAtSamraksh\MF\MicroFrameworkPK_v4_3\DeviceCode\Targets\Native\Krait\DeviceCode\Krait.h>
#define VectorTableOffsetRegister 0xE000ED08

#define OFFSET 0x20000

extern "C"
{

void VectorRelocate()
{
	*(volatile uint32_t *) VectorTableOffsetRegister |= (uint32_t) 0;

}
}
