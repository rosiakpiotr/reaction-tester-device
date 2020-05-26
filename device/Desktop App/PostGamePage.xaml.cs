using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace RCClient
{
    public partial class PostGamePage : Page
    {
        ConnectionManager connectionManager { get; set; }

        public PostGamePage(List<Statistics> statistics, ConnectionManager connectionManager)
        {
            InitializeComponent();

            this.connectionManager = connectionManager;

            foreach (Statistics s in statistics)
            {
                s.CalculateMissedCount();
            }

            var sorted = statistics.OrderBy(stats => stats.BestTime()).ThenBy(stats => stats.AverageTimings()).ThenBy(stats => stats.MissedCount).ToList();
            var rowGroup = Table.RowGroups.FirstOrDefault();

            foreach (Statistics stats in sorted)
            {
                var row = new TableRow
                {
                    Background = Brushes.Wheat
                };

                row.Cells.Add(new TableCell(new Paragraph(new Run(stats.Nickname))) { ColumnSpan = 2, TextAlignment = TextAlignment.Center });
                
                foreach(int timing in stats.Timings)
                {
                   row.Cells.Add(new TableCell(new Paragraph(new Run(timing == -1 || timing == 0 ? ("missed") : (timing.ToString() + " ms")))) { TextAlignment = TextAlignment.Center });
                }

                row.Cells.Add(new TableCell(new Paragraph(new Run($"{stats.AverageTimings()} ms"))) { TextAlignment = TextAlignment.Center });
                row.Cells.Add(new TableCell(new Paragraph(new Run(stats.BestOne()))) { ColumnSpan = 2, TextAlignment = TextAlignment.Center });

                rowGroup.Rows.Add(row);
            }//*/
        }

        private void GoBackToHomeScreen_Click(object sender, RoutedEventArgs e)
        {
            NavigationService.Navigate(new HomePage(connectionManager));
        }
    }
}
