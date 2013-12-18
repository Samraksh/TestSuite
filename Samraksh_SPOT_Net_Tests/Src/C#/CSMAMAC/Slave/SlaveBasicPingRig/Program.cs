using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using Samraksh.SPOT.Net;
using Samraksh.SPOT.Net.Mac;
using System.Threading;

namespace BasicPing
{
    public class SlaveBasicPing
    {
        CSMA myCSMA;

        MacConfiguration macconfig;

        ReceiveCallBack callback;

        NeighbourhoodChangeCallBack myNeighborCB;

        static OutputPort SendPort = new OutputPort((Cpu.Pin)30, true);
        static OutputPort ReceivePort = new OutputPort((Cpu.Pin)31, true);


        void HandleMessage(UInt16 numberOfPackets)
        {
            Debug.Print("Number Of Packets in Buffer : " + numberOfPackets.ToString() + "\n");

            UInt16 packetsToRead = numberOfPackets;

            Debug.EnableGCMessages(false);

           // ReceivePort.Write(true);
           // ReceivePort.Write(false);

            
            while (packetsToRead > 0)
            {
                ReceivePort.Write(true);
                Message recvMessage = myCSMA.GetNextPacket();
                ReceivePort.Write(false);

                if (recvMessage == null)
                {
                    Debug.Print("No packets in buffer \n");
                    return;
                }

                byte[] packet = recvMessage.GetMessage();

                byte LQI = recvMessage.LQI;
                ushort src = recvMessage.Src;
                byte RSSI = recvMessage.RSSI;
                bool unicast = recvMessage.Unicast;

                UInt16 msgid = (UInt16) (packet[2] & 0xff);
                msgid |= (UInt16) ((packet[1] << 8) & 0xff00);


                Debug.Print("Recieved Message Msgid " + msgid.ToString()  + " " + " LQI : " + LQI.ToString() + " SRC : " + src.ToString() + " RSSI : " + RSSI.ToString() + " UNICAST : " + unicast.ToString() + "\n");

                packetsToRead--;

                myCSMA.ReleasePacket();

            }
             
        }

        void NeighborChange(UInt16 noOfNeigbors)
        {
        }

        public SlaveBasicPing()
        {
            macconfig = new MacConfiguration();

            macconfig.radioConfig.SetRadioName(Samraksh.SPOT.Net.Radio.RadioName.RF231RADIO);

            myNeighborCB = NeighborChange;
            callback = HandleMessage;

            if (CSMA.Configure(macconfig, callback, myNeighborCB) != DeviceStatus.Success)
            {
                Debug.Print("The CSMA Configure call failed \n");
            }

            try
            {
                myCSMA = CSMA.Instance;
            }
            catch (MacNotConfiguredException)
            {
                Debug.Print("Exception in MasterBasicPing due to Mac not being configured\n");
            }
            catch (Exception)
            {
                Debug.Print("Unknown exception in MasterBasicPing\n");
            }

            Debug.Print("CSMA Init done. \n");

        }

        public static void Main()
        {
            SlaveBasicPing prg = new SlaveBasicPing();

            while (true)
            {
                Thread.Sleep(500);
            }
        }

    }
}
