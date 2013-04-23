using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Net.Radio;
using System.Threading;

namespace Samraksh_SPOT_Net_Tests
{
    class RadioTest
    {
        Radio_802_15_4 radioObj;
        RadioConfiguration radioConfiguration;
        Samraksh.SPOT.Net.ReceiveCallBack mycallback;

        Random rng;
        byte[] message;

        public RadioTest()
        {
            rng = new Random(255);
            message = new byte[128];
        }

        public bool Level_0A()
        {
            bool result = false;

            mycallback = HandleMessage;

            radioConfiguration = new RadioConfiguration();

            radioConfiguration.Channel = (int)Channels.Channel_26;
            radioConfiguration.TxPower = (int)TxPowerValue.Power_3dBm;

            Radio_802_15_4.Configure(radioConfiguration, mycallback);

            radioObj = Radio_802_15_4.GetInstance();

            Timer mytimer = new Timer(Callback, null, 0, 200);

            return result;
        }


        void Callback(Object state)
        {
            ushort size = (ushort)rng.Next(255);

            rng.NextBytes(message);

            radioObj.Send(message, size);
        }

        void HandleMessage(UInt16 NumberOfPackets)
        {
        }

    }

    class Program
    {
        static void Main(string[] args)
        {
        }
    }

    public class Program
    {
        public static void Main()
        {
            Debug.Print(
                Resources.GetString(Resources.StringResources.String1));
        }

    }
}
