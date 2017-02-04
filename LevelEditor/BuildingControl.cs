using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LevelEditor
{
    public partial class BuildingControl : UserControl
    {
        private bool m_selected = false;
        public bool Selected { get { return m_selected; } set { Select(value); } }
        public bool Dragging { get; set; }

        private Vector3 m_worldSize, m_worldPosition;
        public Vector3 WorldSize { get { return m_worldSize; } set { m_worldSize.Y = value.Y; Size = m_editor.WorldToEditorSize(value); } }
        public Vector3 WorldPosition { get { return m_worldPosition; } set { m_worldPosition.Y = value.Y; Location = m_editor.WorldToEditorCoords(value); } }

        public string ObjectName { get; set; }

        private Point m_previousMousePos;

        private EditorWindow m_editor;

        public BuildingControl(EditorWindow editor)
        {
            InitializeComponent();

            m_editor = editor;

            this.LocationChanged += BuildingControl_LocationChanged;
            this.SizeChanged += BuildingControl_SizeChanged;

            BackColor = Color.Red;

            Size = new Size(10, 10);
        }

        private void BuildingControl_SizeChanged(object sender, EventArgs e)
        {
            Vector3 worldSize = m_editor.EditorToWorldSize(Size);
            m_worldSize.X = worldSize.X; m_worldSize.Z = worldSize.Z;
        }

        private void BuildingControl_LocationChanged(object sender, EventArgs e)
        {
            Vector3 worldPosition = m_editor.EditorToWorldCoords(Location);
            m_worldPosition.X = worldPosition.X; m_worldPosition.Z = worldPosition.Z;

            int B = (int)(m_worldPosition.Y / 100000 * 255);
            int R = (255 - B);
            BackColor = Color.FromArgb(R, 0, B);
        }

        protected override void OnClick(EventArgs e)
        {
            base.OnClick(e);

            bool previous = Selected;
            m_editor.UnselectAll();
            Selected = !previous;
        }

        private void Select(bool val)
        {
            m_selected = val;
            if(val)
            {
                BackColor = Color.Blue;
                m_editor.BuildingSelected(this);
            }
            else
            {
                BackColor = Color.Red;
                m_editor.BuidlingUnselected(this);
            }
        }

        protected override void OnMouseDown(MouseEventArgs e)
        {
            base.OnMouseDown(e);
            Dragging = true;
            m_previousMousePos = e.Location;
        }

        protected override void OnMouseUp(MouseEventArgs e)
        {
            base.OnMouseUp(e);
            Dragging = false;
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);

            if (Dragging)
                Location = new Point(Location.X + (e.X - m_previousMousePos.X), Location.Y + (e.Y - m_previousMousePos.Y));
        }
    }
}
