using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Diagnostics;
using System.Threading;
using System.IO.Ports;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System;

namespace RCClient
{
    public partial class MainWindow : Window
    {
        private ConnectionManager connectionManager;

        public MainWindow()
        {
            InitializeComponent();
            connectionManager = new ConnectionManager();
            Main.Content = new HomePage(connectionManager);
        }

        public bool IsClosed { get; private set; }

        // A la destructor.
        protected override void OnClosed(EventArgs e)
        {
            connectionManager.Quit();

            base.OnClosed(e);
            IsClosed = true;
        }
    }
}
