using System;
using Microsoft.SPOT;

namespace Samraksh.SPOT.Test
{
    public class TestFailedException : System.ApplicationException
    {
        public TestFailedException(string message)
        {
            Debug.Print("FAILED : " + message);
        }
    }

    public class TestTerminatedUnSuccessfullyException : System.ApplicationException
    {
        public TestTerminatedUnSuccessfullyException()
        {
            Debug.Print("Test Terminated Unsuccessfully");
        }

        public TestTerminatedUnSuccessfullyException(string message)
        {
            Debug.Print(message);
        }
    }
}
