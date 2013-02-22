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
    public partial class SignatoryEditorForm : Form
    {
        bool _changed;

        bool Changed
        {
            get { return _changed; }
            set
            {
                if (value) toolStripStatus.Text = "Changed";
                else toolStripStatus.Text = null;
                _changed = value;
            }
        }

        public SignatoryEditorForm()
        {
            InitializeComponent();
        }

		void Save()
        {
            Validate();
            signatoryBindingSource.EndEdit();
            tableAdapterManager.UpdateAll(signatureCounterDataSet);
            Changed = false;
            toolStripStatus.Text = "Saved";
        }

        private void SignatoryEditor_Load(object sender, EventArgs e)
        {
            signatoryTableAdapter.Fill(signatureCounterDataSet.Signatory);
            Changed = false;
        }

        private void SignatoryEditorForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (e.CloseReason == CloseReason.UserClosing && Changed)
            {
				switch (MessageBox.Show("Save signatory infomation befor closing?", Text, MessageBoxButtons.YesNoCancel))
                {
                	case DialogResult.Yes:
                        try
                        {
                            Save();
                        }
                        catch (Exception exp)
                        {
                            MessageBox.Show("Saving signatory infomation was failed:\n" + exp.Message);
                            e.Cancel = true;
                        }
                        break;
                    case DialogResult.No:
						break;
                    case DialogResult.Cancel:
                        e.Cancel = true;
						break;
				}
            }
        }

        private void dataGridView1_DataError(object sender, DataGridViewDataErrorEventArgs e)
        {
			if (e.Exception != null) toolStripStatus.Text = e.Exception.Message;
            e.Cancel = true;
        }

        private void dataGridView1_CellValueChanged(object sender, DataGridViewCellEventArgs e)
        {
            Changed = true;
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                Save();
            }
            catch (Exception exp)
            {
                toolStripStatus.Text = exp.Message;
                return;
            }
        }
    }
}
