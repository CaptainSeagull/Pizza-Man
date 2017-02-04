using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace LevelEditor
{
    public partial class NewBuildingPopup : Form
    {
        [DllImport("GetModelData.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void GetModelData(string file, ref float sizeX, ref float sizeY, ref float sizeZ, ref float positionX, ref float positionY, ref float positionZ);

        BuildingData m_buildingData;

        public NewBuildingPopup()
        {
            InitializeComponent();
            m_buildingData = new BuildingData();
        }

        private void m_chooseFileButton_Click(object sender, EventArgs e)
        {
            openFileDialog1.ShowDialog(this);

            if(openFileDialog1.FileName != "")
            {
                string[] directories = openFileDialog1.FileName.Split('\\');
                Stack<string> necessaryDirectors = new Stack<string>();
                for(int i=directories.Length-1; i>=0; i--)
                {
                    necessaryDirectors.Push(directories[i]);

                    if (directories[i].Equals("Models"))
                        break;
                }
                string relativePath = "";
                bool first = true;
                while(necessaryDirectors.Count > 0)
                {
                    if (!first)
                        relativePath += "\\";
                    else
                        first = false;

                    relativePath += necessaryDirectors.Pop();
                }
                float sizeX = 0f, sizeY = 0f, sizeZ = 0f, positionX = 0f, positionY = 0f, positionZ = 0f;
                GetModelData(openFileDialog1.FileName, ref sizeX, ref sizeY, ref sizeZ, ref positionX, ref positionY, ref positionZ);

                m_buildingData = new BuildingData(relativePath, new Vector3(sizeX, sizeY, sizeZ), new Vector3(positionX, positionY, positionZ));
                m_sizeXController.Value = (decimal)sizeX;
                m_sizeYController.Value = (decimal)sizeY;
                m_sizeZController.Value = (decimal)sizeZ;

                m_nameTextBox.Text = relativePath;
            }
        }

        private void m_finishButton_Click(object sender, EventArgs e)
        {
            if(m_buildingData.Path == "")
            {
                MessageBox.Show(this, "Path not valid.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            } 
            if (m_sizeYController.Value == 0)
            {
                MessageBox.Show(this, "Height not valid.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            } 
            if (m_sizeXController.Value == 0)
            {
                MessageBox.Show(this, "Width not valid.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            } 
            if (m_nameTextBox.Text == "")
            {
                MessageBox.Show(this, "Name not valid.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            m_buildingData.Size.X = (float)m_sizeXController.Value;
            m_buildingData.Size.Y = (float)m_sizeYController.Value;
            m_buildingData.Size.Z = (float)m_sizeZController.Value;

            if (!((EditorWindow)Owner).AddBuilding(m_nameTextBox.Text, m_buildingData))
                MessageBox.Show(this, "Name already taken.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            else
                Close();
        }
    }
}
