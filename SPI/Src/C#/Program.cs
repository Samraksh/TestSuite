using System;
using System.Threading;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
//using Samraksh.SPOT.Hardware;

/* This is a simple working version of a test program for the SPI protocol for the eMote.
 * This simply sets up the pins and writes data */

namespace Samraksh.SPOT.Testing
{
    public class SPITest
    {
		
		private SPI _spi0;
		private Random prng;

        /*public class Configuration
        {
            public readonly Cpu.Pin BusyPin;
            public readonly bool BusyPin_ActiveState;
            public readonly bool ChipSelect_ActiveState;
            public readonly uint ChipSelect_HoldTime;
            public readonly Cpu.Pin ChipSelect_Port;
            public readonly uint ChipSelect_SetupTime;
            public readonly bool Clock_Edge;
            public readonly bool Clock_IdleState;
            public readonly uint Clock_RateKHz;
            public readonly SPI.SPI_module SPI_mod;

            public Configuration(Cpu.Pin ChipSelect_Port, bool ChipSelect_ActiveState, uint ChipSelect_SetupTime, uint ChipSelect_HoldTime, 
         *                                                      bool Clock_IdleState, bool Clock_Edge, uint Clock_RateKHz, SPI.SPI_module SPI_mod);
            public Configuration(Cpu.Pin ChipSelect_Port, bool ChipSelect_ActiveState, uint ChipSelect_SetupTime, uint ChipSelect_HoldTime, 
         *                                                      bool Clock_IdleState, bool Clock_Edge, uint Clock_RateKHz, SPI.SPI_module SPI_mod, Cpu.Pin BusyPin, bool BusyPin_ActiveState);
        }*/
		
		public SPITest()
		{
				// Configure to the exposed spi module
				// spi1 is hooked up the radio and can not be used here for testing 
                /*_spi0 = new SPI(new SPI.Configuration((Cpu.Pin)8, false, 0, 0, false,
                        false, 2, SPI.SPI_module.SPI2));*/
                _spi0 = new SPI(new SPI.Configuration(Cpu.Pin.GPIO_Pin8, false, 0, 0, false,
                        false, 2, SPI.SPI_module.SPI2));
                /*_spi0 = new SPI(new SPI.Configuration((Cpu.Pin)Cpu.PinUsage.OUTPUT, false, 0, 0, false,
                            false, 2, SPI.SPI_module.SPI2));*/
                	
				//prng = new Random();
		}
		
		public void Run()
		{
			byte[] data = new byte[1];
            
			while(true)
			{
					//prng.NextBytes(data);
                    data[0] = 0;
                    _spi0.Write(data);
                	
					Thread.Sleep(100);

                    data[0] = 1;
                    _spi0.Write(data);
			}
		}
		
        public static void Main()
        {
		
			SPITest spitest = new SPITest();
			
			spitest.Run();
        
        }

    }
}
