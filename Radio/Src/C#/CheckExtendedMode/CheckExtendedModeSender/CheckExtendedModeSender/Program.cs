﻿/*--------------------------------------------------------------------
 * CSMA Radio Ping-Pong: app note for the eMote .NOW
 * (c) 2013-2015 The Samraksh Company
 * 
 * Version history
 *      1.0:	- Initial release
 *      1.1:	- Upgraded to v. 12 namespaces
 *				- Various cleanup and refactoring
 *		1.2:    - Changes to include eMote v.13 changes (Sep 17, 2015)
---------------------------------------------------------------------*/

using System;
using System.Reflection;
using System.Threading;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

using Samraksh.eMote.Net;
using Samraksh.eMote.Net.Mac;
using Samraksh.eMote.Net.Radio;

namespace Samraksh.DotNow.PingPong {

    /// <summary>
    /// Set up two motes to send and receive messages between each other.
    /// The goal is to converge on a shared value.
    /// Each mote randomly chooses an initial current value and sends it to the other.
    /// When a mote receives a message, it sets its current value to the maximum of the two and then increments it by 1, and displays it on the LCD.
    /// After a delay, the mote sends the new value. The delay is to keep the LCD display from changing too fast.
    /// 
    /// If a mote does not hear from the other, then after a (longer) delay, it resends its current value.
    /// It does this repeatedly, at the longer interval.
    /// </summary>
    public class Program {

		// This is used as a header for the packet payload to identify the app
        const string Header = "PingPong";

        const string Payload = "This is a long string repeated many times over";
        //static byte[] Payload = new byte[1000];
        //static byte[] merged = new byte[Header.Length + Payload.Length];

        // The current value
        static int _currVal;

        // LCD and Radio objects
        static SimpleCsmaRadio _csmaRadio;

        // Reply timer. Slows down interaction by not sending reply messages until the timer expires
        const int SendInterval = 4000; // Time to wait before sending reply
        //static Timer _replyTimer;
        static readonly TimerCallback ReplyTimerCallback = reply_Timeout;

        // No response timer. If no message received, send current value again
        //  Timer is reset whenever a message is received
        const int NoResponseInterval = SendInterval * 4; // Time to wait before re-sending; must be larger than send interval
        static Timer _noResponseDelayTimer;
        static readonly TimerCallback NoResponseDelayTimerCallback = noResponseDelay_Timeout;

        static Timer _sendTimer, _retryTimer;
        static readonly TimerCallback SendTimerCallback = RadioSendHelper;
        static readonly TimerCallback RetryTimerCallback = RetrySendHelper;
        const int sendInterval = 3000;
        const int retryInterval = 5;

        //static bool toggle = true;

        static int counter = 0;

        static Int32 currentTry = 0;
        const Int32 retryLimit = 2;

        private static OutputPort testPort_PB8 = new OutputPort(GPIOPins.GPIO_PIN_PB8, true);
        private static OutputPort testPort_PA1 = new OutputPort(GPIOPins.GPIO_PIN_PA1, true);
        
        /// <summary>
        /// Main program. Set things up and then go to sleep forever.
        /// </summary>
        public static void Main() {

            Debug.EnableGCMessages(false);  // We don't want to see garbage collector messages in the Output window

			Debug.Print(VersionInfo.VersionBuild(Assembly.GetExecutingAssembly()));

            Debug.Print("I am the sender");
                        
            // Pick a value randomly
            //_currVal = (new Random()).Next(99);  // We're choosing a fairly small value to avoid runover on the LCD display (since it only has 4 positions)
            
            // Set up the radio for CSMA interaction
            //  The first two arguments are fairly standard but you're free to try changing them
            //  The last argument is the method to call when a message is received
            _csmaRadio = new SimpleCsmaRadio(140, TxPowerValue.Power_0Point7dBm, RadioReceive);

            // Send the current value now
            //RadioSend(_currVal.ToString().Trim());

            // Start a one-shot timer that resets itself whenever it expires
            //StartOneshotTimer(ref _noResponseDelayTimer, NoResponseDelayTimerCallback, NoResponseInterval);

            _sendTimer = new Timer(SendTimerCallback, null, sendInterval, sendInterval);
            _retryTimer = new Timer(RetryTimerCallback, null, Timeout.Infinite, Timeout.Infinite);

            /*for (int i = 0; i < Payload.Length; i++)
            {
                Payload[i] = (byte)i;
            }*/

            // Everything is set up. Go to sleep forever, pending events
            Thread.Sleep(Timeout.Infinite);
        }

