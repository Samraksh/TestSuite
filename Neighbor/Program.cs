using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Net;
using System.Threading;
using Samraksh.SPOT.Net.Mac;
using Microsoft.SPOT.Hardware;
using Samraksh.SPOT.Hardware.EmoteDotNow;

namespace Samraksh.SPOT.Tests
{
    public class NeighbourTableTesting
    {
        
        EmoteLCD lcd = new EmoteLCD();
        int neighborCount;

        OMAC myOMAC;
        MacConfiguration omacConfig = null;
        ReceiveCallBack rcallback;        
        NeighbourhoodChangeCallBack ncallback;

        void HandleMessage(UInt16 numberOfPackets)
        {
        }

        void HandleNeighbourChange(UInt16 neighboursChanged)
        {
            neighborCount = neighboursChanged;
			Debug.Print("Neighbor change: " + neighboursChanged.ToString() + "\r\n");
            

            UInt16[] neighbourlist = myOMAC.GetNeighbourList();

            if (neighbourlist == null)
            {
                Debug.Print("The Neighborlist is null \n");
            }
            else
            {

                Debug.Print("My Neighbours are : ");

                for (int i = 0; i < 12; i++)
                {
                    if (neighbourlist[i] != 0)
                    {
                        Debug.Print(neighbourlist[i].ToString() + " ");
                    }
                }

                Debug.Print("\n");
            }

            int onesDigit = neighborCount % 10;
            int tensDigit = ((neighborCount - onesDigit) % 100) / 10;           
            lcd.Write(LCD.CHAR_0, LCD.CHAR_0, LCD.CHAR_0 + tensDigit, LCD.CHAR_0 + onesDigit);
             
        }

        public NeighbourTableTesting()
        {
            lcd.Initialize();
            lcd.Write(LCD.CHAR_0, LCD.CHAR_0, LCD.CHAR_0, LCD.CHAR_0);

            omacConfig = new MacConfiguration();
            omacConfig.NeighbourLivelinesDelay = 60;            

            rcallback = HandleMessage;

            ncallback = HandleNeighbourChange;

            if (OMAC.Configure(omacConfig, rcallback, ncallback) != DeviceStatus.Success)
            {
                Debug.Print("The OMAC Configure call failed \n");
            }

            myOMAC = OMAC.Instance;
            
            
        }


        void updateDisplay()
        {
            int onesDigit = neighborCount % 10;
            int tensDigit = ((neighborCount - onesDigit) % 100) / 10;
            lcd.Write(LCD.CHAR_0, LCD.CHAR_0, LCD.CHAR_0 + tensDigit, LCD.CHAR_0 + onesDigit);
        }

        public static void Main()
        {
           NeighbourTableTesting ntest = new NeighbourTableTesting();


            while (true)
            {
                ntest.updateDisplay();
                Thread.Sleep(5000);
                Debug.Print("Running...\r\n");                
            }

        }

    }
}



