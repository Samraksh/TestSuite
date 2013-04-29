using System;
using Microsoft.SPOT;

namespace Samraksh.SPOT.Tests
{
    public class Parameters
    {
        public bool useLogic = false;
        
        // test specific parameters
        public double frequency = 2.5;
        public double upperAllowedFrequency = 2.7;
        public double lowerAllowedFrequency = 2.3;
        public double expectedFrequency = 2.5;
    }
}
