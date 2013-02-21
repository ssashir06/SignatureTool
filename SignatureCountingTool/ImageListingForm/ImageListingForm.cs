using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Signature.CountingTool
{
    public partial class ImageListingForm : Form
    {
        public ImageListingForm()
        {
            InitializeComponent();
            SetupTables();
        }

        private void ImageListingForm_Shown(object sender, EventArgs e)
        {
            Start();
            InitializeMatchingModel();
            (new SignatoryEditorForm()).Show(this);
        }

        private void addImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AddImages();
            ShowSignatureList();
        }

        private void dataGridViewImages_SelectionChanged(object sender, EventArgs e)
        {
            ShowSelectedImage();
        }

        private void buttonAddSignature_Click(object sender, EventArgs e)
        {
            if (SelectedImage == null) return;
            AddSignature();
            ShowSignatureList();
        }

        private void buttonRemoveSignature_Click(object sender, EventArgs e)
        {
            if (listBoxSignatures.SelectedItem == null) return;
            RemoveSignature();
            ShowSignatureList();
        }

        private void listBoxSignatures_SelectedIndexChanged(object sender, EventArgs e)
        {
            ShowTrimmingRectangle();
        }

        private void pictureBoxTaken_SizeChanged(object sender, EventArgs e)
        {
            ShowTrimmingRectangle();
        }

        private void pictureBoxDraggingBR_MouseMove(object sender, MouseEventArgs e)
        {
            ResizeTrimmingRectangle(e, true);
        }

        private void pictureBoxDraggingBR_MouseUp(object sender, MouseEventArgs e)
        {
            ResizeTrimmingRectangle(e, false);
        }

        private void panelTrimmingRectangle_MouseMove(object sender, MouseEventArgs e)
        {
            MoveTrimmingRectangle(e, true);
        }

        private void panelTrimmingRectangle_MouseUp(object sender, MouseEventArgs e)
        {
            MoveTrimmingRectangle(e, false);
        }

        private void newModelToolStripMenuItem_Click(object sender, EventArgs e)
        {
            InitializeMatchingModel();
        }

        private void loadModelToolStripMenuItem_Click(object sender, EventArgs e)
        {
            LoadModel();
        }

        private void startMatchingToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MatchSignatures();
            ShowSignatureList();
        }
    }
}
