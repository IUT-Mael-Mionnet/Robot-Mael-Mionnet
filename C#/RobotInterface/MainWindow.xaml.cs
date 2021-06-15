﻿using ExtendedSerialPort;
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
            serialPort1 = new ReliableSerialPort("COM5", 115200, Parity.None, 8, StopBits.One);
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
                DecodeMessage(byteReceived);
            }
            
        
        }

        private void boutonClear_Click(object sender, RoutedEventArgs e)
        {
            textBoxReception.Text = "";
            IRGauche.Text = "";
            IRDroit.Text = "";
            IRCentre.Text = "";
            textBox4.Text = "";
            textBox5.Text = "";
        }

        private void boutonTest_Click(object sender, RoutedEventArgs e)
        {
            //byte [] byteList = new byte [20];
            //for (int i = 0; i < 20; i++)
            //{
            //    byteList[i] = (byte)(2 * i);
            //}
            //serialPort1.Write(byteList, 0, byteList.Length);

            int msgFunction = 0x0080, msgPayloadLength;
            byte[] msgPayload = Encoding.ASCII.GetBytes(textBoxEmission.Text);
            msgPayloadLength = msgPayload.Length;
            UartEncodeAndSendMessage (msgFunction, msgPayloadLength, msgPayload);
            textBoxEmission.Text = "";
        }

        public enum StateReception
        {
            Waiting,
            FunctionMSB,
            FunctionLSB,
            PayloadLengthMSB,
            PayloadLengthLSB,
            Payload,
            CheckSum
        }

        StateReception rcvState = StateReception.Waiting;
        int msgDecodedFunction = 0;
        int msgDecodedPayloadLength = 0;
        byte[] msgDecodedPayload;
        int msgDecodedPayloadIndex = 0;

        private void DecodeMessage(byte c)
        {
            switch (rcvState)
            {
                case StateReception.Waiting:
                    if (c == 0xFE)
                    {
                        rcvState = StateReception.FunctionMSB;
                        msgDecodedFunction = 0;
                        msgDecodedPayloadLength = 0;
                        //msgDecodedPayload;
                        msgDecodedPayloadIndex = 0;
                    }
                    break;

                case StateReception.FunctionMSB:
                    msgDecodedFunction = c;
                    msgDecodedFunction = msgDecodedFunction << 8;
                    rcvState = StateReception.FunctionLSB;
                    break;

                case StateReception.FunctionLSB:
                    msgDecodedFunction += c;
                    rcvState = StateReception.PayloadLengthMSB;
                    break;

                case StateReception.PayloadLengthMSB:
                    msgDecodedPayloadLength = c;
                    msgDecodedPayloadLength = (msgDecodedPayloadLength << 8);
                    rcvState = StateReception.PayloadLengthLSB;
                    break;

                case StateReception.PayloadLengthLSB:
                    msgDecodedPayloadLength += c;
                    if (msgDecodedPayloadLength == 0) {
                        rcvState = StateReception.Waiting;
                    }
                    else {
                        rcvState = StateReception.Payload;
                        
                    }
                    msgDecodedPayload = new byte[msgDecodedPayloadLength];
                    break;

                case StateReception.Payload:
                    msgDecodedPayload [msgDecodedPayloadIndex] = (byte) c;
                    msgDecodedPayloadIndex++;

                    if (msgDecodedPayloadIndex == msgDecodedPayloadLength)
                    {
                        rcvState = StateReception.CheckSum;
                    }
                    break;

                case StateReception.CheckSum:
                    byte calculatedChecksum = CalculateChecksum(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
                    byte receivedChecksum = c;
                    if (calculatedChecksum == receivedChecksum)
                    {
                        //textBoxReception.Text += "ouiiii!!!" + "\n";
                        ProcessDecodeMessage(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
                        rcvState = StateReception.Waiting;
                    }
                    else
                    {
                        textBoxReception.Text += "nooooon!!!!" + "\n";
                        
                    }
                    
                    break;

                default:
                    rcvState = StateReception.Waiting;
                    break;
            }
        }

        private enum FonctionId
        {
            text = 0x0080,
            led = 0x0020,
            dist = 0x0030,
            vit = 0x0040,
            etat = 0x0050
        }

        private enum StateRobot
        {
            STATE_ATTENTE = 0,
            STATE_ATTENTE_EN_COURS = 1,
            STATE_AVANCE = 2,
            STATE_AVANCE_EN_COURS = 3,
            STATE_TOURNE_GAUCHE = 4,
            STATE_TOURNE_GAUCHE_EN_COURS = 5,
            STATE_TOURNE_DROITE = 6,
            STATE_TOURNE_DROITE_EN_COURS = 7,
            STATE_TOURNE_SUR_PLACE_GAUCHE = 8,
            STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS = 9,
            STATE_TOURNE_SUR_PLACE_DROITE = 10,
            STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS = 11,
            STATE_ARRET = 12,
            STATE_ARRET_EN_COURS = 13,
            STATE_RECULE = 14,
            STATE_RECULE_EN_COURS = 15,
            STATE_RALENTI = 16,
            STATE_RALENTI_EN_COURS = 17,
            STATE_TOURNE_UN_PEU_GAUCHE = 18,
            STATE_TOURNE_UN_PEU_GAUCHE_EN_COURS = 19,
            STATE_TOURNE_UN_PEU_DROITE = 20,
            STATE_TOURNE_UN_PEU_DROITE_EN_COURS = 21,
        }

        void ProcessDecodeMessage (int msgFunction, int msgPayloadLenght, byte [] msgPayload)
        {
            //textBoxReception.Text += "0x" + msgFunction.ToString("X4") + "\n";
            //textBoxReception.Text += msgPayloadLenght + "\n";
            if (msgFunction == (int)FonctionId.dist)
            {
                IRGauche.Text = "";
                IRGauche.Text += msgPayload[0] + " cm";
                IRCentre.Text = "";
                IRCentre.Text += msgPayload[1] + " cm";
                IRDroit.Text = "";
                IRDroit.Text += msgPayload[2] + " cm";

            }
            if(msgFunction == (int)FonctionId.text)
            {
                for (int i = 0; i < msgPayloadLenght; i++)
                {
                    textBoxReception.Text += Convert.ToChar(msgPayload[i]);
                }
                textBoxReception.Text += "\n";
            }

            if (msgFunction == (int)FonctionId.etat)
            {
                //textBoxReception.Text += msgPayload[0] + "\n";

                RBTrecept.Text = "";
                int instant = (( (int)msgPayload[1]) << 24) + (( (int)msgPayload[2]) << 16) + (( (int)msgPayload[3]) << 8) + ( (int)msgPayload[4]);
                RBTrecept.Text += " \nRobot State : " + ((StateRobot)(msgPayload[0])).ToString() + "−" + instant.ToString() + " ms";


//                for (int i = 1; i < msgPayloadLenght; i++)
//                {
//                    textBoxReception.Text += msgPayload[i];
//                }
                
            }
        }
    }
}
