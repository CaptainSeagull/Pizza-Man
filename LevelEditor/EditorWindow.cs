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
using System.Xml.Linq; 

namespace LevelEditor
{
    public struct BuildingData
    {
        public string Path;
        public Vector3 Size;
        public Vector3 Position;

        public BuildingData(string path, Vector3 size, Vector3 position)
        {
            Path = path;
            Size = size;
            Position = position;
        }
    }
    public partial class EditorWindow : Form
    {

        private List<BuildingControl> m_buildingControls;
        private Dictionary<string, BuildingData> m_buildingData;
        private BuildingControl m_startBuilding = null, m_endBuilding = null;

        private Vector3 m_worldCoordMin, m_worldCoordMax;
        public Vector3 WorldCoordMin { get { return m_worldCoordMin; } private set { m_worldCoordMin = value; UpdatePositions(); } }
        public Vector3 WorldCoordMax { get { return m_worldCoordMax; } private set { m_worldCoordMax = value; UpdatePositions(); } }

        private const float ZOOM_FACTOR = 50f;

        public EditorWindow()
        {
            InitializeComponent();
            m_gamePanel.DragDrop += m_gamePanel_DragDrop;
            m_gamePanel.DragOver += m_gamePanel_DragOver;
            m_gamePanel.MouseClick += m_gamePanel_MouseClick;
            m_buildingsList.MouseDown += m_buildingsList_MouseDown;

            m_buildingControls = new List<BuildingControl>();
            m_buildingData = new Dictionary<string, BuildingData>();
            WorldCoordMax = new Vector3(10000f, 0f, 10000f);
            WorldCoordMin = -WorldCoordMax;
            
        }

        private void m_gamePanel_MouseClick(object sender, MouseEventArgs e)
        {
            UnselectAll();
        }

        public void UnselectAll()
        {
            foreach (BuildingControl control in m_buildingControls)
                control.Selected = false;
        }

        public bool AddBuilding(string name, BuildingData building)
        {
            if(!m_buildingData.ContainsKey(name))
            {
                m_buildingData.Add(name, building);
                m_buildingsList.Items.Add(name);
                return true;
            }
            return false;
        }

        private void UpdatePositions()
        {
            foreach (BuildingControl control in m_buildingControls)
            {
                //Not as strange as it looks...
                control.WorldPosition = control.WorldPosition;
                control.WorldSize = control.WorldSize;
            }
        }

        private void m_createButton_Click(object sender, EventArgs e)
        {
           NewBuildingPopup popupWin = new NewBuildingPopup();
           popupWin.Show(this);
        }

