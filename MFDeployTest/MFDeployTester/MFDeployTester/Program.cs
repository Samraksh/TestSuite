using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Collections;

using _DBG = Microsoft.SPOT.Debugger;
using _WP = Microsoft.SPOT.Debugger.WireProtocol;

// Nived.Sivadas
// Attempts to test each functionality of MFDeploy due  to the host of issues we are facing 

namespace Samraksh.SPOT.Tests
{

    public enum EraseOptions
    {
        Deployment = 0x01,
        UserStorage = 0x02,
        FileSystem = 0x04,
        Firmware = 0x08,
        UpdateStorage = 0x10,
        SimpleStorage = 0x20,
    }

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

        private _DBG.PortDefinition m_port;
        private _DBG.PortDefinition m_portTinyBooter;
        

        public Stream stream;

        public _DBG.PortDefinition pd;

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

        private bool IsClrDebuggerEnabled()
        {
            try
            {
                if (m_eng.IsConnectedToTinyCLR)
                {
                    return (m_eng.Capabilities.SourceLevelDebugging);
                }
            }
            catch
            {
            }
            return false;
        }

        public bool Disconnect()
        {
            if (m_eng != null)
            {
                m_eng.OnNoise -= new _DBG.NoiseEventHandler(OnNoiseHandler);
                m_eng.OnMessage -= new _DBG.MessageEventHandler(OnMessage);

                m_eng.Stop();
                m_eng.Dispose();
                m_eng = null;
            }
            return true;
        }

        public bool Connect(int timeout_ms, bool tryconnect)
        {
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

                if (m_eng.TryToConnect(0, timeout_ms))
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

            return true;
        }

        public MFDeploy()
        {
            //_DBG.PortDefinition pd = null;

            m_eng = null;
            pd = null;

            /*

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
            */

            
        }

        public bool Erase(params EraseOptions[] options)
        {
            bool ret = false;
            bool fReset = false;

            if (m_eng == null) throw new Exception("Eng instance not initialized\n");

            EraseOptions optionFlags = 0;

            if (options == null || options.Length == 0)
            {
                optionFlags = (EraseOptions.Deployment | EraseOptions.FileSystem | EraseOptions.UserStorage | EraseOptions.SimpleStorage | EraseOptions.UpdateStorage);
            }
            else
            {
                foreach (EraseOptions opt in options)
                {
                    optionFlags |= opt;
                }
            }

            Console.WriteLine("Connecting to Device ... \n");

            if (!Connect(500, true))
            {
                throw new Exception("Unable to connect to device\n");
            }

            Console.WriteLine("Obtaining Flash Sector Map ... \n");

            _DBG.WireProtocol.Commands.Monitor_FlashSectorMap.Reply reply = m_eng.GetFlashSectorMap();

            if (reply == null) throw new Exception("Unable to connect to device\n");

            _DBG.WireProtocol.Commands.Monitor_Ping.Reply ping = m_eng.GetConnectionSource();

            ret = true;


            long total = 0;
            long value = 0;

            bool isConnectedToCLR = ((ping != null) && (ping.m_source == _DBG.WireProtocol.Commands.Monitor_Ping.c_Ping_Source_TinyCLR));

            // Pause execution 
            if (isConnectedToCLR)
            {
                Console.WriteLine("Connected to CLR and pausing execution ... \n");
                m_eng.PauseExecution();
            }

            List<_DBG.WireProtocol.Commands.Monitor_FlashSectorMap.FlashSectorData> eraseSectors = new List<_DBG.WireProtocol.Commands.Monitor_FlashSectorMap.FlashSectorData>();

            foreach (_DBG.WireProtocol.Commands.Monitor_FlashSectorMap.FlashSectorData fsd in reply.m_map)
            {
                
                switch (fsd.m_flags & _DBG.WireProtocol.Commands.Monitor_FlashSectorMap.c_MEMORY_USAGE_MASK)
                {
                    case _DBG.WireProtocol.Commands.Monitor_FlashSectorMap.c_MEMORY_USAGE_DEPLOYMENT:
                        if (EraseOptions.Deployment == (optionFlags & EraseOptions.Deployment))
                        {
                            eraseSectors.Add(fsd);
                            total++;
                        }
                        break;

                }
            }

            foreach (_DBG.WireProtocol.Commands.Monitor_FlashSectorMap.FlashSectorData fsd in eraseSectors)
            {

                Console.WriteLine("Erasing ... Address {0:X}  Size {1:X}", fsd.m_address.ToString(), fsd.m_size.ToString());

                ret &= m_eng.EraseMemory(fsd.m_address, fsd.m_size);

                value++;
            }

            // reset if we specifically entered tinybooter for the erase
            if (fReset)
            {
                m_eng.ExecuteMemory(0);
                
            }
            // reboot if we are talking to the clr
            if (isConnectedToCLR)
            {
                Console.WriteLine("Rebooting ...");
                m_eng.RebootDevice(_DBG.Engine.RebootOption.RebootClrOnly);
            }

            return ret;
        }