        /// <summary>
        /// Handle a received message
        /// </summary>
        /// <param name="csma">A CSMA object that has the message info</param>
        static void RadioReceive(CSMA csma) {
            //
            // Check to be sure it's a message we're interested in
            //
            testPort_PA1.Write(true);
            testPort_PA1.Write(false);
            
            //Stop the 1-shot timer
            _retryTimer.Change(Timeout.Infinite, Timeout.Infinite);
            //Received an ack for previous msg, send a new one
            currentTry = 0;
            Debug.Print("Sender got back a msg");
            // Check if there's at least one packet
            if (csma.GetPendingPacketCount() < 1) {
                return;
            }
            // Check to be sure there's something in the packet
            Message packet = csma.GetNextPacket();
            if (packet == null) {
                return;
            }
            
            //toggle = true;
            //_sendTimer.Change(sendInterval, Timeout.Infinite);
            // Check if message is for us
            /*var msgByte = packet.GetMessage();
            var msgChar = System.Text.Encoding.UTF8.GetChars(msgByte);
            var msgStr = new string(msgChar);
            if (msgStr.Substring(0, Header.Length) != Header) {
                return;
            }
            // Get payload and check if it is in the correct format (an integer)
            string payload = msgStr.Substring(Header.Length);
            int recVal;
            try {
                recVal = Int32.Parse(payload);
            }
            catch {
                return;
            }

            //
            // We've received a correct message
            //

            // Reset the no-response timer
            //StartOneshotTimer(ref _noResponseDelayTimer, NoResponseDelayTimerCallback, NoResponseInterval);
            //Stop timer
            //_noResponseDelayTimer.Change(Timeout.Infinite, Timeout.Infinite);

            // Update the current value
            int origVal = _currVal;
            _currVal = System.Math.Max(_currVal, recVal);
            _currVal++;
            Debug.Print("Orig val " + origVal + ", rec val " + recVal + ", new val " + _currVal);*/

            // Wait a bit before sending reply
            //StartOneshotTimer(ref _replyTimer, ReplyTimerCallback, SendInterval);
        }

        static void RetrySendHelper(object obj)
        {
            currentTry++;
        }

        static void RadioSendHelper(object obj){
            if (currentTry == 0 || currentTry >= retryLimit)
            {
                // Pick a value randomly
                _currVal = (new Random()).Next(99);  // We're choosing a fairly small value to avoid runover on the LCD display (since it only has 4 positions)
                //Debug.Print("_currVal is " + _currVal);
                if (currentTry >= retryLimit)
                {
                    currentTry = 0;
                }
            }

            //if (toggle)
            if(currentTry < retryLimit)
            {
                //toggle = false;
                Debug.Print("Sending message " + _currVal + "; Attempt " + currentTry);
                testPort_PB8.Write(true);
                RadioSend(_currVal.ToString().Trim());
                testPort_PB8.Write(false);
                //Start the 1-shot timer that will be stopped if there is a hardware ack
                _retryTimer.Change(retryInterval, Timeout.Infinite);

                //_sendTimer.Change(Timeout.Infinite, Timeout.Infinite);
                //Below for loop takes 480ms (450 times Header, payload and number transmitted) to complete
                /*for (int i = 0; i < 450; i++)
                {
                    if (i == 0)
                    {
                        Debug.Print("Sending message " + _currVal);
                    }
                    // Send the current value now
                    //Each send takes about 18 usec (56 bytes - payload (46) + header (8) + number (2))
                    //There is a gap of about 1 ms between each send
                    RadioSend(_currVal.ToString().Trim());
                    //Thread.Sleep(1);
                }*/
                /*var startTime = System.DateTime.Now;
                RadioSend(_currVal.ToString().Trim());
                var endTime = System.DateTime.Now;
                Debug.Print("Elapsed time " + (endTime - startTime));*/
            }
            /*else
            {
                toggle = true;
                //Debug.Print("Going quiet");
                //_sendTimer.Change(sendInterval, Timeout.Infinite);
            }*/
        }

        /// <summary>
        /// Send a message
        /// </summary>
        /// <remarks>It will be preceded by the HEADER</remarks>
        /// <param name="toSend">String to be sent</param>
        static void RadioSend(string toSend) {
            byte[] toSendByte = System.Text.Encoding.UTF8.GetBytes(Header + toSend + Payload);
            if (counter == 0)
            {
                counter++;
                //Debug.Print("send length " + toSendByte.Length.ToString());
            }
            _csmaRadio.Send(Addresses.BROADCAST, toSendByte);
        }

        /// <summary>
        /// Send the current value when the reply timer expires
        /// </summary>
        /// <param name="obj">Ignored</param>
        static void reply_Timeout(object obj) {
            RadioSend(_currVal.ToString().Trim());
            Debug.Print("Sending message " + _currVal);
        }

        /// <summary>
        /// Resend the current value when the no-response timer expires
        /// </summary>
        /// <param name="obj">Ignored</param>
        static void noResponseDelay_Timeout(object obj) {
            RadioSend(_currVal.ToString().Trim());
            // Give a short interruption to show that we've received no response
            Thread.Sleep(500);
            // Restart the no-response timer & display a message
            StartOneshotTimer(ref _noResponseDelayTimer, NoResponseDelayTimerCallback, NoResponseInterval);
            Debug.Print("No message received ... broadcasting again");
        }

        /// <summary>
        /// Start (or restart) a one-shot timer
        /// </summary>
        /// <remarks>It will run once and then stop</remarks>
        /// <param name="timer">The timer</param>
        /// <param name="callBack">The timer's callback</param>
        /// <param name="interval">The interval</param>
        static void StartOneshotTimer(ref Timer timer, TimerCallback callBack, int interval) {
            if (timer == null) {
                timer = new Timer(callBack, null, interval, Timeout.Infinite);
            }
            else {
                timer.Change(interval, Timeout.Infinite);
            }
        }

    }
}

