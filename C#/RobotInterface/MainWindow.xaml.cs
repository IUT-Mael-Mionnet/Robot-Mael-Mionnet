using ExtendedSerialPort;
using System;
using System.Collections.Generic;
using System.IO.Ports;
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
using System.Windows.Threading;

namespace RobotInterface
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>

    public partial class MainWindow : Window
    {
        ReliableSerialPort serialPort1;
        AsyncCallback SerialPort1_DataReceived;
        DispatcherTimer timerAffichage;

        Robot robot = new Robot();
        
        public MainWindow()
        {
            InitializeComponent();            
            serialPort1 = new ReliableSerialPort("COM4", 115200, Parity.None, 8, StopBits.One);
            serialPort1.DataReceived += SerialPort1_DataReceived1;
            serialPort1.Open();

            timerAffichage = new DispatcherTimer();
            timerAffichage.Interval = new TimeSpan(0, 0, 0, 0, 100);
            timerAffichage.Tick += TimerAffichage_Tick;
            timerAffichage.Start();
            
            robot.receivedText = "";
        }

        private void SerialPort1_DataReceived1(object sender, DataReceivedArgs e)
        {
            robot.receivedText += Encoding.UTF8.GetString(e.Data, 0, e.Data.Length);
            for (int j = 0; j < e.Data.Length; j++)
            {
                robot.byteListReceived.Enqueue(e.Data[j]);
            }
        }

        byte CalculateChecksum (int msgFunction, int msgPayloadLength, byte[] msgPayload) 
        {
            byte checksum;
            checksum = 0xFE;
            checksum ^= (byte)(msgFunction >> 8);
            checksum ^= (byte)(msgFunction >> 0);
            checksum ^= (byte)(msgPayloadLength >> 8);
            checksum ^= (byte)(msgPayloadLength >> 0);
            for(int i = 0; i < msgPayloadLength; i++)
            {
                checksum ^= (byte)(msgPayload [i]);
            }
            // soit a est la valeur qui fait le ou exclusif de SOF,COMMAND,PAYLOADLENGTH et PAYLOAD
            return checksum;
            //on retourne la valeur de l'octet Checksum.
        }

        void UartEncodeAndSendMessage (int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            // Ici on envoi un message qui comprend SOF, Command, PayloadLength, Payload et CheckSum.
            byte checksum = CalculateChecksum(msgFunction, msgPayloadLength, msgPayload);
            byte[] msg = new byte[6 + msgPayloadLength];
            int pos = 0;
            msg[pos++] = 0xFE;
            msg[pos++] = (byte)(msgFunction >> 8);
            msg[pos++] = (byte)(msgFunction >> 0);
            msg[pos++] = (byte)(msgPayloadLength >> 8);
            msg[pos++] = (byte)(msgPayloadLength >> 0);
            for(int i = 0; i < msgPayloadLength; i++)
            {
                msg[pos++] = msgPayload[i];
            }
            msg[pos++] = checksum;
            serialPort1.Write(msg, 0, msg.Length);
        }

        int a = 0;

        public object SerialPort1 { get; private set; }

        void SendMessage()
        {
            serialPort1.WriteLine(textBoxEmission.Text);
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

            SendMessage();

        }

        private void textBoxEmission_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                SendMessage();
            }
        }
        
        private void TimerAffichage_Tick(object sender, EventArgs e)
        {
            if (robot.receivedText != "")
            {
                robot.receivedText = "";
            }
            while (robot.byteListReceived.Count != 0)
            {
                byte byteReceived = robot.byteListReceived.Dequeue();
                string blabla;
                blabla = "0x" + byteReceived.ToString("X2")+" ";
                textBoxReception.Text += blabla;
            }
            
        
        }

        private void boutonClear_Click(object sender, RoutedEventArgs e)
        {
            textBoxReception.Text = "";
        }

        private void boutonTest_Click(object sender, RoutedEventArgs e)
        {
            //byte [] byteList = new byte [20];
            //for (int i = 0; i < 20; i++)
            //{
            //    byteList[i] = (byte)(2 * i);
            //}
            //serialPort1.Write(byteList, 0, byteList.Length);

            int msgFunction, msgPayloadLength;
            msgFunction = 0x0080;
            byte[] msgPayload = Encoding.ASCII.GetBytes(textBoxEmission.Text);
            msgPayloadLength = msgPayload.Length;
            UartEncodeAndSendMessage (msgFunction, msgPayloadLength, msgPayload);
            textBoxEmission.Text = "";
        }
    }
}
