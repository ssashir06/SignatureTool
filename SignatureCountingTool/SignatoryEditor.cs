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
    public partial class SignatoryEditor : Form
    {
        public SignatoryEditor()
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
            catch (ConstraintException exp)
            {
                MessageBox.Show(exp.Message);
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
