using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

// Note: this is an i2c test done with the accelerometer at the other end of the bus
// I2C Testing can not be done with out a slave
namespace Samraksh.SPOT.Tests
{

    public class I2CTest
    {
        // Enum describing the GRange of the accelerometer
        public enum GRange : byte
        {
            TwoG = 0x00,
            FourG = 0x01,
            EightG = 0x02,
            SixteenG = 0x03
        }

        // Enum describing the output resolution of the accelerometer
        public enum OutputResolution : byte
        {
            FixedResolution = 0x00,
            FullResolution = 0x08
        }

        public struct Data
        {
            /// <summary>
            /// The raw x axis acceleration data 
            /// </summary>
            public Int16 RawX;

            /// <summary>
            /// The raw y axis acceleration data 
            /// </summary>
            public Int16 RawY;

            /// <summary>
            /// The raw z axis acceleration data 
            /// </summary>
            public Int16 RawZ;

            public Single X;

            public Single Y;

            public Single Z;

            public override string ToString()
            {
                return "X=" + X.ToString() + "g, Y=" + Y.ToString() + "g, Z=" + Z.ToString() + "g";
            }

        }

        // Enum describing the register map on the accelerometer 
        private enum RegisterMap : byte
        {
            DEVID = 0x00,
            THRESH_TAP = 0x1d,
            OFSX = 0x1e,
            OFSY = 0x1f,
            OFSZ = 0x20,
            DUR = 0x21,
            LATENT = 0x22,
            WINDOW = 0x23,
            THRESH_ACT = 0x24,
            THRESH_INACT = 0x25,
            TIME_INACT = 0x26,
            ACT_INACT_CTL = 0x27,
            THRESH_FF = 0x28,
            TIME_FF = 0x29,
            TAP_AXES = 0x2a,
            ACT_TAP_STATUS = 0x2b,
            BW_RATE = 0x2c,
            POWER_CTL = 0x2d,
            INT_ENABLE = 0x2e,
            INT_MAP = 0x2f,
            INT_SOURCE = 0x30,
            DATA_FORMAT = 0x31,
            DATAX0 = 0x32,
            DATAX1 = 0x33,
            DATAY0 = 0x34,
            DATAY1 = 0x35,
            DATAZ0 = 0x36,
            DATAZ1 = 0x37,
            FIFO_CTL = 0x38,
            FIFO_STATUS = 0x39
        }

        private I2CDevice.Configuration configuration;

        private I2CDevice i2cbus;

        private byte ReadRegister(RegisterMap register)
        {
            I2CDevice.I2CTransaction[] xActions = new I2CDevice.I2CTransaction[2];

            byte[] RegisterNum = new byte[1] { (byte) register };
            byte[] id = new byte[1];

            xActions[0] = I2CDevice.CreateWriteTransaction(RegisterNum);
            xActions[1] = I2CDevice.CreateReadTransaction(id);

            i2cbus.Execute(xActions, 1000);
           
            return id[0];
        }

        private void WriteRegister(RegisterMap register, byte value)
        {
            byte[] data = new byte[] { (byte)register, value };

            I2CDevice.I2CWriteTransaction write = I2CDevice.CreateWriteTransaction(data);
            I2CDevice.I2CTransaction[] writeTransaction = new I2CDevice.I2CTransaction[] { write };

            lock (i2cbus)
            {
                i2cbus.Config = configuration;
                if (i2cbus.Execute(writeTransaction, 1000) == 0)
                {
                    Debug.Print("WriteRegister failed");
                }
            }

        }

        public I2CTest()
        {
            configuration = new I2CDevice.Configuration(0x53, 100);

            i2cbus = new I2CDevice(configuration);
        }


        public void Level_0A()
        {
            int totalTries = 10;
            int successfulTries = 0;

            int i = 0;

            while (i++ < totalTries)
            {
                if (ReadRegister(0x0) == 0xE5)
                {
                    successfulTries++;

                    Debug.Print("Testing ... Success");
                }
                else
                {
                    Debug.Print("Testing ... Fail");
                }



                System.Threading.Thread.Sleep(250);
            }

            Debug.Print("Number of successful pings " + successfulTries.ToString());
         
        }

        public void Level_0B()
        {
            Debug.Print("Check Point 1");

            WriteRegister(RegisterMap.DATA_FORMAT, (byte)OutputResolution.FullResolution | (byte)GRange.SixteenG);
            WriteRegister(RegisterMap.POWER_CTL, 0x08);

            Debug.Print("Check Point 2");

            Data sensorData = new Data();

            byte[] data = new byte[6];

            I2CDevice.I2CWriteTransaction write = I2CDevice.CreateWriteTransaction(new Byte[] { (Byte)RegisterMap.DATAX0 });
            I2CDevice.I2CReadTransaction read = I2CDevice.CreateReadTransaction(data);
            I2CDevice.I2CTransaction[] getDataTransaction = new I2CDevice.I2CTransaction[] { write, read };

            int totalTries = 10;
            int successfulTries = 0;
            int i = 0;
            bool successFlag = false;

            Debug.Print("Check Point 3");

            while (i++ < totalTries)
            {
                lock (i2cbus)
                {
                    i2cbus.Config = configuration;
                    if (i2cbus.Execute(getDataTransaction, 1000) == 0)
                    {
                        Debug.Print("Fail");
                        
                    }
                    else
                    {
                        successFlag = true;
                        successfulTries++;
                    }
                }

                if (successFlag)
                {

                    sensorData.RawX = (Int16)(data[1] << 8 | data[0]); ;
                    sensorData.RawY = (Int16)(data[3] << 8 | data[2]);
                    sensorData.RawZ = (Int16)(data[5] << 8 | data[4]);
                    /*
                    sensorData.RawX = (Int16)(data[1] << 8 | data[0]);
                    sensorData.RawY = (Int16)(data[3] << 8 | data[2]);
                    sensorData.RawZ = (Int16)(data[5] << 8 | data[4]);
                    */
                    sensorData.X = 0.004f * sensorData.RawX;
                    sensorData.Y = 0.004f * sensorData.RawY;
                    sensorData.Z = 0.004f * sensorData.RawZ;

                    Debug.Print(sensorData.ToString());
                }
               

                successFlag = false;
                System.Threading.Thread.Sleep(250);
            }
            


        }

        public static void Main()
        {

            Debug.EnableGCMessages(false);

            I2CTest test = new I2CTest();

            test.Level_0A();

            test.Level_0B();
            /*
            I2CDevice.Configuration con = new I2CDevice.Configuration(0x53, 100);

            I2CDevice i2cdevice = new I2CDevice(con);

            I2CDevice.I2CTransaction[] xActions = new I2CDevice.I2CTransaction[2];

            byte[] RegisterNum = new byte[1] { 0x0 };
            byte[] id          = new byte[1];

            xActions[0] = I2CDevice.CreateWriteTransaction(RegisterNum);
            xActions[1] = I2CDevice.CreateReadTransaction(id);

            if (i2cdevice.Execute(xActions, 1000) == 0)
            {
                Debug.Print("Failed to perform i2c transcation");
            }
            else
            {
                Debug.Print("Transaction was successful" + id[0].ToString());
            }
            */

        }

    }
}
