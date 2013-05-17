using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MFDeployTester
{
    public class UnabletToConfigureComPortException : Exception
    {
        public UnabletToConfigureComPortException() : base("Unable to Confifure the Com Port\n") { }
    }
}
