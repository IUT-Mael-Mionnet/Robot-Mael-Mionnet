using System;
using System.Collections.Generic;
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

namespace RobotInterface
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }
        int a = 0;
        void SendMessage()
        {
            string message = textBoxEmission.Text;
            textBoxReception.Text = textBoxReception.Text + "Reçu : " + message;
            textBoxEmission.Text = "";
        }
        private void buttonEnvoyer_Click(object sender, RoutedEventArgs e)
        {
            
            if (a == 1)
            {
                buttonEnvoyer.Background = Brushes.RoyalBlue;
                a = 0;
            }
            else
            {
                buttonEnvoyer.Background = Brushes.Beige;
                a = 1;
            }

            string message = textBoxEmission.Text;
            textBoxReception.Text = textBoxReception.Text + "Reçu : "  + message + "\n";
            textBoxEmission.Text = "";

        }


        private void textBoxEmission_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                SendMessage();
            }
        }
    }
}
