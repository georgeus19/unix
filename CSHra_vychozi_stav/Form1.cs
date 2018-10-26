using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CSHra
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        Mapa mapa;
        Graphics g;
        private void button1_Click(object sender, EventArgs e)
        {
            g = CreateGraphics();
            mapa = new Mapa("plan.txt", "ikonky.png");
            this.Text = "Zbývá sebrat " + mapa.ZbyvaDiamantu + " diamantů";

            timer1.Enabled = true;
            button1.Visible = false;
        }

        private void timer1_Tick(object sender, EventArgs e)
        {

            switch (mapa.stav)
            {
                case Stav.bezi:
                    mapa.PohniVsemiPrvky(stisknutaSipka);
                    mapa.VykresliSe(g, ClientSize.Width, ClientSize.Height);
                    this.Text = "Zbývá sebrat " + mapa.ZbyvaDiamantu + " diamantů";
                    break;
                case Stav.vyhra:
                    timer1.Enabled = false;
                    MessageBox.Show("Vyhra!");
                    break;
                case Stav.prohra:
                    timer1.Enabled = false;
                    MessageBox.Show("Prohra!");
                    break;
                default:
                    break;
            }
        }

        StisknutaSipka stisknutaSipka = StisknutaSipka.zadna;

        // HACK na odchyceni stisku sipek
        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            if (keyData == Keys.Up)
            {
                stisknutaSipka = StisknutaSipka.nahoru;
                return true;
            }
            if (keyData == Keys.Down)
            {
                stisknutaSipka = StisknutaSipka.dolu;
                return true;
            }
            if (keyData == Keys.Left)
            {
                stisknutaSipka = StisknutaSipka.doleva;
                return true;
            }
            if (keyData == Keys.Right)
            {
                stisknutaSipka = StisknutaSipka.doprava;
                return true;
            }
            return base.ProcessCmdKey(ref msg, keyData);
        }

        private void Form1_KeyUp(object sender, KeyEventArgs e)
        {
            stisknutaSipka = StisknutaSipka.zadna;
        }

        private void Form1_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e)
        {
        }
    }
}