        public bool ConnectToTinyBooter()
        {
            bool ret = false;

            if (!Connect(500, true)) return false;

            if (m_eng != null)
            {
                if (m_eng.ConnectionSource == _DBG.ConnectionSource.TinyBooter) return true;

                m_eng.RebootDevice(_DBG.Engine.RebootOption.EnterBootloader);

                // tinyBooter is only com port so
                if (m_port is _DBG.PortDefinition_Tcp)
                {
                    _DBG.PortDefinition pdTmp = m_port;

                    Disconnect();

                    try
                    {
                        m_port = m_portTinyBooter;

                        // digi takes forever to reset
                        if (!Connect(60000, true))
                        {
                            Console.WriteLine("Unable to connect to TinyBooter ...\n");
                            return false;
                        }
                    }
                    finally
                    {
                        m_port = pdTmp;
                    }
                }
                bool fConnected = false;
                for (int i = 0; i < 40; i++)
                {

                    if (fConnected = m_eng.TryToConnect(0, 500, true, _DBG.ConnectionSource.Unknown))
                    {
                        _WP.Commands.Monitor_Ping.Reply reply = m_eng.GetConnectionSource();
                        ret = (reply.m_source == _WP.Commands.Monitor_Ping.c_Ping_Source_TinyBooter);

                        break;
                    }
                }
                if (!fConnected)
                {
                    Console.WriteLine("Unable to connect to TinyBooter ...\n");
                }
            }
            return ret;
        }

        private void PrepareForDeploy(ArrayList blocks)
        {
            const uint c_DeploySector = _WP.Commands.Monitor_FlashSectorMap.c_MEMORY_USAGE_DEPLOYMENT;
            const uint c_SectorUsageMask = _WP.Commands.Monitor_FlashSectorMap.c_MEMORY_USAGE_MASK;

            bool fEraseDeployment = false;

            // if vsdebug is not enabled then we cannot write/erase
            if (!IsClrDebuggerEnabled())
            {
                // only check for signature file if we are uploading firmware
                if (!ConnectToTinyBooter()) throw new Exception("No Response from device\n"); ;
            }

            _WP.Commands.Monitor_FlashSectorMap.Reply map = m_eng.GetFlashSectorMap();

            if (map == null) throw new Exception("No Response from device\n");

            foreach (_DBG.SRecordFile.Block bl in blocks)
            {
                foreach (_WP.Commands.Monitor_FlashSectorMap.FlashSectorData sector in map.m_map)
                {
                    if (sector.m_address == bl.address)
                    {
                        // only support writing with CLR to the deployment sector and RESERVED sector (for digi)
                        if (c_DeploySector == (c_SectorUsageMask & sector.m_flags))
                        {
                            fEraseDeployment = true;
                        }
                        else
                        {
                            if (m_eng.ConnectionSource != _DBG.ConnectionSource.TinyBooter)
                            {
                                //if (OnProgress != null) OnProgress(0, 1, Properties.Resources.StatusConnectingToTinyBooter);

                                // only check for signature file if we are uploading firmware
                                if (!ConnectToTinyBooter()) throw new Exception("No Response from device\n"); 
                            }
                        }
                        break;
                    }
                }
            }
            if (fEraseDeployment)
            {
                this.Erase(EraseOptions.Deployment);
            }
            else if (m_eng.ConnectionSource != _DBG.ConnectionSource.TinyBooter)
            {
                //if we are not writing to the deployment sector then assure that we are talking with TinyBooter
                ConnectToTinyBooter();
            }
            if (m_eng.ConnectionSource == _DBG.ConnectionSource.TinyCLR)
            {
                m_eng.PauseExecution();
            }
        }

