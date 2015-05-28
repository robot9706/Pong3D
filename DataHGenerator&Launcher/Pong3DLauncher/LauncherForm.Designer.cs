namespace Pong3DLauncher
{
    partial class LauncherForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(LauncherForm));
            this.label1 = new System.Windows.Forms.Label();
            this.gameWidth = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.gameHeight = new System.Windows.Forms.NumericUpDown();
            this.soundCheckBox = new System.Windows.Forms.CheckBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.startBtn = new System.Windows.Forms.Button();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.fullscreen = new System.Windows.Forms.CheckBox();
            ((System.ComponentModel.ISupportInitialize)(this.gameWidth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.gameHeight)).BeginInit();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(9, 97);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(71, 16);
            this.label1.TabIndex = 1;
            this.label1.Text = "Felbontás:";
            // 
            // gameWidth
            // 
            this.gameWidth.Location = new System.Drawing.Point(3, 3);
            this.gameWidth.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.gameWidth.Minimum = new decimal(new int[] {
            320,
            0,
            0,
            0});
            this.gameWidth.Name = "gameWidth";
            this.gameWidth.Size = new System.Drawing.Size(120, 22);
            this.gameWidth.TabIndex = 2;
            this.gameWidth.Value = new decimal(new int[] {
            1280,
            0,
            0,
            0});
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(129, 6);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(14, 16);
            this.label2.TabIndex = 3;
            this.label2.Text = "x";
            // 
            // gameHeight
            // 
            this.gameHeight.Location = new System.Drawing.Point(149, 3);
            this.gameHeight.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.gameHeight.Minimum = new decimal(new int[] {
            240,
            0,
            0,
            0});
            this.gameHeight.Name = "gameHeight";
            this.gameHeight.Size = new System.Drawing.Size(120, 22);
            this.gameHeight.TabIndex = 4;
            this.gameHeight.Value = new decimal(new int[] {
            720,
            0,
            0,
            0});
            // 
            // soundCheckBox
            // 
            this.soundCheckBox.AutoSize = true;
            this.soundCheckBox.Checked = true;
            this.soundCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.soundCheckBox.Location = new System.Drawing.Point(113, 159);
            this.soundCheckBox.Name = "soundCheckBox";
            this.soundCheckBox.Size = new System.Drawing.Size(67, 20);
            this.soundCheckBox.TabIndex = 5;
            this.soundCheckBox.Text = "Hang?";
            this.soundCheckBox.UseVisualStyleBackColor = true;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.gameWidth);
            this.panel1.Controls.Add(this.label2);
            this.panel1.Controls.Add(this.gameHeight);
            this.panel1.Location = new System.Drawing.Point(10, 116);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(273, 30);
            this.panel1.TabIndex = 6;
            // 
            // startBtn
            // 
            this.startBtn.Location = new System.Drawing.Point(79, 219);
            this.startBtn.Name = "startBtn";
            this.startBtn.Size = new System.Drawing.Size(134, 37);
            this.startBtn.TabIndex = 7;
            this.startBtn.Text = "Start";
            this.startBtn.UseVisualStyleBackColor = true;
            this.startBtn.Click += new System.EventHandler(this.startBtn_Click);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = global::Pong3DLauncher.Properties.Resources.PongLogo;
            this.pictureBox1.Location = new System.Drawing.Point(41, 18);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(210, 48);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // fullscreen
            // 
            this.fullscreen.AutoSize = true;
            this.fullscreen.Location = new System.Drawing.Point(80, 185);
            this.fullscreen.Name = "fullscreen";
            this.fullscreen.Size = new System.Drawing.Size(132, 20);
            this.fullscreen.TabIndex = 8;
            this.fullscreen.Text = "Teljes képernyő?";
            this.fullscreen.UseVisualStyleBackColor = true;
            // 
            // LauncherForm
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.ClientSize = new System.Drawing.Size(292, 262);
            this.Controls.Add(this.fullscreen);
            this.Controls.Add(this.startBtn);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.soundCheckBox);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.pictureBox1);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "LauncherForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Pong3D";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.gameWidth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.gameHeight)).EndInit();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown gameWidth;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.NumericUpDown gameHeight;
        private System.Windows.Forms.CheckBox soundCheckBox;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button startBtn;
        private System.Windows.Forms.CheckBox fullscreen;
    }
}

