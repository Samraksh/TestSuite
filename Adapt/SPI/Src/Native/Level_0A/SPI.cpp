////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "SPI.h"

extern MSM8960_SPI_Driver MSM8960_SPI_Driver_obj;

/* This tests the SPI interface on Adapt. The test is successful if SPI initialization happens successfully. */

SPI::SPI()
{
	gsbi_id = GSBI_ID1;
	SPI_mod = CPOL_LOW_CPHA_LOW;
	IO_mode = FIFO_Mode;
	master_slave_mode = Master;
	clock_rate = HS_MODE;

	hal_printf("Calling constructor of SPI\n");

	MSM8960_SPI_Driver_obj.Initialize(gsbi_id, SPI_mod, IO_mode, master_slave_mode, clock_rate);
}

/*BOOL SPI::Initialize(int gsbi_id, int SPI_mode, int IO_mode, int master_slave_mode, int clock_rate)
{
	MSM8960_SPI_Driver_obj.Initialize(gsbi_id, SPI_mode, IO_mode, master_slave_mode, clock_rate);
	return TRUE;
}*/

BOOL SPI::DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy)
{
	hal_printf("\r\nresult=%s\r\n", (result) ? "PASS":"FAIL");
	hal_printf("\r\naccuracy=%d\r\n", accuracy);
	hal_printf("\r\nresultParameter1=%s\r\n", resultParameter1);
	hal_printf("\r\nresultParameter2=%s\r\n", resultParameter2);
	hal_printf("\r\nresultParameter3=%s\r\n", "null");
	hal_printf("\r\nresultParameter4=%s\r\n", "null");
	hal_printf("\r\nresultParameter5=%s\r\n", "null");

	return TRUE;
}

/* This test toggles a GPIO pin high and then low */
BOOL SPI::Level_0A()
{
	CPU_SPI_Xaction_Start(config);
	int data = 1;
	CPU_SPI_WriteByte(config, data);
	CPU_SPI_Xaction_Stop(config);

	for(int i = 0; i < 100000; ++i);
	for(int i = 0; i < 100000; ++i);


	CPU_SPI_Xaction_Start(config);
	data = 0;
	CPU_SPI_WriteByte(config, data);
	CPU_SPI_Xaction_Stop(config);

	return TRUE;
}



BOOL SPI::Execute( int testLevel )
{
	BOOL result;

	switch(testLevel)
	{
	case 0:
		result = Level_0A();
		break;
	default:
		break;
	}

}



