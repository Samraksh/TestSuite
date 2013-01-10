using System;
using System.Threading;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using Samraksh.SPOT.Hardware;

namespace Samraksh.SPOT.Testing
{
    public class SPITest
    {
		
		private SPI _spi;
		private Random prng;
		
		public SPITest()
		{
				// Configure to the exposed spi module
				// spi1 is hooked up the radio and can not be used here for testing 
				_spi = new SPI(new SPI.Configuration((Cpu.Pin)5, true, 0, 0, false,
					false, 4000, SPI.SPI_module.SPI2));
					
				prng = new Random();
		}
		
		public void Run()
		{
			byte[] data = new byte[10];
			while(true)
			{
					prng.NextBytes(data);
					_spi.Write(data);
					Thread.Sleep(100);
			}
		}
		
        public static void Main()
        {
		
			SPITest spitest = new SPITest();
			
			spitest.Run();
        
        }

    }
}
