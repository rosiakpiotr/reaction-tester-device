using System.Threading;
using System.IO.Ports;
using System;
using System.Windows;

namespace RCClient
{
    public delegate void Callback();
    public delegate void MessageCallback(string arg);

    public class ConnectionManager
    {
        private volatile bool runChecker;
        private volatile bool refreshRequested;
        public bool IsConnected { get; private set; }
        private ArduinoConnection arduinoConnection;

        // By default don't do anything.
        public Callback ConnectedCallback { get; set; }
        public Callback DisconnectedCallback { get; set; }
        public MessageCallback OnMessageReceive { get; set; }

        public ConnectionManager()
        {
            TryConnect();

            if (arduinoConnection == null)
            {
                MessageBox.Show("Arduino is connected but another app is using it. Close the other program and retry.", "Error!");
                Environment.Exit(0);
            }

            Thread t = new Thread(new ThreadStart(CheckConnection));
            t.Start();
            arduinoConnection.DataReceived += new SerialDataReceivedEventHandler(
            (object sender, SerialDataReceivedEventArgs a) =>
            {
                if (Monitor.TryEnter(arduinoConnection))
                {
                    try
                    {
                        OnMessageReceive?.Invoke(Receive());
                    }
                    finally
                    {
                        if (Monitor.IsEntered(arduinoConnection))
                            Monitor.Exit(arduinoConnection);
                    }
                }
            });
        }

        private void CheckConnection()
        {
            runChecker = true;
            refreshRequested = true;

            while (runChecker)
            {
                if (Monitor.TryEnter(arduinoConnection))
                {
                    try
                    {
                        if (!Send("1000"))
                        {
                            DisconnectedCallback?.Invoke();
                            TryConnect();
                            IsConnected = false;
                        }
                        else
                        {
                              ConnectedCallback?.Invoke();
                              IsConnected = true;

                        }
                    }
                    finally
                    {
                        if (Monitor.IsEntered(arduinoConnection))
                            Monitor.Exit(arduinoConnection);
                    }
                }

                int sleeptFor = 0;
                while (++sleeptFor < 2000)
                {
                    if (refreshRequested)
                    {
                        refreshRequested = false;
                        break;
                    }
                    Thread.Sleep(1);
                }
            }
        }

        private void TryConnect()
        {
            try
            {
                arduinoConnection = new ArduinoConnection();
            }
            catch(Exception e) {
                //arduinoConnection = new ArduinoConnection(new SerialPort()); // Give empty serial to prevent errors.
                System.Diagnostics.Trace.WriteLine(e.StackTrace);
            }
        }

        // Sends string with new line mark to arduino.
        public bool Send(string message)
        {
            if (arduinoConnection.IsOpen)
            {
                try
                {
                    arduinoConnection.WriteLine(message);
                    return true;
                }
                catch (Exception e)
                {
                    System.Diagnostics.Trace.WriteLine(e.StackTrace);
                }
            }

            return false;
        }

        // Returns string received from arduino (reading until new line mark is found).
        public string Receive()
        {
            try
            {
                // Arduino sends new line mark at the end. Get rid of it here.
                string income = arduinoConnection.ReadLine();
                return income.Substring(0, income.Length - 1);
            } catch (Exception e)
            {
                System.Diagnostics.Trace.WriteLine(e.StackTrace);
                return null;
            }
        }

        // Stops everything.
        public void Quit()
        {
            if (arduinoConnection != null) arduinoConnection.Close();
            runChecker = false; // terminates 'while' loop.
        }

        // Sets request flag, so thread can check connection with arduino when possible.
        public void Refresh()
        {
            refreshRequested = true;
        }
    }
}
