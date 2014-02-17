using System;
using System.Threading;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
//using Samraksh.SPOT.Hardware;

namespace Samraksh.SPOT.Testing
{
    public class SPITest
    {
		
		//private SPI _spi0, _spi1, _spi2, _spi3, _spi4, _spi5, _spi6, _spi7, _spi8, _spi9, _spi10, _spi11, _spi12, _spi13, _spi14, _spi15;
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
                _spi0 = new SPI(new SPI.Configuration((Cpu.Pin)10, false, 0, 0, false,
                        false, 2, SPI.SPI_module.SPI4));
            /*    
                _spi1 = new SPI(new SPI.Configuration((Cpu.Pin)1, true, 0, 0, false,
                        false, 24000, SPI.SPI_module.SPI2));
                _spi2 = new SPI(new SPI.Configuration((Cpu.Pin)2, true, 0, 0, false,
                        false, 24000, SPI.SPI_module.SPI2));
                _spi3 = new SPI(new SPI.Configuration((Cpu.Pin)3, true, 0, 0, false,
                        false, 24000, SPI.SPI_module.SPI2));
				_spi4 = new SPI(new SPI.Configuration((Cpu.Pin)4, true, 0, 0, false,
                        false, 24000, SPI.SPI_module.SPI2));
                _spi5 = new SPI(new SPI.Configuration((Cpu.Pin)5, true, 0, 0, false,
                        false, 24000, SPI.SPI_module.SPI2));
                _spi6 = new SPI(new SPI.Configuration((Cpu.Pin)6, true, 0, 0, false,
                        false, 24000, SPI.SPI_module.SPI2));
                _spi7 = new SPI(new SPI.Configuration((Cpu.Pin)7, true, 0, 0, false,
                        false, 24000, SPI.SPI_module.SPI2));
                _spi8 = new SPI(new SPI.Configuration((Cpu.Pin)8, true, 0, 0, false,
                        false, 24000, SPI.SPI_module.SPI2));
                _spi9 = new SPI(new SPI.Configuration((Cpu.Pin)9, true, 0, 0, false,
                        false, 24000, SPI.SPI_module.SPI2));
                _spi10 = new SPI(new SPI.Configuration((Cpu.Pin)10, true, 0, 0, false,
                        false, 24000, SPI.SPI_module.SPI2));
                _spi11 = new SPI(new SPI.Configuration((Cpu.Pin)11, true, 0, 0, false,
                        false, 24000, SPI.SPI_module.SPI2));
                _spi12 = new SPI(new SPI.Configuration((Cpu.Pin)12, true, 0, 0, false,
                        false, 24000, SPI.SPI_module.SPI2));
                _spi13 = new SPI(new SPI.Configuration((Cpu.Pin)13, true, 0, 0, false,
                        false, 24000, SPI.SPI_module.SPI2));
                _spi14 = new SPI(new SPI.Configuration((Cpu.Pin)14, true, 0, 0, false,
                        false, 24000, SPI.SPI_module.SPI2));
                _spi15 = new SPI(new SPI.Configuration((Cpu.Pin)15, true, 0, 0, false,
                        false, 24000, SPI.SPI_module.SPI2));*/
					
				prng = new Random();
		}
		
		public void Run()
		{
			byte[] data = new byte[1];
            
			while(true)
			{
					//prng.NextBytes(data);
                    data[0] = 0;
                    _spi0.Write(data);
                	/*
                    _spi1.Write(data);
                    _spi2.Write(data);
                    _spi3.Write(data);
                    _spi4.Write(data);
                    _spi5.Write(data);
                    _spi6.Write(data);
                    _spi7.Write(data);
                    _spi8.Write(data);
                    _spi9.Write(data);
                    _spi10.Write(data);
                    _spi11.Write(data);
                    _spi12.Write(data);
                    _spi13.Write(data);
                    _spi14.Write(data);
                    _spi15.Write(data);*/
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
