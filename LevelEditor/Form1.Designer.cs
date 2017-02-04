namespace LevelEditor
{
    partial class Form1
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
            this.m_gamePanel = new System.Windows.Forms.Panel();
            this.m_createButton = new System.Windows.Forms.Button();
            this.m_deleteButton = new System.Windows.Forms.Button();
            this.m_saveButton = new System.Windows.Forms.Button();
            this.m_saveFileDialog = new System.Windows.Forms.SaveFileDialog();
            this.m_loadButton = new System.Windows.Forms.Button();
            this.m_loadFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.SuspendLayout();
            // 
            // m_gamePanel
            // 
            this.m_gamePanel.BackColor = System.Drawing.SystemColors.AppWorkspace;
            this.m_gamePanel.Location = new System.Drawing.Point(0, -1);
            this.m_gamePanel.Name = "m_gamePanel";
            this.m_gamePanel.Size = new System.Drawing.Size(1027, 684);
            this.m_gamePanel.TabIndex = 0;
            // 
            // m_createButton
            // 
            this.m_createButton.Location = new System.Drawing.Point(1111, 42);
            this.m_createButton.Name = "m_createButton";
            this.m_createButton.Size = new System.Drawing.Size(90, 23);
            this.m_createButton.TabIndex = 1;
            this.m_createButton.Text = "Add Building";
            this.m_createButton.UseVisualStyleBackColor = true;
            this.m_createButton.Click += new System.EventHandler(this.m_createButton_Click);
            // 
            // m_deleteButton
            // 
            this.m_deleteButton.Location = new System.Drawing.Point(1111, 71);
            this.m_deleteButton.Name = "m_deleteButton";
            this.m_deleteButton.Size = new System.Drawing.Size(90, 23);
            this.m_deleteButton.TabIndex = 2;
            this.m_deleteButton.Text = "Delete Building";
            this.m_deleteButton.UseVisualStyleBackColor = true;
            this.m_deleteButton.Click += new System.EventHandler(this.m_deleteButton_Click);
            // 
            // m_saveButton
            // 
            this.m_saveButton.Location = new System.Drawing.Point(1111, 628);
            this.m_saveButton.Name = "m_saveButton";
            this.m_saveButton.Size = new System.Drawing.Size(75, 23);
            this.m_saveButton.TabIndex = 3;
            this.m_saveButton.Text = "Save";
            this.m_saveButton.UseVisualStyleBackColor = true;
            this.m_saveButton.Click += new System.EventHandler(this.m_saveButton_Click);
            // 
            // m_saveFileDialog
            // 
            this.m_saveFileDialog.DefaultExt = "pza";
            this.m_saveFileDialog.Filter = "Level file | *.pza";
            this.m_saveFileDialog.Title = "Save the Level";
            // 
            // m_loadButton
            // 
            this.m_loadButton.Location = new System.Drawing.Point(1111, 599);
            this.m_loadButton.Name = "m_loadButton";
            this.m_loadButton.Size = new System.Drawing.Size(75, 23);
            this.m_loadButton.TabIndex = 4;
            this.m_loadButton.Text = "Load";
            this.m_loadButton.UseVisualStyleBackColor = true;
            this.m_loadButton.Click += new System.EventHandler(this.m_loadButton_Click);
            // 
            // m_loadFileDialog
            // 
            this.m_loadFileDialog.DefaultExt = "pza";
            this.m_loadFileDialog.FileName = "openFileDialog1";
            this.m_loadFileDialog.Filter = "Level file | *.pza";
            this.m_loadFileDialog.Title = "Load Level";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1264, 682);
            this.Controls.Add(this.m_loadButton);
            this.Controls.Add(this.m_saveButton);
            this.Controls.Add(this.m_deleteButton);
            this.Controls.Add(this.m_createButton);
            this.Controls.Add(this.m_gamePanel);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel m_gamePanel;
        private System.Windows.Forms.Button m_createButton;
        private System.Windows.Forms.Button m_deleteButton;
        private System.Windows.Forms.Button m_saveButton;
        private System.Windows.Forms.SaveFileDialog m_saveFileDialog;
        private System.Windows.Forms.Button m_loadButton;
        private System.Windows.Forms.OpenFileDialog m_loadFileDialog;
    }
}

