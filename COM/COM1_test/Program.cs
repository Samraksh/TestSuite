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

		static void SerialPort1Handler(object sender, SerialDataReceivedEventArgs e)
        {            
			try{
				byte[] m_recvBuffer = new byte[100];
            	SerialPort serialPort = (SerialPort)sender;
			
				int numBytes = serialPort.BytesToRead;
				if (numBytes > 100)
					numBytes = 100;
            	serialPort.Read(m_recvBuffer, 0, numBytes);
				serialPort.Write(m_recvBuffer, 0, numBytes);
				serialPort.Flush();
			}
			catch (Exception)
            {
				//Debug.Print("\r\nexception\r\n");
			}
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

			serialPort1.Open();
			while (true) {
				System.Threading.Thread.Sleep(100);
			}	
		}
		
		
   }
}
