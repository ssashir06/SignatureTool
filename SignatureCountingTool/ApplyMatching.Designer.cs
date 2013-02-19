namespace SignatureCountingTool
{
    partial class ApplyMatching
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.doneToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.cancelToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.signatureCounterDataSet = new SignatureCountingTool.Database.SignatureCounterDataSet();
            this.guessedSignatureBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.signatureBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.signatureTableAdapter = new SignatureCountingTool.Database.SignatureCounterDataSetTableAdapters.SignatureTableAdapter();
            this.tableAdapterManager = new SignatureCountingTool.Database.SignatureCounterDataSetTableAdapters.TableAdapterManager();
            this.menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.signatureCounterDataSet)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.guessedSignatureBindingSource)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.signatureBindingSource)).BeginInit();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.doneToolStripMenuItem,
            this.cancelToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(582, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // doneToolStripMenuItem
            // 
            this.doneToolStripMenuItem.Name = "doneToolStripMenuItem";
            this.doneToolStripMenuItem.Size = new System.Drawing.Size(47, 20);
            this.doneToolStripMenuItem.Text = "Done";
            // 
            // cancelToolStripMenuItem
            // 
            this.cancelToolStripMenuItem.Name = "cancelToolStripMenuItem";
            this.cancelToolStripMenuItem.Size = new System.Drawing.Size(55, 20);
            this.cancelToolStripMenuItem.Text = "Cancel";
            // 
            // statusStrip1
            // 
            this.statusStrip1.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.statusStrip1.Location = new System.Drawing.Point(0, 599);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(582, 22);
            this.statusStrip1.TabIndex = 1;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // signatureCounterDataSet
            // 
            this.signatureCounterDataSet.DataSetName = "SignatureCounterDataSet";
            this.signatureCounterDataSet.Prefix = "SignatureCounterDataTable";
            this.signatureCounterDataSet.SchemaSerializationMode = System.Data.SchemaSerializationMode.IncludeSchema;
            // 
            // guessedSignatureBindingSource
            // 
            this.guessedSignatureBindingSource.DataMember = "GuessedSignature";
            this.guessedSignatureBindingSource.DataSource = this.signatureCounterDataSet;
            // 
            // signatureBindingSource
            // 
            this.signatureBindingSource.DataMember = "Signature";
            this.signatureBindingSource.DataSource = this.signatureCounterDataSet;
            // 
            // signatureTableAdapter
            // 
            this.signatureTableAdapter.ClearBeforeFill = true;
            // 
            // tableAdapterManager
            // 
            this.tableAdapterManager.BackupDataSetBeforeUpdate = false;
            this.tableAdapterManager.ImageFileTableAdapter = null;
            this.tableAdapterManager.MatchingTableAdapter = null;
            this.tableAdapterManager.SelectedSignatoryTableAdapter = null;
            this.tableAdapterManager.SignatoryTableAdapter = null;
            this.tableAdapterManager.SignatureTableAdapter = this.signatureTableAdapter;
            this.tableAdapterManager.TrimTableAdapter = null;
            this.tableAdapterManager.TypeTableAdapter = null;
            this.tableAdapterManager.UpdateOrder = SignatureCountingTool.Database.SignatureCounterDataSetTableAdapters.TableAdapterManager.UpdateOrderOption.InsertUpdateDelete;
            // 
            // ApplyMatching
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(582, 621);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.menuStrip1);
            this.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "ApplyMatching";
            this.Text = "ApplyMatching";
            this.Load += new System.EventHandler(this.ApplyMatching_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.signatureCounterDataSet)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.guessedSignatureBindingSource)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.signatureBindingSource)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripMenuItem doneToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem cancelToolStripMenuItem;
        private Database.SignatureCounterDataSet signatureCounterDataSet;
        private System.Windows.Forms.BindingSource guessedSignatureBindingSource;
        private System.Windows.Forms.BindingSource signatureBindingSource;
        private Database.SignatureCounterDataSetTableAdapters.SignatureTableAdapter signatureTableAdapter;
        private Database.SignatureCounterDataSetTableAdapters.TableAdapterManager tableAdapterManager;

    }
}