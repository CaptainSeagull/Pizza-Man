using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml;

namespace LevelEditor
{
    public partial class Form1 : Form
    {
        List<BuildingControl> m_buildingControls;

        public Form1()
        {
            InitializeComponent();
            m_buildingControls = new List<BuildingControl>();
        }

        private void m_createButton_Click(object sender, EventArgs e)
        {
            BuildingControl newBuilding = new BuildingControl();
            newBuilding.Location = new Point(0, 0);
            newBuilding.AutoSize = true;
            newBuilding.Parent = m_gamePanel;
            m_gamePanel.Controls.Add(newBuilding);
            m_buildingControls.Add(newBuilding);
        }

        private void m_deleteButton_Click(object sender, EventArgs e)
        {
            Queue<BuildingControl> toBeRemoved = new Queue<BuildingControl>();

            foreach(BuildingControl building in m_buildingControls)
            {
                if (building.Selected)
                    toBeRemoved.Enqueue(building);
            }

            foreach(BuildingControl removal in toBeRemoved)
            {
                m_buildingControls.Remove(removal);
                m_gamePanel.Controls.Remove(removal);
            }
        }

        private void m_saveButton_Click(object sender, EventArgs e)
        {
            m_saveFileDialog.ShowDialog(this);

            if(m_saveFileDialog.FileName != "")
            {
                XmlWriter xmlWriter = XmlWriter.Create(m_saveFileDialog.FileName);

                xmlWriter.WriteStartDocument();
                xmlWriter.WriteStartElement("Buildings");

                foreach(BuildingControl building in m_buildingControls)
                {
                    xmlWriter.WriteStartElement("Building");
                    xmlWriter.WriteStartElement("Position");
                    xmlWriter.WriteAttributeString("x", building.Location.X.ToString());
                    xmlWriter.WriteAttributeString("y", "0");
                    xmlWriter.WriteAttributeString("z", building.Location.Y.ToString());
                    xmlWriter.WriteEndElement();
                    xmlWriter.WriteEndElement();
                }

                xmlWriter.WriteEndElement();
                xmlWriter.Close();
            }
        }

        private void m_loadButton_Click(object sender, EventArgs e)
        {
            m_loadFileDialog.ShowDialog(this);

            if (m_loadFileDialog.FileName != "")
            {
                XmlReader xmlReader = XmlReader.Create(m_loadFileDialog.FileName);

                while (xmlReader.Read())
                {
                    if ((xmlReader.NodeType == XmlNodeType.Element) && (xmlReader.Name == "Position"))
                    {
                        if (xmlReader.HasAttributes)
                        {
                            BuildingControl newBuilding = new BuildingControl();
                            int x = int.Parse(xmlReader.GetAttribute("x"));
                            int y = int.Parse(xmlReader.GetAttribute("z"));
                            newBuilding.Location = new Point(x, y);
                            newBuilding.AutoSize = true;
                            newBuilding.Parent = m_gamePanel;
                            m_gamePanel.Controls.Add(newBuilding);
                            m_buildingControls.Add(newBuilding);
                        }
                    }
                }
                xmlReader.Close();
            }
        }
    }
}