        private void AddBuildingToScene(string name, Point editorCoords)
        {
            if(m_buildingData.ContainsKey(name))
            {
                BuildingData newBuildingsData = m_buildingData[name];
                BuildingControl newBuilding = new BuildingControl(this);
                newBuilding.Parent = m_gamePanel;
                newBuilding.ObjectName = name;
                newBuilding.WorldSize = newBuildingsData.Size;
                newBuilding.Location = editorCoords;
                m_gamePanel.Controls.Add(newBuilding);
                m_buildingControls.Add(newBuilding);
            }
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
                XmlWriterSettings settings = new XmlWriterSettings();
                settings.Indent = true;
                settings.NewLineOnAttributes = true;
                XmlWriter xmlWriter = XmlWriter.Create(m_saveFileDialog.FileName, settings);
                

                xmlWriter.WriteStartDocument();
                xmlWriter.WriteStartElement("Level");
                {
                    xmlWriter.WriteStartElement("Models");
                    {
                        xmlWriter.WriteAttributeString("Count", m_buildingData.Count.ToString());
                        foreach (var pair in m_buildingData)
                        {
                            xmlWriter.WriteStartElement("BuildingModel");
                            {
                                xmlWriter.WriteAttributeString("Name", pair.Key);
                                xmlWriter.WriteAttributeString("Path", pair.Value.Path);
                                xmlWriter.WriteStartElement("Position");
                                {
                                    xmlWriter.WriteAttributeString("x", pair.Value.Position.X.ToString());
                                    xmlWriter.WriteAttributeString("y", pair.Value.Position.Y.ToString());
                                    xmlWriter.WriteAttributeString("z", pair.Value.Position.Z.ToString());
                                }
                                xmlWriter.WriteEndElement();
                                xmlWriter.WriteStartElement("Scale");
                                {
                                    xmlWriter.WriteAttributeString("x", pair.Value.Size.X.ToString());
                                    xmlWriter.WriteAttributeString("y", pair.Value.Size.Y.ToString());
                                    xmlWriter.WriteAttributeString("z", pair.Value.Size.Z.ToString());
                                }
                                xmlWriter.WriteEndElement();
                            }
                            xmlWriter.WriteEndElement();
                        }
                    }
                    xmlWriter.WriteEndElement();

                    xmlWriter.WriteStartElement("Objects");
                    {
                        xmlWriter.WriteAttributeString("Count", m_buildingControls.Count.ToString());
                        foreach (BuildingControl building in m_buildingControls)
                        {
                            xmlWriter.WriteStartElement("Building");
                            {
                                xmlWriter.WriteAttributeString("Name", building.ObjectName);
                                bool start = (building == m_startBuilding);
                                bool end = (building == m_endBuilding);
                                xmlWriter.WriteAttributeString("StartBuilding", (start) ? "true" : "false");
                                xmlWriter.WriteAttributeString("EndBuilding", (end) ? "true" : "false");
                                xmlWriter.WriteStartElement("Position");
                                {
                                    xmlWriter.WriteAttributeString("x", building.WorldPosition.X.ToString());
                                    xmlWriter.WriteAttributeString("y", building.WorldPosition.Y.ToString());
                                    xmlWriter.WriteAttributeString("z", building.WorldPosition.Z.ToString());
                                }
                                xmlWriter.WriteEndElement();
                                xmlWriter.WriteStartElement("Scale");
                                {
                                    xmlWriter.WriteAttributeString("x", building.WorldSize.X.ToString());
                                    xmlWriter.WriteAttributeString("y", building.WorldSize.Y.ToString());
                                    xmlWriter.WriteAttributeString("z", building.WorldSize.Z.ToString());
                                }
                                xmlWriter.WriteEndElement();
                            }
                            xmlWriter.WriteEndElement();
                        }
                    }
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
                m_startBuilding = null;
                m_buildingControls.Clear();
                m_buildingData.Clear();
                m_gamePanel.Controls.Clear();

                using(XmlReader level = XmlReader.Create(m_loadFileDialog.FileName))
                {
                    level.ReadToFollowing("Models");
                    int count = int.Parse(level.GetAttribute("Count"));
                    for (int i = 0; i < count; i++)
                    {
                        level.ReadToFollowing("BuildingModel");
                        string name = level.GetAttribute("Name");
                        string path = level.GetAttribute("Path");

                        level.ReadToFollowing("Position");
                        float x = float.Parse(level.GetAttribute("x"));
                        float y = float.Parse(level.GetAttribute("y"));
                        float z = float.Parse(level.GetAttribute("z"));
                        Vector3 position = new Vector3(x, y, z);

                        level.ReadToFollowing("Scale");
                        x = float.Parse(level.GetAttribute("x"));
                        y = float.Parse(level.GetAttribute("y"));
                        z = float.Parse(level.GetAttribute("z"));
                        Vector3 size = new Vector3(x, y, z);

                        BuildingData newData = new BuildingData(path, size, position);
                        m_buildingData.Add(name, newData);
                        //level.Read();
                    }

                    level.ReadToFollowing("Objects");
                    count = int.Parse(level.GetAttribute("Count"));
                    for(int i=0; i<count; i++)
                    {
                        level.ReadToFollowing("Building");
                        BuildingControl newBuilding = new BuildingControl(this);
                        newBuilding.ObjectName = level.GetAttribute("Name").ToString();
                        if (bool.Parse(level.GetAttribute("StartBuilding")))
                            m_startBuilding = newBuilding;
                        if (bool.Parse(level.GetAttribute("EndBuilding")))
                            m_endBuilding = newBuilding;

                        level.ReadToFollowing("Position");
                        float x = float.Parse(level.GetAttribute("x"));
                        float y = float.Parse(level.GetAttribute("y"));
                        float z = float.Parse(level.GetAttribute("z"));
                        newBuilding.WorldPosition = new Vector3(x, y, z);

                        level.ReadToFollowing("Scale");
                        x = float.Parse(level.GetAttribute("x"));
                        y = float.Parse(level.GetAttribute("y"));
                        z = float.Parse(level.GetAttribute("z"));
                        newBuilding.WorldSize = new Vector3(x, y, z);

                        newBuilding.Parent = m_gamePanel;
                        m_gamePanel.Controls.Add(newBuilding);
                        m_buildingControls.Add(newBuilding);
                        //level.Read();
                    }
                }
            }
        }

