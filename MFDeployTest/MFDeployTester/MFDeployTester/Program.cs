using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

using _DBG = Microsoft.SPOT.Debugger;

// Nived.Sivadas
// Attempts to test each functionality of MFDeploy due  to the host of issues we are facing 

namespace MFDeployTester
{
    public enum PingConnectionType
    {
        TinyCLR,
        TinyBooter,
        NoConnection,
        MicroBooter,
    }

    public class MFDeploy
    {

        private _DBG.Engine m_eng;

        public Stream stream;

        private readonly byte[] m_data =
                    {
                        67, 111, 112, 121, 114, 105, 103, 104, 116, 32, 50, 48, 48, 51, 13, 10,
                        77, 105, 99, 114, 111, 115, 111, 102, 116, 32, 67, 111, 114, 112, 13, 10,
                        49, 32, 77, 105, 99, 114, 111, 115, 111, 102, 116, 32, 87, 97, 121, 13,
                        10, 82, 101, 100, 109, 111, 110, 100, 44, 32, 87, 65, 13, 10, 57, 56,
                        48, 53, 50, 45, 54, 51, 57, 57, 13, 10, 85, 46, 83, 46, 65, 46,
                        13, 10, 65, 108, 108, 32, 114, 105, 103, 104, 116, 115, 32, 114, 101, 115,
                        101, 114, 118, 101, 100, 46, 13, 10, 77, 73, 67, 82, 79, 83, 79, 70,
                        84, 32, 67, 79, 78, 70, 73, 68, 69, 78, 84, 73, 65, 76, 13, 10,
                        55, 231, 64, 0, 118, 157, 50, 129, 173, 196, 117, 75, 87, 255, 238, 223,
                        181, 114, 130, 29, 130, 170, 89, 70, 194, 108, 71, 230, 192, 61, 9, 29,
                        216, 23, 196, 204, 21, 89, 242, 196, 143, 255, 49, 65, 179, 224, 237, 213,
                        15, 250, 92, 181, 77, 10, 200, 21, 219, 202, 181, 127, 64, 172, 101, 87,
                        166, 35, 162, 28, 70, 172, 138, 40, 35, 215, 207, 160, 195, 119, 187, 95,
                        239, 213, 127, 201, 46, 15, 60, 225, 19, 252, 227, 17, 211, 80, 209, 52,
                        74, 122, 115, 2, 144, 20, 153, 241, 244, 57, 139, 10, 57, 65, 248, 204,
                        149, 252, 17, 159, 244, 11, 186, 176, 59, 187, 167, 107, 83, 163, 62, 122
                    };

        public void OnNoiseHandler(byte[] data, int index, int count)
        {
        }

        public void OnMessage(_DBG.WireProtocol.IncomingMessage msg, string text)
        {
        }

        public MFDeploy()
        {
            _DBG.PortDefinition pd = null;

            m_eng = null;

            string port = "COM1";
            uint baudrate = 115200;

            pd = _DBG.PortDefinition.CreateInstanceForSerial(port, port, baudrate);



            try
            {
                if (m_eng == null)
                {
                    m_eng = new _DBG.Engine(pd);

                    m_eng.OnNoise += new _DBG.NoiseEventHandler(OnNoiseHandler);
                    m_eng.OnMessage += new _DBG.MessageEventHandler(OnMessage);

                    m_eng.Start();
                }

                if (m_eng.TryToConnect(0, 1000))
                {
                    m_eng.UnlockDevice(m_data);
                }
                else
                {
                    Console.WriteLine("Unable to connect to COM Port\n");
                }


            }
            catch (Exception)
            {
            }


            
        }

        public PingConnectionType Ping()
        {
            // Set default to no connection 
            PingConnectionType ret = PingConnectionType.NoConnection;

            if (m_eng != null)
            {
                _DBG.WireProtocol.Commands.Monitor_Ping.Reply reply = m_eng.GetConnectionSource();

                if (reply != null)
                {
                    switch (reply.m_source)
                    {
                        case _DBG.WireProtocol.Commands.Monitor_Ping.c_Ping_Source_TinyCLR:
                            ret = PingConnectionType.TinyCLR;
                            break;
                        case _DBG.WireProtocol.Commands.Monitor_Ping.c_Ping_Source_TinyBooter:
                            ret = PingConnectionType.TinyBooter;
                            break;
                    }
                }
            }


            return ret;

        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            PingConnectionType ptype = PingConnectionType.NoConnection;
            MFDeploy mf = new MFDeploy();
            UInt16 i = 0;
            UInt16 successfulPings = 0;

            while (i++ < 100)
            {
                 ptype = mf.Ping();

                 if (ptype == PingConnectionType.NoConnection)
                     successfulPings++;

                 Console.WriteLine(ptype.ToString());

                 Thread.Sleep(500);
                
            }

            Console.WriteLine("Ping Rate : " + successfulPings / i);
        }
    }
}