        public bool Deploy(string filePath, string signatureFile, ref uint entryPoint)
        {
            entryPoint = 0;

            if (!File.Exists(filePath)) throw new FileNotFoundException(filePath);
            if (m_eng == null) throw new Exception("Engine not initialized");


            m_eng.TryToConnect(1, 100, true, _DBG.ConnectionSource.Unknown);

            bool sigExists = File.Exists(signatureFile);
            FileInfo fi = new FileInfo(filePath);

            ArrayList blocks = new ArrayList();
            entryPoint = _DBG.SRecordFile.Parse(filePath, blocks, sigExists ? signatureFile : null);

            if (blocks.Count > 0)
            {
                long total = 0;
                long value = 0;

                for (int i = 0; i < blocks.Count; i++)
                {
                    total += (blocks[i] as _DBG.SRecordFile.Block).data.Length;
                }

                PrepareForDeploy(blocks);
                
                foreach (_DBG.SRecordFile.Block block in blocks)
                {
                    long len  = block.data.Length;
                    uint addr = block.address;

                    //if (EventCancel.WaitOne(0, false)) throw new MFUserExitException();

                    block.data.Seek(0, SeekOrigin.Begin);

                    //if (OnProgress != null)
                    //{
                    //    OnProgress(0, total, string.Format(Properties.Resources.StatusEraseSector, block.address));
                    //}

                    // the clr requires erase before writing
                    if (!m_eng.EraseMemory(block.address, (uint)len)) return false;

                    while(len > 0 )
                    {
            //            if (EventCancel.WaitOne(0, false)) throw new MFUserExitException();

                        int buflen = len > 1024? 1024: (int)len;
                        byte[] data = new byte[buflen];

                        if (block.data.Read(data, 0, buflen) <= 0)  return false;

                        if (!m_eng.WriteMemory(addr, data)) return false;

                        value += buflen;
                        addr += (uint)buflen;
                        len  -= buflen;

                    }
                    if (_DBG.ConnectionSource.TinyCLR != m_eng.ConnectionSource)
                    {
                        byte[] emptySig = new byte[128];

            

                        if (!m_eng.CheckSignature(((block.signature == null || block.signature.Length == 0)? emptySig: block.signature), 0))  throw new Exception("Signature can not be verified") ;
                    }
                }
            

            


            }

            return true;

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

    class MFDeployTester
    {
        MFDeploy mf;

        public MFDeployTester()
        {
            mf = new MFDeploy();

            mf.Connect(500, true);
        }

        public void DeployTest()
        {
            UInt16 i = 0;

            string deploymentFile = "HelloWorld.s19";

            string signatureFile = "";

            uint entrypoint = 0;

            while (i++ < 100)
            {
                try
                {
                    mf.Deploy(deploymentFile, signatureFile, ref entrypoint);
                }
                catch (Exception)
                {
                    mf.Disconnect();
                }

                Thread.Sleep(500);
                mf.Disconnect();

            }

        }

        public void EraseTest()
        {
            UInt16 i = 0;
            EraseOptions[] options = new EraseOptions[1];
            UInt16 successfulErases = 0;

            options[0] = EraseOptions.Deployment;

            mf.Connect(500, true);

            while (i++ < 100)
            {
                try
                {
                    if (mf.Erase(options))
                    {
                        Console.WriteLine("Erase Successful");
                        successfulErases++;
                    }
                }
                catch (Exception)
                {
                    // Disconnect the link
                    mf.Disconnect();
                }

                Thread.Sleep(500);

            }

            mf.Disconnect();

            Console.WriteLine("Number of erases " + successfulErases.ToString() + "Total number of erases " + i.ToString()); 

        }


        public void PingTest()
        {
            PingConnectionType ptype = PingConnectionType.NoConnection;
            UInt16 i = 0;
            UInt16 successfulPings = 0;

            mf.Connect(500, true);

            while (i++ < 100)
            {
                ptype = mf.Ping();

                if (ptype != PingConnectionType.NoConnection)
                    successfulPings++;

                Console.WriteLine(ptype.ToString());

                Thread.Sleep(500);

            }

            Console.WriteLine("Successfull Pings " + successfulPings.ToString() + " Total Pings " + i.ToString());

            mf.Disconnect();
        }

        static void Main(string[] args)
        {
            MFDeployTester mf = new MFDeployTester();

            //mf.PingTest();

            //mf.EraseTest();

            

        }
    }
}
