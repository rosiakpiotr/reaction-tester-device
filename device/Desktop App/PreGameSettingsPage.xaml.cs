using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;

namespace RCClient
{
    public partial class PreGameSettingsPage : Page
    {
        private ConnectionManager connectionManager;
        private int PlayersCount { get; set; }

        public PreGameSettingsPage(ConnectionManager connectionManager)
        {
            InitializeComponent();
            PlayersCount = 1;
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
                Dispatcher.BeginInvoke(new Action(() =>
                {
                    GreenDotImage.Visibility = Visibility.Hidden;
                    RedDotImage.Visibility = Visibility.Visible;
                }));
            };

            // Check for connection to show red or green indicator.
            connectionManager.Refresh();
        }

        private void PlayersCButtonI_Click(object sender, RoutedEventArgs e)
        {
            if (PlayersCount < 10)
            {
                PlayersCount++;
            }

            PlayersCTextBlock.Text = PlayersCount.ToString();
        }

        private void PlayersCButtonD_Click(object sender, RoutedEventArgs e)
        {
            if (PlayersCount > 1)
            {
                PlayersCount--;
            }

            PlayersCTextBlock.Text = PlayersCount.ToString();
        }

        private void ConfirmButton_Click(object sender, RoutedEventArgs e)
        {
            List<string> nicknames = new List<string>(PlayersCount);

            for (int i = 0; i < PlayersCount; i++)
            {
                nicknames.Add($"Player { i + 1 }");
            }

            new GamesManager(nicknames, NavigationService).ShowGames(connectionManager);
        }
    }

    class GamesManager
    {
        private int currentPlayer;
        private int playersMax;
        private List<string> nicknames;
        private NavigationService navigationService;
        private List<Statistics> statistics;

        public GamesManager(List<string> nicknames, NavigationService navigationService)
        {
            playersMax = nicknames.Count;
            this.navigationService = navigationService;
            this.nicknames = nicknames;
            statistics = new List<Statistics>();
            currentPlayer = 0;
        }

        public void ShowGames(ConnectionManager connectionManager)
        {
            GamePage gp = new GamePage(nicknames[currentPlayer++], connectionManager)
            {
                OnGameFinish = (Statistics playerStats) =>
                {
                    statistics.Add(playerStats);
                    
                    // Start new game if there is any more players.
                    if (currentPlayer < playersMax)
                    {
                        ShowGames(connectionManager);
                    }
                    else
                    {
                        navigationService.Navigate(new PostGamePage(statistics, connectionManager));
                    }
                }
            };
            navigationService.Navigate(gp);
        }
    }

    public class Statistics
    {
        public string Nickname { get; set; }
        public int[] Timings { get; set; }
        public int MissedCount { get; private set; }
        
        public Statistics(string nickname) { Nickname = nickname; Timings = new int[5]; MissedCount = 0; }

        public void CalculateMissedCount()
        {
            Timings.Sum(number => { if (number == -1) MissedCount++; return 0; });
        }

        public float AverageTimings()
        {
            return MissedCount < Timings.Length ? 
                Timings.Sum(number => { if (number == -1) return 0; return number; }) / (Timings.Length - MissedCount) 
                : 0.0f;
        }

        public string BestOne()
        {
            int best = BestTime();
            if (best == -1) { return "missed all :/"; }
            return $"{ best } ms in { Array.IndexOf(Timings, best) + 1 } attempt";
        }

        public int BestTime()
        {
            int min = Timings.Min(num => { if (num != -1) return num; return 0xFFFFFFF; });
            if (min == 0xFFFFFFF) { return -1; }
            return min;
        }
    }
}
