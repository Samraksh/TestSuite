using System;
using System.Threading;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

/* Implementation of bit-banging. This is how it works: 
 * 1. Select a device. Make the GPIO pin for CS high.
 * 2. Send bits 7 to 0. 
 *    2a. If bit is high, set the corresponding GPIO pin for MOSI high, else make it low.
 *    2b. Pulse the clock pin (make it high to low) so that the pin can be read.
 * 3. Deselect the device by making the GPIO pin for CS low.
 * 
 * Referred to    Dropbox\Dev - QualComm Docs\Library\8960\HW\MSM8960\80-N1622-1 MSM8960-MSM8660A-MSM8260A DEVICE SPECIFICATION(DQ).pdf.qck for the pins
 */

namespace KraitSPITesting
{
    public class Program
    {
        /* (from Dropbox\Adapt\SmartFile\EVT2\Adapt_Core_Module_Documentation\8XXA01_V1.02_DL(120907).pdf -- page 14)
         * FPGA_SPI_DIN is GPIO6
         * FPGA_SPI_DOUT is GPIO7
         * FPGA_SPI_CS_N is GPIO8
         * FPGA_SPI_SCLK is GPIO9
         */
        /*private static OutputPort SpiCLK = new OutputPort(Cpu.Pin.GPIO_Pin9, true);
        private static OutputPort SpiCS = new OutputPort(Cpu.Pin.GPIO_Pin8, true);
        private static OutputPort SpiMISO = new OutputPort(Cpu.Pin.GPIO_Pin7, true);
        private static OutputPort SpiMOSI = new OutputPort(Cpu.Pin.GPIO_Pin6, true);*/




        /* Below pin details are from the docs - 
         *                  1. Dropbox\Adapt\SmartFile\EVT2\Adapt_Expansion_Board_Documentation\8XXA01-B_V1.02_DL(120907).pdf   - page 8 
         *                  2. Dropbox\Adapt\Hardware\ADAPT Product Guide v1p0.pdf                                              - page 41
         */

        private static OutputPort SpiCLK = new OutputPort((Cpu.Pin)58, true);   
                          //Pin is called MSM_GPIO1_DEBUG. On the Adapt dev board, it is called M_GPIO1.
                          //This also maps to GPIO_58, but the pin# is 41 (from Dropbox\Adapt\SmartFile\EVT2\Adapt_Core_Module_Documentation\8XXA01_V1.02_DL(120907).pdf -- page 14)
        private static OutputPort SpiCS = new OutputPort((Cpu.Pin)55, true);    
                          //Pin is called MSM_GPIO2_DEBUG. On the Adapt dev board, it is called M_GPIO2. 
                          //This also maps to GPIO_55, but the pin# is 43 (from Dropbox\Adapt\SmartFile\EVT2\Adapt_Core_Module_Documentation\8XXA01_V1.02_DL(120907).pdf -- page 14)
        private static OutputPort SpiMISO = new OutputPort((Cpu.Pin)53, true);  
                          //Pin is called MSM_GPIO3_DEBUG. On the Adapt dev board, it is called M_GPIO3. 
                          //This also maps to GPIO_53, but the pin# is 45 (from Dropbox\Adapt\SmartFile\EVT2\Adapt_Core_Module_Documentation\8XXA01_V1.02_DL(120907).pdf -- page 14)
        private static OutputPort SpiMOSI = new OutputPort((Cpu.Pin)52, true);  
                          //Pin is called MSM_GPIO4_DEBUG. On the Adapt dev board, it is called M_GPIO4.
                          //This also maps to GPIO_52, but the pin# is 49 (from Dropbox\Adapt\SmartFile\EVT2\Adapt_Core_Module_Documentation\8XXA01_V1.02_DL(120907).pdf -- page 14)

        public static void Main()
        {
            byte data;
            byte value;
            byte constant = 128; //0x80
            
            while (true)
            {

                data = 170; //0xAA

                SpiCS.Write(true);

                for (int index = 0; index < 8; ++index)
                {
                    //Debug.Print("data is: " + data.ToString("X") + "\n");
                    value = (byte)(data & constant);
                    //Debug.Print("value is: " + value.ToString("X") + "\n");
                    
                    if (value == constant)
                    {
                        //Debug.Print("Writing 1 \n");
                        SpiMOSI.Write(true);
                    }
                    else
                    {
                        //Debug.Print("Writing 0 \n");
                        SpiMOSI.Write(false);
                    }

                    SpiCLK.Write(true);
                    SpiCLK.Write(false);

                    data <<= 1;
                    
                    //Thread.Sleep(10);
                }

                SpiCS.Write(false);

                Thread.Sleep(250);
            }
        }

    }
}
