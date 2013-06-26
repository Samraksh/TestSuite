using System;
using Microsoft.SPOT;
using System.Threading;
using Microsoft.SPOT.Hardware;

namespace Samraksh.SPOT.Tests
{
    public class SDTest
    {

        public static Random rng = new Random();

        public static bool sdSuccessFlag = false;

        public Samraksh.SPOT.Hardware.EmoteDotNow.SD.SDCallBackType sdResultCallBack;

        public static Samraksh.SPOT.Hardware.EmoteDotNow.SD mysdCard;

        public static byte[] dataBuffer = new byte[512];
        public static byte[] outputBuffer = new byte[512];

        public static OutputPort alive = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN1, false);
        public static OutputPort error = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN2, false);

        public static Samraksh.SPOT.Hardware.EmoteDotNow.EmoteLCD lcd;
        

        public SDTest()
        {

            lcd = new Hardware.EmoteDotNow.EmoteLCD();

            lcd.Initialize();

            sdResultCallBack = mySdCallback;

            mysdCard = new Hardware.EmoteDotNow.SD(sdResultCallBack);

            if (!Samraksh.SPOT.Hardware.EmoteDotNow.SD.Initialize())
            {
                error.Write(true);
                lcd.Write(Hardware.EmoteDotNow.LCD.CHAR_0, Hardware.EmoteDotNow.LCD.CHAR_0, Hardware.EmoteDotNow.LCD.CHAR_0, Hardware.EmoteDotNow.LCD.CHAR_0);
                Debug.Print("SD Card Initialization failed \n");
                throw new Exception("SD Initialization failed");
            }
        }

        public void Run()
        {
            UInt16 testCount = 0;

            while (testCount++ < 100)
            {

                if (testCount % 2 == 0)
                    alive.Write(true);
                else
                    alive.Write(false);

                rng.NextBytes(dataBuffer);

                if (!Samraksh.SPOT.Hardware.EmoteDotNow.SD.Write(dataBuffer, 0, 512))
                {
                    error.Write(true);
                    Debug.Print("Writing to SD Card failed ... \n");
                    lcd.Write(Hardware.EmoteDotNow.LCD.CHAR_E, Hardware.EmoteDotNow.LCD.CHAR_R, Hardware.EmoteDotNow.LCD.CHAR_R, Hardware.EmoteDotNow.LCD.CHAR_0);
                    return;
                }

                while (!sdSuccessFlag)
                {
                    Thread.Sleep(20);
                }

                sdSuccessFlag = false;

                if (!Samraksh.SPOT.Hardware.EmoteDotNow.SD.Read(outputBuffer, 0, 512))
                {
                    Debug.Print("Reading from the SD Card failed.... \n");
                    lcd.Write(Hardware.EmoteDotNow.LCD.CHAR_E, Hardware.EmoteDotNow.LCD.CHAR_R, Hardware.EmoteDotNow.LCD.CHAR_R, Hardware.EmoteDotNow.LCD.CHAR_1);
                    return;
                }

                for (UInt16 i = 0; i < 512; i++)
                {
                    if (dataBuffer[i] != outputBuffer[i])
                    {
                        lcd.Write(Hardware.EmoteDotNow.LCD.CHAR_E, Hardware.EmoteDotNow.LCD.CHAR_R, Hardware.EmoteDotNow.LCD.CHAR_R, Hardware.EmoteDotNow.LCD.CHAR_2);
                        Debug.Print("SD Card Test failed .. \n");
                        return;
                            
                    }
                }

            }

            if (testCount >= 100)
            {
                Debug.Print("SD Card Test Passed .. \n");
                lcd.Write(Hardware.EmoteDotNow.LCD.CHAR_S, Hardware.EmoteDotNow.LCD.CHAR_U, Hardware.EmoteDotNow.LCD.CHAR_C, Hardware.EmoteDotNow.LCD.CHAR_S);
                return;
            }
        }


        public static void mySdCallback(Samraksh.SPOT.Hardware.DeviceStatus status)
        {

            Debug.Print("Recieved SD Callback\n");

            sdSuccessFlag = true;
           
        }

        public static void Main()
        {
            SDTest sdTest = new SDTest();

            sdTest.Run();
            
        }

    }
}
