using System;
using System.Threading;
using System.IO;
using System.IO.Ports;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

namespace COM
{
    public class Program
    {
        public static void Main()
        {
			//Thread.Sleep(9000);
		//	byte[] m_recvBuffer = new byte[100];
				
			Debug.Print("This is my main\r\n");

			/*string[] ports = SerialPort.GetPortNames();
			Debug.Print("Port names:\r\n");
			foreach(string port in ports)
            {
                Debug.Print(port);
            }*/

			SerialPort serialPort = new SerialPort("COM1");
			serialPort.BaudRate = 115200;
            serialPort.Parity = Parity.None;
            serialPort.StopBits = StopBits.One;
            serialPort.DataBits = 8;
            serialPort.Handshake = Handshake.None;

			serialPort.DataReceived += new SerialDataReceivedEventHandler(SerialPortHandler);

			serialPort.Open();
			byte[] testOut = new byte[10];
			for (int i=0; i<10; i++)
				testOut[i] = (byte)('a' + i);
			serialPort.Write(testOut, 0, 5);
			Debug.Print("\r\n");
			serialPort.Write(testOut, 0, 5);
			Debug.Print("\r\n");
			serialPort.Write(testOut, 0, 5);
			Debug.Print("\r\n");

			Debug.Print("result = PASS\r\n");
			Debug.Print("accuracy = 1.2\r\n");
			Debug.Print("resultParameter1 = p1 return\r\n");
			Debug.Print("resultParameter2 = p2 return\r\n");
			Debug.Print("resultParameter3 = p3 return\r\n");
			Debug.Print("resultParameter4 = p4 return\r\n");
			Debug.Print("resultParameter5 = p5 return\r\n"); 
			//int numBytes;

			while (true) {}
			/*while (true){
				numBytes = serialPort.BytesToRead;
				if (numBytes != 0)
					Debug.Print("bytes to read: " + numBytes.ToString());
				serialPort.Read(m_recvBuffer, 0, numBytes);
				for (int i=0; i<numBytes; i++)
					Debug.Print(((char)m_recvBuffer[i]).ToString());
			}*/			
		}
		
		static void SerialPortHandler(object sender, SerialDataReceivedEventArgs e)
        {            
			byte[] m_recvBuffer = new byte[100];
			//char[] charBuffer = new char[100];
			//Debug.Print("serial port handler\r\n");
            SerialPort serialPort = (SerialPort)sender;
            //string inData = serialPort.ReadExisting();
			//Debug.Print(inData);
			
			int numBytes = serialPort.BytesToRead;
            serialPort.Read(m_recvBuffer, 0, numBytes);
			serialPort.Write(m_recvBuffer, 0, numBytes);
			serialPort.Flush();
			//Debug.Print("bytes read: " + numBytes.ToString());
			/*for (int i=0; i<numBytes; i++)
				charBuffer[i] = (char)m_recvBuffer[i];
			string s = new string(charBuffer);
			Debug.Print(s);*/
            /*accumReceiveString = String.Concat(accumReceiveString, inData);

            string strippedReceive = String.Empty;
            while ((accumReceiveString.Contains("\n")) || (accumReceiveString.Contains("\r")))
            {
                strippedReceive = accumReceiveString.Substring(0, accumReceiveString.IndexOf('\n'));
                accumReceiveString = accumReceiveString.Remove(0, accumReceiveString.IndexOf('\n') + 1);
                processResponse(strippedReceive);
            }*/
        }
   }
}
