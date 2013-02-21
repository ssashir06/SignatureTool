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
        public SignatoryEditorForm()
        {
            InitializeComponent();
        }

        private void signatoryBindingNavigatorSaveItem_Click(object sender, EventArgs e)
        {
            Validate();
            try
            {
                signatoryBindingSource.EndEdit();
            }
            catch (Exception exp)
            {
                toolStripStatus.Text = exp.Message;
                return;
            }
            tableAdapterManager.UpdateAll(signatureCounterDataSet);
        }

        private void SignatoryEditor_Load(object sender, EventArgs e)
        {
            signatoryTableAdapter.Fill(signatureCounterDataSet.Signatory);
        }

        private void signatoryDataGridView_DataError(object sender, DataGridViewDataErrorEventArgs e)
        {
			if (e.Exception != null) toolStripStatus.Text = e.Exception.Message;
            e.Cancel = true;
        }
    }
}
