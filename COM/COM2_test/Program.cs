using System;
using System.Threading;
using System.IO;
using System.IO.Ports;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
//using Samraksh.eMote.DotNow;

namespace COM
{
    public class Program
    {
		public static SerialPort serialPort1;
		public static SerialPort serialPort2;

		static void SerialPort1Handler(object sender, SerialDataReceivedEventArgs e)
        {            
			byte[] m_recvBuffer = new byte[100];
            SerialPort serialPort = (SerialPort)sender;
			
			int numBytes = serialPort.BytesToRead;
            serialPort.Read(m_recvBuffer, 0, numBytes);
			serialPort2.Write(m_recvBuffer, 0, numBytes);
			serialPort2.Flush();

        }

		static void SerialPort2Handler(object sender, SerialDataReceivedEventArgs e)
        {            
			byte[] m_recvBuffer = new byte[100];
            SerialPort serialPort = (SerialPort)sender;
			
			int numBytes = serialPort.BytesToRead;
            serialPort.Read(m_recvBuffer, 0, numBytes);
			for (int i = 0; i<numBytes; i++){
				m_recvBuffer[i] = (byte)(m_recvBuffer[i]+1);
			}
			serialPort1.Write(m_recvBuffer, 0, numBytes);
			serialPort1.Flush();

        }

        public static void Main()
        {
			serialPort1 = new SerialPort("COM1");
			serialPort1.BaudRate = 115200;
            serialPort1.Parity = Parity.None;
            serialPort1.StopBits = StopBits.One;
            serialPort1.DataBits = 8;
            serialPort1.Handshake = Handshake.None;
			serialPort1.DataReceived += new SerialDataReceivedEventHandler(SerialPort1Handler);

			serialPort2 = new SerialPort("COM2");
			serialPort2.BaudRate = 115200;
            serialPort2.Parity = Parity.None;
            serialPort2.StopBits = StopBits.One;
            serialPort2.DataBits = 8;
            serialPort2.Handshake = Handshake.None;
			serialPort2.DataReceived += new SerialDataReceivedEventHandler(SerialPort1Handler);

			serialPort1.Open();			
			serialPort2.Open();

			while (true) {
				//System.Threading.Thread.Sleep(100);
			}	
		}
		
		
   }
}
