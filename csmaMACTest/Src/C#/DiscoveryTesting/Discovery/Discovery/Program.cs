using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Net;
using Samraksh.SPOT.Net.Mac;

using System.Threading;

namespace Discovery
{

    public class NeighbourTableTesting
    {
        CSMA csmaObject;

        UInt16 myAddress;

        ReceiveCallBack rcallback;

        NeighbourhoodChangeCallBack ncallback;

        MacConfiguration csmaConfig = null;

        void HandleMessage(UInt16 numberOfPackets)
        {
            Debug.Print("The Number of Packets : " + numberOfPackets + "\n");
        }

        public NeighbourTableTesting()
        {

            csmaConfig = new MacConfiguration();

            csmaConfig.NeighbourLivelinesDelay = 100;

            rcallback = HandleMessage;

            ncallback = HandleNeighbourChange;

            if (CSMA.Configure(csmaConfig, rcallback, ncallback) != DeviceStatus.Success)
            {
                Debug.Print("The CSMA Configure call failed \n");
            }

            try
            {
                csmaObject = CSMA.Instance;
            }
            catch (MacNotConfiguredException m)
            {
                Debug.Print("Exception in NeighbourTable Test : Mac not configured \n");
            }
            catch (Exception e)
            {
                Debug.Print("Unknown exception from mac grab instance\n");
            }

            myAddress = csmaObject.GetAddress();

            Debug.Print("CSMA Init done. \n");

        }

        void HandleNeighbourChange(UInt16 neighboursChanged)
        {
            UInt16[] neighbourlist = csmaObject.GetNeighbourList();

            if (neighbourlist == null)
            {
                Debug.Print("The Neighborlist is null \n");
                return;
            }

            Debug.Print("My Neighbours are : \n");

            for (int i = 0; i < 5; i++)
            {
                Debug.Print(neighbourlist[i].ToString() + " ");
            }

            Debug.Print("\n");
        }

    }

    public class Program
    {
        public static void Main()
        {
            NeighbourTableTesting ntest = new NeighbourTableTesting();

            while (true)
            {
                Thread.Sleep(500);
            }
          
        }

    }
}
