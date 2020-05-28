using System;
using System.Windows;
using System.Windows.Controls;

namespace RCClient
{
    public partial class HomePage : Page
    {
        private ConnectionManager connectionManager;

        public HomePage(ConnectionManager connectionManager)
        {
            InitializeComponent();

            this.connectionManager = connectionManager;

            // Set callbacks, what should happen when arduino connected / disconnected.
            connectionManager.ConnectedCallback = () =>
            {
                // This executed when arduino gets plugged in.
                //Trace.WriteLine("Arduino connected");
                Dispatcher.BeginInvoke(new Action(() =>
                {
                    GreenDotImage.Visibility = Visibility.Visible;
                    RedDotImage.Visibility = Visibility.Hidden;
                }));
            };

            connectionManager.DisconnectedCallback = () =>
            {

                // This executed when arduino gets unplugged.
                //Trace.WriteLine("Arduino DC");
                Dispatcher.BeginInvoke(new Action(() =>
                {
                    GreenDotImage.Visibility = Visibility.Hidden;
                    RedDotImage.Visibility = Visibility.Visible;
                }));
            };

            // Check for connection to show red or green indicator.
            connectionManager.Refresh();
        }

        private void OnSettings_Click(object sender, RoutedEventArgs e)
        {
            // Start new page showing settings.
            NavigationService.Navigate(new AboutPage());
        }

        private void PlayButton_Click(object sender, RoutedEventArgs e)
        {
            // Start new page with pre settings to game.
            NavigationService.Navigate(new PreGameSettingsPage(connectionManager));
        }
    }
}
