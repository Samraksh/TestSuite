////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "..\DeviceCode\Targets\Native\Krait\DeviceCode\Krait_SPI\Krait_SPI.h"

#define LEVEL_0A	0

enum GSBI_IDs
{
	GSBI_ID1 = 1,
	GSBI_ID2,
	GSBI_ID3,
	GSBI_ID4,
	GSBI_ID5,
	GSBI_ID6,
	GSBI_ID7,
	GSBI_ID8,
	GSBI_ID9,
	GSBI_ID10
};

/*enum IO_Modes
{
	FIFO_Mode,
	Block_Mode,
	Data_Mover_Mode
};

enum SPI_Clock_rate
{
	Non_HS_MODE,
	HS_MODE
};

enum master_slave_mode
{
	Master,
	Slave
};

enum SPI_CPOL_CPHA_mode
{
	CPOL_LOW_CPHA_LOW,
	CPOL_LOW_CPHA_HIGH,
	CPOL_HIGH_CPHA_LOW,
	CPOL_HIGH_CPHA_HIGH
};*/


class SPI
{
	SPI_CONFIGURATION config;
	int gsbi_id, SPI_mod, IO_mode, master_slave_mode, clock_rate;

public:

	SPI();

	//BOOL 	Initialize(int gsbi_id, int SPI_mod, int IO_mode, int master_slave_mode, int clock_rate);

    BOOL	Level_0A();

    BOOL	Execute( int testLevel );

	BOOL	DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy);

};


