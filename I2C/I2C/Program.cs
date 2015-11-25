using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using Samraksh.eMote.DotNow;
using System.Threading;

namespace TestSuite
{
    public class Program
    {
		/*private static I2CDevice.Configuration config;
        private static I2CDevice i2cdevice;
			
		public static void writeData(Byte register, Byte value)
        {

			
            Byte[] data = new Byte[] { (Byte)register, value };

            I2CDevice.I2CWriteTransaction write = I2CDevice.CreateWriteTransaction(data);
            I2CDevice.I2CTransaction[] writeTransaction = new I2CDevice.I2CTransaction[] { write };

            lock (i2cdevice)
            {
                i2cdevice.Config = config;
                i2cdevice.Execute(writeTransaction, 50);
                
            }
        }
		public static void writeByte(Byte dataValue)
        {
            Byte[] data = new Byte[] { dataValue };

            I2CDevice.I2CWriteTransaction write = I2CDevice.CreateWriteTransaction(data);
            I2CDevice.I2CTransaction[] writeTransaction = new I2CDevice.I2CTransaction[] { write };

            //lock (i2cdevice)
            //{
                i2cdevice.Config = config;
                i2cdevice.Execute(writeTransaction, 100);                
            //}
        }*/
        public static void Main()
        {
			I2CDevice.Configuration config = new I2CDevice.Configuration(0x38, 0x11);
  
			I2CDevice i2c = new I2CDevice(config);
			
			
			byte[] write1 = new Byte[] {0x80, 0x48, 0xee, 0xef, 0x8d, 0xed };
			byte[] write2 = new Byte[] {0x80, 0x48, 0x8f, 0x8e, 0xcd, 0x6e };
			byte[] read = new byte[1];
  
			// create I2C write and read transaction
			I2CDevice.I2CTransaction[] i2cTx1 = new I2CDevice.I2CTransaction[1];
			i2cTx1[0] = I2CDevice.CreateWriteTransaction(write2);
			//i2cTx1[1] = I2CDevice.CreateReadTransaction(read);

			I2CDevice.I2CTransaction[] i2cTx2 = new I2CDevice.I2CTransaction[1];
			i2cTx2[0] = I2CDevice.CreateWriteTransaction(write1);
			//i2cTx2[1] = I2CDevice.CreateReadTransaction(read);
  
			while (true){
				// execution
				i2c.Execute(i2cTx1, 100);
				Thread.Sleep(1000);
				i2c.Execute(i2cTx2, 100);
				Thread.Sleep(1000);
			}

			/*try
            {
                config = new I2CDevice.Configuration(0x38, 100);
                i2cdevice = new I2CDevice(config);
            }
            catch
            {
                Debug.Print("I2C Bus initialization failed");
            }
			
			while (true){
				/*writeByte(0x80);
				Thread.Sleep(100);
				writeByte(0x48);
				Thread.Sleep(100);
				writeByte(0xee);
				Thread.Sleep(100);
				writeByte(0xef);
				Thread.Sleep(100);
				writeByte(0x8d);
				Thread.Sleep(100);
				writeByte(0xed);*/
				/*writeData(0xa5, 0x0F);
				//writeData(0xa6, 0x62);
				//writeData(0x22, 0x0F);
				//writeData(0xff, 0x0F);
				Thread.Sleep(1000);
			}*/
            
        }
    }
}
