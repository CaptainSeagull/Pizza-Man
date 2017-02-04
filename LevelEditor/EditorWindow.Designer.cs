namespace LevelEditor
{
    partial class EditorWindow
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
            this.m_buildingsList = new System.Windows.Forms.ListBox();
            this.m_buildingControlPanel = new System.Windows.Forms.Panel();
            this.m_endPointCheckBox = new System.Windows.Forms.CheckBox();
            this.m_startPointCheckBox = new System.Windows.Forms.CheckBox();
            this.label1 = new System.Windows.Forms.Label();
            this.m_buildingYPosController = new System.Windows.Forms.NumericUpDown();
            this.m_buildingControlPanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.m_buildingYPosController)).BeginInit();
            this.SuspendLayout();
            // 
            // m_gamePanel
            // 
            this.m_gamePanel.AllowDrop = true;
            this.m_gamePanel.BackColor = System.Drawing.SystemColors.AppWorkspace;
            this.m_gamePanel.Location = new System.Drawing.Point(1, 0);
            this.m_gamePanel.Name = "m_gamePanel";
            this.m_gamePanel.Size = new System.Drawing.Size(1066, 681);
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
            this.m_deleteButton.Location = new System.Drawing.Point(1111, 195);
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
            this.m_loadFileDialog.Filter = "Level file | *.pza";
            this.m_loadFileDialog.Title = "Load Level";
            // 
            // m_buildingsList
            // 
            this.m_buildingsList.FormattingEnabled = true;
            this.m_buildingsList.Location = new System.Drawing.Point(1111, 83);
            this.m_buildingsList.Name = "m_buildingsList";
            this.m_buildingsList.Size = new System.Drawing.Size(120, 95);
            this.m_buildingsList.TabIndex = 5;
            // 
            // m_buildingControlPanel
            // 
            this.m_buildingControlPanel.Controls.Add(this.m_endPointCheckBox);
            this.m_buildingControlPanel.Controls.Add(this.m_startPointCheckBox);
            this.m_buildingControlPanel.Controls.Add(this.label1);
            this.m_buildingControlPanel.Controls.Add(this.m_buildingYPosController);
            this.m_buildingControlPanel.Location = new System.Drawing.Point(1111, 248);
            this.m_buildingControlPanel.Name = "m_buildingControlPanel";
            this.m_buildingControlPanel.Size = new System.Drawing.Size(120, 117);
            this.m_buildingControlPanel.TabIndex = 6;
            // 
            // m_endPointCheckBox
            // 
            this.m_endPointCheckBox.AutoSize = true;
            this.m_endPointCheckBox.Location = new System.Drawing.Point(0, 97);
            this.m_endPointCheckBox.Name = "m_endPointCheckBox";
            this.m_endPointCheckBox.Size = new System.Drawing.Size(72, 17);
            this.m_endPointCheckBox.TabIndex = 3;
            this.m_endPointCheckBox.Text = "End Point";
            this.m_endPointCheckBox.UseVisualStyleBackColor = true;
            this.m_endPointCheckBox.CheckedChanged += new System.EventHandler(this.m_endPointCheckBox_CheckedChanged);
            // 
            // m_startPointCheckBox
            // 
            this.m_startPointCheckBox.AutoSize = true;
            this.m_startPointCheckBox.Location = new System.Drawing.Point(1, 74);
            this.m_startPointCheckBox.Name = "m_startPointCheckBox";
            this.m_startPointCheckBox.Size = new System.Drawing.Size(75, 17);
            this.m_startPointCheckBox.TabIndex = 2;
            this.m_startPointCheckBox.Text = "Start Point";
            this.m_startPointCheckBox.UseVisualStyleBackColor = true;
            this.m_startPointCheckBox.CheckedChanged += new System.EventHandler(this.m_startPointCheckBox_CheckedChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(54, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Y Position";
            // 
            // m_buildingYPosController
            // 
            this.m_buildingYPosController.Location = new System.Drawing.Point(1, 25);
            this.m_buildingYPosController.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.m_buildingYPosController.Name = "m_buildingYPosController";
            this.m_buildingYPosController.Size = new System.Drawing.Size(116, 20);
            this.m_buildingYPosController.TabIndex = 0;
            this.m_buildingYPosController.ValueChanged += new System.EventHandler(this.m_buildingYPosController_ValueChanged);
            // 
            // EditorWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1264, 682);
            this.Controls.Add(this.m_buildingControlPanel);
            this.Controls.Add(this.m_buildingsList);
            this.Controls.Add(this.m_loadButton);
            this.Controls.Add(this.m_saveButton);
            this.Controls.Add(this.m_deleteButton);
            this.Controls.Add(this.m_createButton);
            this.Controls.Add(this.m_gamePanel);
            this.Name = "EditorWindow";
            this.Text = "Form1";
            this.m_buildingControlPanel.ResumeLayout(false);
            this.m_buildingControlPanel.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.m_buildingYPosController)).EndInit();
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
        private System.Windows.Forms.ListBox m_buildingsList;
        private System.Windows.Forms.Panel m_buildingControlPanel;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown m_buildingYPosController;
        private System.Windows.Forms.CheckBox m_startPointCheckBox;
        private System.Windows.Forms.CheckBox m_endPointCheckBox;
    }
}

