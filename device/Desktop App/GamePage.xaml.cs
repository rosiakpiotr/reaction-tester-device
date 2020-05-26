using System.Windows.Controls;
using System;
using System.Windows.Media.Imaging;
using System.ComponentModel;

namespace RCClient
{
    public delegate void EndGameCallback(Statistics playerStats);

    public partial class GamePage : Page
    {
        public EndGameCallback OnGameFinish;
        public ConnectionManager ConnectionManager { get; private set; }
        private Statistics PlayerStats;

        public GamePage(string nickname, ConnectionManager connectionManager)
        {
            InitializeComponent();
            NicknameTextBlock.Text = nickname;
            PlayerStats = new Statistics(nickname);
            this.ConnectionManager = connectionManager;
            // Set callbacks, what should happen when arduino connected / disconnected.
            connectionManager.ConnectedCallback = () =>
            {
                // This executed when arduino gets plugged in.
                Dispatcher.BeginInvoke(new Action(() =>
                {
                    GreenDotImage.Visibility = System.Windows.Visibility.Visible;
                    RedDotImage.Visibility = System.Windows.Visibility.Hidden;
                }));
            };

            connectionManager.DisconnectedCallback = () =>
            {
                Dispatcher.BeginInvoke(new Action(() =>
                {
                    GreenDotImage.Visibility = System.Windows.Visibility.Hidden;
                    RedDotImage.Visibility = System.Windows.Visibility.Visible;
                }));
            };

            // Check for connection to show red or green indicator instantly.
            connectionManager.Refresh();

            int currentAttempt = 0;
            connectionManager.OnMessageReceive = (string data) =>
            {
                System.Diagnostics.Trace.WriteLine(data);

                if (data != null)
                {
                    if (data == "2001")
                    {
                        Dispatcher.Invoke(new Action(() =>
                        {
                            StackPanel panel = (StackPanel)LEDSGrid.Children[currentAttempt];
                            ((Image)(panel.Children[0])).Source = new BitmapImage(new Uri("Resources/yellow_diode.png", UriKind.Relative));
                        }));
                    }

                    if (data.Contains("2010"))
                    {
                        Dispatcher.Invoke(new Action(() =>
                        {
                            StackPanel panel = (StackPanel)LEDSGrid.Children[index: currentAttempt];
                            ((Image)(panel.Children[0])).Source = new BitmapImage(new Uri("Resources/green_diode.png", UriKind.Relative));
                            ((TextBlock)panel.Children[2]).Visibility = System.Windows.Visibility.Visible;
                            ((TextBlock)panel.Children[2]).Text = data.Substring(data.IndexOf('[') + 1) + " ms";
                            PlayerStats.Timings[currentAttempt++] = int.Parse(data.Substring(data.IndexOf('[') + 1));
                        }));
                    }

                    if (data == "2009")
                    {
                        Dispatcher.Invoke(new Action(() =>
                        {
                            StackPanel panel = (StackPanel)LEDSGrid.Children[index: currentAttempt];
                            ((Image)(panel.Children[0])).Source = new BitmapImage(new Uri("Resources/red_diode.png", UriKind.Relative));
                            PlayerStats.Timings[currentAttempt++] = -1;
                        }));
                    }

                    if (currentAttempt + 1 > 5)
                    {
                        OnAllAttemptsUsed();
                        connectionManager.OnMessageReceive = null;
                        return;
                    }

                } // if data != null
            };
        }

        private void StartNewGame(int delay)
        {
            // Check if there is connection.
            if (ConnectionManager.IsConnected)
            {
                // 2000 is code saying "Start game"
                var delayedNewGame = new BackgroundWorker();
                delayedNewGame.DoWork += (object sender, DoWorkEventArgs e) =>
                {
                    System.Threading.Thread.Sleep(delay);
                    ConnectionManager.Send("2000");
                };
                delayedNewGame.RunWorkerAsync();
            }
        }

        private void ReadyButton_Click(object sender, System.Windows.RoutedEventArgs e)
        {
            // Hide this button and nickname and show leds.
            IamReadyGrid.Visibility = System.Windows.Visibility.Collapsed;
            NicknameTextBlock.Visibility = System.Windows.Visibility.Collapsed;

            LEDSGrid.Visibility = System.Windows.Visibility.Visible;
            StartNewGame(new Random().Next(1000, 2000));
        }

        private void OnAllAttemptsUsed()
        {
            // Reset diodes with some nice animation(4 available animations).
            System.Threading.Thread.Sleep(100);
            ConnectionManager.Send("2999");

            // If line below is commented, we don't send animation.
            //ConnectionManager.Send(new Random().Next(3000, 3005).ToString());

            Dispatcher.Invoke(new Action(() =>
            {
                ConfirmLeaveButton.Visibility = System.Windows.Visibility.Visible;
            }));
        }

        private void ConfirmLeaveButton_Click(object sender, System.Windows.RoutedEventArgs e) => OnGameFinish(PlayerStats);
    }
}
