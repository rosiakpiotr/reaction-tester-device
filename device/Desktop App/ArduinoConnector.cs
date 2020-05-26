using System;
using System.IO.Ports;
using System.Management;

namespace RCClient
{
    class ArduinoConnection : SerialPort
    {
        public ArduinoConnection(SerialPort port) { }

        public ArduinoConnection() : base()
        {
            // Set port to arduino nano.
            string portName = FindSerialWith("USB-SERIAL CH340");
            if (portName != "")
            {
                PortName = portName;
                WriteTimeout = 1500;
                ReadTimeout = 1500;
                Open();
            }
        }

        private string FindSerialWith(string phrase)
        {
            ManagementObjectSearcher searcher =
                    new ManagementObjectSearcher("root\\CIMV2",
                    "SELECT * FROM Win32_PnPEntity");

            foreach (ManagementObject queryObj in searcher.Get())
            {
                if (queryObj["Caption"].ToString().Contains(phrase))
                {
                    string caption = ((String)queryObj["Caption"]);
                    return caption.Substring(caption.IndexOf("COM"), 4);
                }
            }

            return "";
        }
    }
}
