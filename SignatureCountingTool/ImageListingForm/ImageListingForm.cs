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
        }

        private void addImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AddImageFiles();
            ShowSignatureList();
            ShowSelectedImage();
            tables.ImageFileTableAdapter.Fill(signatureCounterDataSet1.ImageFile);
        }

        private void scanToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AddScanedImages();
            ShowSignatureList();
            ShowSelectedImage();
            tables.ImageFileTableAdapter.Fill(signatureCounterDataSet1.ImageFile);
        }

        private void dataGridViewImages_SelectionChanged(object sender, EventArgs e)
        {
            ShowSignatureList();
            ShowSelectedImage();
            ShowTrimmingRectangle();
        }

        private void buttonAddSignature_Click(object sender, EventArgs e)
        {
            if (!SelectedImages.Any()) return;
            AddSignature();
            ShowSignatureList();
            ShowSelectedImage();
            ShowTrimmingRectangle();
        }

        private void buttonRemoveSignature_Click(object sender, EventArgs e)
        {
            if (listBoxSignatures.SelectedItem == null) return;
            RemoveSignature();
            ShowSignatureList();
            ShowSelectedImage();
            ShowTrimmingRectangle();
        }

        private void listBoxSignatures_SelectedIndexChanged(object sender, EventArgs e)
        {
            ShowSelectedImage();
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

        private void startMatchingToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Validate();
            tables.UpdateAll(signatureCounterDataSet1);
            MatchSignatures();
            ShowSignatureList();
        }

        private void editToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ShowSignatoryEditorForm();
        }

        private void dataGridViewImages_UserDeletedRow(object sender, DataGridViewRowEventArgs e)
        {
            Validate();
            signatureCounterDataSet1.Clear();
            tables.UpdateAll(signatureCounterDataSet1);
            tables.SignatureTableAdapter.Fill(signatureCounterDataSet1.Signature);

            ShowSignatureList();
            ShowSelectedImage();
            ShowTrimmingRectangle();
        }

        private void ImageListingForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            SaveModel();
        }
    }
}
