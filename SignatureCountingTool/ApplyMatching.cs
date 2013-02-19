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
    public partial class ApplyMatching : Form
    {
        public ApplyMatching()
        {
            InitializeComponent();
        }

        private void ApplyMatching_Load(object sender, EventArgs e)
        {
            // TODO: このコード行はデータを 'signatureCounterDataSet.Signature' テーブルに読み込みます。必要に応じて移動、または削除をしてください。
            this.signatureTableAdapter.Fill(this.signatureCounterDataSet.Signature);

        }
    }
}
