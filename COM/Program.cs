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
		static void SerialPortHandler(object sender, SerialDataReceivedEventArgs e)
        {            
			byte[] m_recvBuffer = new byte[100];
            SerialPort serialPort = (SerialPort)sender;
			
			int numBytes = serialPort.BytesToRead;
            serialPort.Read(m_recvBuffer, 0, numBytes);
			serialPort.Write(m_recvBuffer, 0, numBytes);
			serialPort.Flush();

        }

        public static void Main()
        {
			//Debug.EnableGCMessages(false);
			//Debug.Print("Starting Main\r\n");

			SerialPort serialPort = new SerialPort("COM1");
			serialPort.BaudRate = 115200;
            serialPort.Parity = Parity.None;
            serialPort.StopBits = StopBits.One;
            serialPort.DataBits = 8;
            serialPort.Handshake = Handshake.None;

			serialPort.DataReceived += new SerialDataReceivedEventHandler(SerialPortHandler);

			serialPort.Open();
			/*Thread.Sleep(20000);
			Debug.Print("result = PASS\r\n");
			Debug.Print("accuracy = 1.2\r\n");
			Debug.Print("resultParameter1 = p1 return\r\n");
			Debug.Print("resultParameter2 = p2 return\r\n");
			Debug.Print("resultParameter3 = p3 return\r\n");
			Debug.Print("resultParameter4 = p4 return\r\n");
			Debug.Print("resultParameter5 = p5 return\r\n"); 
			*/
			while (true) {
				//System.Threading.Thread.Sleep(100);
			}	
		}
		
		
   }
}
