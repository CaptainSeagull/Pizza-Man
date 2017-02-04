namespace LevelEditor
{
    partial class NewBuildingPopup
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
            this.m_chooseFileButton = new System.Windows.Forms.Button();
            this.m_sizeLabel = new System.Windows.Forms.Label();
            this.m_sizeXController = new System.Windows.Forms.NumericUpDown();
            this.m_sizeYController = new System.Windows.Forms.NumericUpDown();
            this.m_nameTextBox = new System.Windows.Forms.TextBox();
            this.m_finishButton = new System.Windows.Forms.Button();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.m_sizeZController = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.m_sizeXController)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.m_sizeYController)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.m_sizeZController)).BeginInit();
            this.SuspendLayout();
            // 
            // m_chooseFileButton
            // 
            this.m_chooseFileButton.Location = new System.Drawing.Point(98, 12);
            this.m_chooseFileButton.Name = "m_chooseFileButton";
            this.m_chooseFileButton.Size = new System.Drawing.Size(75, 23);
            this.m_chooseFileButton.TabIndex = 0;
            this.m_chooseFileButton.Text = "Choose File";
            this.m_chooseFileButton.UseVisualStyleBackColor = true;
            this.m_chooseFileButton.Click += new System.EventHandler(this.m_chooseFileButton_Click);
            // 
            // m_sizeLabel
            // 
            this.m_sizeLabel.AutoSize = true;
            this.m_sizeLabel.Location = new System.Drawing.Point(13, 61);
            this.m_sizeLabel.Name = "m_sizeLabel";
            this.m_sizeLabel.Size = new System.Drawing.Size(34, 13);
            this.m_sizeLabel.TabIndex = 1;
            this.m_sizeLabel.Text = "SizeX";
            // 
            // m_sizeXController
            // 
            this.m_sizeXController.DecimalPlaces = 3;
            this.m_sizeXController.Location = new System.Drawing.Point(59, 61);
            this.m_sizeXController.Maximum = new decimal(new int[] {
            1000000,
            0,
            0,
            0});
            this.m_sizeXController.Name = "m_sizeXController";
            this.m_sizeXController.Size = new System.Drawing.Size(83, 20);
            this.m_sizeXController.TabIndex = 2;
            // 
            // m_sizeYController
            // 
            this.m_sizeYController.DecimalPlaces = 3;
            this.m_sizeYController.Location = new System.Drawing.Point(59, 87);
            this.m_sizeYController.Maximum = new decimal(new int[] {
            1000000,
            0,
            0,
            0});
            this.m_sizeYController.Name = "m_sizeYController";
            this.m_sizeYController.Size = new System.Drawing.Size(83, 20);
            this.m_sizeYController.TabIndex = 3;
            // 
            // m_nameTextBox
            // 
            this.m_nameTextBox.Location = new System.Drawing.Point(86, 153);
            this.m_nameTextBox.Name = "m_nameTextBox";
            this.m_nameTextBox.Size = new System.Drawing.Size(100, 20);
            this.m_nameTextBox.TabIndex = 4;
            this.m_nameTextBox.Text = "Name";
            // 
            // m_finishButton
            // 
            this.m_finishButton.Location = new System.Drawing.Point(98, 211);
            this.m_finishButton.Name = "m_finishButton";
            this.m_finishButton.Size = new System.Drawing.Size(75, 23);
            this.m_finishButton.TabIndex = 5;
            this.m_finishButton.Text = "Finish";
            this.m_finishButton.UseVisualStyleBackColor = true;
            this.m_finishButton.Click += new System.EventHandler(this.m_finishButton_Click);
            // 
            // m_sizeZController
            // 
            this.m_sizeZController.DecimalPlaces = 3;
            this.m_sizeZController.Location = new System.Drawing.Point(59, 114);
            this.m_sizeZController.Maximum = new decimal(new int[] {
            1000000,
            0,
            0,
            0});
            this.m_sizeZController.Name = "m_sizeZController";
            this.m_sizeZController.Size = new System.Drawing.Size(83, 20);
            this.m_sizeZController.TabIndex = 6;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 89);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(34, 13);
            this.label1.TabIndex = 7;
            this.label1.Text = "SizeY";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(13, 116);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(34, 13);
            this.label2.TabIndex = 8;
            this.label2.Text = "SizeZ";
            // 
            // NewBuildingPopup
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 262);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.m_sizeZController);
            this.Controls.Add(this.m_finishButton);
            this.Controls.Add(this.m_nameTextBox);
            this.Controls.Add(this.m_sizeYController);
            this.Controls.Add(this.m_sizeXController);
            this.Controls.Add(this.m_sizeLabel);
            this.Controls.Add(this.m_chooseFileButton);
            this.Name = "NewBuildingPopup";
            this.Text = "NewBuildingPopup";
            ((System.ComponentModel.ISupportInitialize)(this.m_sizeXController)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.m_sizeYController)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.m_sizeZController)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button m_chooseFileButton;
        private System.Windows.Forms.Label m_sizeLabel;
        private System.Windows.Forms.NumericUpDown m_sizeXController;
        private System.Windows.Forms.NumericUpDown m_sizeYController;
        private System.Windows.Forms.TextBox m_nameTextBox;
        private System.Windows.Forms.Button m_finishButton;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.NumericUpDown m_sizeZController;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
    }
}