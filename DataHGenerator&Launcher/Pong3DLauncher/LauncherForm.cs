using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;

namespace Pong3DLauncher
{
    public partial class LauncherForm : Form
    {
        [DllImport("winmm.dll", SetLastError = true)]
        public static extern uint waveOutGetNumDevs();

        public LauncherForm()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            try
            {
                uint soundCards = waveOutGetNumDevs();
                if (soundCards == 0)
                {
                    soundCheckBox.Enabled = false;
                    soundCheckBox.Checked = false;
                }
            }
            catch
            {
                //Ká
                soundCheckBox.Enabled = false;
                soundCheckBox.Checked = false;
            }
        }

        private void startBtn_Click(object sender, EventArgs e)
        {
            string folder = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string game = Path.Combine(folder, "Pong3D.exe");
            if (!File.Exists(game))
            {
                MessageBox.Show("A játék nem található!\n(" + game + ")");
                return;
            }
            else
            {
                string commands = string.Empty;
                if (!soundCheckBox.Checked)
                    commands += "nosound";
                if (fullscreen.Checked)
                    commands += " fullscreen";
                commands += " w:" + ((int)gameWidth.Value).ToString() + " h:" + ((int)gameHeight.Value).ToString();

                ProcessStartInfo info = new ProcessStartInfo();
                info.Arguments = commands;
                info.FileName = game;

                Process.Start(info);

                Close();
            }
        }
    }
}