        private void m_buildingsList_MouseDown(object sender, MouseEventArgs e)
        {
            if (m_buildingsList.Items.Count == 0)
                return;

            int index = m_buildingsList.IndexFromPoint(e.X, e.Y);
            string text = "";
            if(index != -1)
                text = m_buildingsList.Items[index].ToString();

            DragDropEffects dragEffects = DoDragDrop(text, DragDropEffects.Link);
        }
        
        private void m_gamePanel_DragOver(object sender, DragEventArgs e)
        {
            e.Effect = DragDropEffects.Link;
        }

        private void m_gamePanel_DragDrop(object sender, DragEventArgs e)
        {
            if(e.Data.GetDataPresent(DataFormats.StringFormat))
            {
                string text = (string)e.Data.GetData(DataFormats.StringFormat);
                Point editorPoint = m_gamePanel.PointToClient(new Point(e.X, e.Y));
                AddBuildingToScene(text, editorPoint);
            }
        }

        public Point WorldToEditorCoords(Vector3 position)
        {
            Vector3 editorWorldSize = WorldCoordMax - WorldCoordMin;

            float x = position.X + editorWorldSize.X / 2f;
            x = x / editorWorldSize.X * m_gamePanel.Width;

            float y = editorWorldSize.Z - position.Z;
            y -= editorWorldSize.Z / 2f;
            y = y / editorWorldSize.Z * m_gamePanel.Height;


            return new Point((int)x, (int)y);

        }

        public Vector3 EditorToWorldCoords(Point position)
        {
            Vector3 editorWorldSize = WorldCoordMax - WorldCoordMin;

            float x = position.X - m_gamePanel.Width / 2f;
            x = x / m_gamePanel.Width * editorWorldSize.X;

            float z = m_gamePanel.Height - position.Y;
            z -= m_gamePanel.Height / 2f;
            z = z / m_gamePanel.Height * editorWorldSize.Z;


            return new Vector3(x, 0f, z);
        }

        public Size WorldToEditorSize(Vector3 size)
        {
            Vector3 editorWorldSize = WorldCoordMax - WorldCoordMin;
            int width = (int)(size.X / editorWorldSize.X * m_gamePanel.Width);
            int height = (int)(width * (size.Z / size.X));

            return new Size(width, height);
        }
        public Vector3 EditorToWorldSize(Size size)
        {
            Vector3 editorWorldSize = WorldCoordMax - WorldCoordMin;
            float width = (float)size.Width / m_gamePanel.Width * editorWorldSize.X;
            float height = (float)(width * ((float)size.Height / size.Width));

            return new Vector3(width, 0f, height);
        }

        public void BuildingSelected(BuildingControl control)
        {
            m_buildingControlPanel.Enabled = true;
            m_startPointCheckBox.Enabled = true;
            m_startPointCheckBox.Checked = (control == m_startBuilding);
            m_endPointCheckBox.Enabled = true;
            m_endPointCheckBox.Checked = (control == m_endBuilding);

            m_buildingYPosController.Value = (decimal)control.WorldPosition.Y;
        }

        public void BuidlingUnselected(BuildingControl control)
        {
            m_buildingControlPanel.Enabled = false;
            m_startPointCheckBox.Enabled = false;
            m_startPointCheckBox.Checked = false;
            m_endPointCheckBox.Enabled = false;
            m_endPointCheckBox.Checked = false;
        }

        private void m_buildingYPosController_ValueChanged(object sender, EventArgs e)
        {
            foreach (BuildingControl building in m_buildingControls)
            {
                if (building.Selected)
                {
                    Vector3 position = building.WorldPosition;
                    position.Y = (float)m_buildingYPosController.Value;
                    building.WorldPosition = position;

                    break;
                }
            }
        }

        private void m_startPointCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (m_startPointCheckBox.Enabled)
            {
                if (m_startPointCheckBox.Checked)
                {
                    foreach (BuildingControl control in m_buildingControls)
                    {
                        if (control.Selected)
                        {
                            m_startBuilding = control;
                            return;
                        }

                    }
                }
                else
                {
                    m_startBuilding = null;
                }
            }
        }

        private void m_endPointCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (m_endPointCheckBox.Enabled)
            {
                if (m_endPointCheckBox.Checked)
                {
                    foreach (BuildingControl control in m_buildingControls)
                    {
                        if (control.Selected)
                        {
                            m_endBuilding = control;
                            return;
                        }

                    }
                }
                else
                {
                    m_endBuilding = null;
                }
            }
        }
    }
}
