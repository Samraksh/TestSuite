#define SI4468
//#define RF231

using System;
using Microsoft.SPOT;
using Samraksh.eMote.Net;
using Samraksh.eMote.Net.MAC;
using Samraksh.eMote.Net.Radio;

namespace Samraksh.eMote.Net.Mac.TestUnInit.Receive
{
	/// <summary>
	/// Handle CSMA radio communication
	/// To keep it simple, we ignore neighborhood changes
	/// </summary>
	public class SimpleCsmaRadio {

		// Set up for callback to user method to handle incoming packets
		public delegate void RadioReceivedData(CSMA csma);

		readonly RadioReceivedData _radioReceivedData;

		// CSMA object that's created & passed back to the user.
		readonly CSMA _csma;
        
        //public Radio_802_15_4_Base radioBase;

		/// <summary>
		/// CSMA radio constructor without neighbor change callback
		/// </summary>
		/// <param name="ccaSensetime">CCA sense time, in ms</param>
		/// <param name="radioReceivedData">Method to call when data received. Can be null if user does not want to be notified of received messages</param>
		public SimpleCsmaRadio(byte ccaSensetime, RadioReceivedData radioReceivedData) {
			/*var macConfig = new MACConfiguration {
				NeighborLivenessDelay = 100, // Neighbor timeout. Neighbor changes are ignored but we still have to specify a value
				CCASenseTime = ccaSensetime
			};*/

            //MacConfiguration macConfig = new MacConfiguration();
            //macConfig.NeighborLivenessDelay = 100;

			//macConfig.radioConfig.SetTxPower(txPowerValue); // This is probably optional
            _radioReceivedData = radioReceivedData;
#if (RF231)
            var radioConfig = new RF231RadioConfiguration(RF231TxPower.Power_3dBm, RF231Channel.Channel_13);
#endif
#if (SI4468)
            var radioConfig = new SI4468RadioConfiguration(SI4468TxPower.Power_20dBm, SI4468Channel.Channel_01);
#endif

            //Radio_802_15_4_Base.Configure(macConfig.radioConfig);
            ////Radio_802_15_4_Base.Config = new RadioConfiguration(macConfig.radioConfig);
            ////radioBase = new Radio_802_15_4_Base();
            
			try {
                _csma = new CSMA(radioConfig);
				//MACBase.Configure(macConfig, Receive, NeighborChange); // Set up MAC base with the MAC configuration, receive callback and neighbor change callback (which does nothing)
				//_csma = CSMA.Instance;
			}
			catch (Exception e) {
				Debug.Print("CSMA configuration error " + e);
			}

            Debug.Print("CSMA address is :  " + _csma.MACRadioObj.RadioAddress.ToString());
		}

		/// <summary>
		/// Send a message
		/// </summary>
		/// <param name="msgType">Message type: broadcast ... </param>
		/// <param name="message">Message to be sent, as a byte array</param>
        public void Send(AddressType address, byte[] message)
        {
            _csma.Send((ushort)address, PayloadType.MFM_Data, message, 0, (ushort)message.Length);
		}

        public bool CCA(Radio_802_15_4_Base radioBase)
        {
            return radioBase.ClearChannelAssesment(0);
        }

        public bool TurnOnRx(Radio_802_15_4_Base radioBase)
        {
			return true;
            //return radioBase.TurnOnRx(0);
        }

		/// <summary>
		/// Callback when neighborhood changes
		/// </summary>
		/// <remarks>
		/// We are ignoring neighborhood changes so this method does nothing
		/// </remarks>
		/// <param name="numberOfNeighbors"></param>
		private static void NeighborChange(UInt16 numberOfNeighbors) {
		}

		/// <summary>
		/// Callback when radio message received
		/// </summary>
		/// <remarks>
		/// If user callback is not null then call with CSMA object
		/// </remarks>
		/// <param name="numberOfPackets"></param>
		private void Receive(UInt16 numberOfPackets) {
			// If the user doesn't want to be notified of received messages, return
			if (_radioReceivedData == null) {
				return;
			}
			// Send the CSMA object to the user.
			// No need to send numberOfPackets; that's available as CSMA.GetPendingPacketCount
			_radioReceivedData(_csma);
		}
	}
}
