using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SignatureCountingTool
{
    public partial class ImageListingForm : Form
    {
        public ImageListingForm()
        {
            InitializeComponent();
        }

        private void ImageListingForm_Shown(object sender, EventArgs e)
        {
            Start();
        }

        private void addImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AddImages();
        }

        private void dataGridViewImages_SelectionChanged(object sender, EventArgs e)
        {
            ShowSelectedImage();
        }

        private void editSignatoryToolStripMenuItem_Click(object sender, EventArgs e)
        {
            (new SignatoryEditor()).ShowDialog(this);
        }

        private void buttonAddRectangle_Click(object sender, EventArgs e)
        {
            if (SelectedImage == null) return;
            AddSignature();
            ShowSignatureList();
        }
    }
}
