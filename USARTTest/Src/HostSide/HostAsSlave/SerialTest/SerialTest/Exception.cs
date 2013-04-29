using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SerialTest
{
    public class UnableToOpenPortException : System.Exception
    {
        public UnableToOpenPortException() : base() { }
        public UnableToOpenPortException(string message) : base(message) { }
    }
}
