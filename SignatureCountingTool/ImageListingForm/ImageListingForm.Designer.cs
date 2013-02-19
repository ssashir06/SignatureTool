﻿namespace SignatureCountingTool
{
    partial class ImageListingForm
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
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.quitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadModelToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveModelToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.startMatchingToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.imageToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.scanToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.addImageToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.signatoryToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editSignatoryToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatus = new System.Windows.Forms.ToolStripStatusLabel();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.dataGridViewImages = new System.Windows.Forms.DataGridView();
            this.signatureCounterDataSet1 = new SignatureCountingTool.Database.SignatureCounterDataSet();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.comboBoxRectangleType = new System.Windows.Forms.ComboBox();
            this.buttonRemoveRectangle = new System.Windows.Forms.Button();
            this.buttonAddRectangle = new System.Windows.Forms.Button();
            this.pictureBoxTaken = new System.Windows.Forms.PictureBox();
            this.splitContainer3 = new System.Windows.Forms.SplitContainer();
            this.propertyGridTrim = new System.Windows.Forms.PropertyGrid();
            this.listBoxSignatures = new System.Windows.Forms.ListBox();
            this.iDDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.takenDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.fileNameDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.directoryDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.mD5DataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.menuStrip1.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewImages)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.signatureCounterDataSet1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).BeginInit();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxTaken)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer3)).BeginInit();
            this.splitContainer3.Panel1.SuspendLayout();
            this.splitContainer3.Panel2.SuspendLayout();
            this.splitContainer3.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItem1,
            this.fileToolStripMenuItem,
            this.imageToolStripMenuItem,
            this.signatoryToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(878, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.quitToolStripMenuItem});
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(37, 20);
            this.toolStripMenuItem1.Text = "File";
            // 
            // quitToolStripMenuItem
            // 
            this.quitToolStripMenuItem.Name = "quitToolStripMenuItem";
            this.quitToolStripMenuItem.Size = new System.Drawing.Size(97, 22);
            this.quitToolStripMenuItem.Text = "Quit";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadModelToolStripMenuItem,
            this.saveModelToolStripMenuItem,
            this.toolStripSeparator1,
            this.startMatchingToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(53, 20);
            this.fileToolStripMenuItem.Text = "Model";
            // 
            // loadModelToolStripMenuItem
            // 
            this.loadModelToolStripMenuItem.Name = "loadModelToolStripMenuItem";
            this.loadModelToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.loadModelToolStripMenuItem.Text = "Load Model";
            // 
            // saveModelToolStripMenuItem
            // 
            this.saveModelToolStripMenuItem.Name = "saveModelToolStripMenuItem";
            this.saveModelToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.saveModelToolStripMenuItem.Text = "Save Model";
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(149, 6);
            // 
            // startMatchingToolStripMenuItem
            // 
            this.startMatchingToolStripMenuItem.Name = "startMatchingToolStripMenuItem";
            this.startMatchingToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.startMatchingToolStripMenuItem.Text = "Start Matching";
            // 
            // imageToolStripMenuItem
            // 
            this.imageToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.scanToolStripMenuItem,
            this.toolStripSeparator2,
            this.addImageToolStripMenuItem});
            this.imageToolStripMenuItem.Name = "imageToolStripMenuItem";
            this.imageToolStripMenuItem.Size = new System.Drawing.Size(52, 20);
            this.imageToolStripMenuItem.Text = "Image";
            // 
            // scanToolStripMenuItem
            // 
            this.scanToolStripMenuItem.Name = "scanToolStripMenuItem";
            this.scanToolStripMenuItem.Size = new System.Drawing.Size(187, 22);
            this.scanToolStripMenuItem.Text = "Start Camera Scanner";
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(184, 6);
            // 
            // addImageToolStripMenuItem
            // 
            this.addImageToolStripMenuItem.Name = "addImageToolStripMenuItem";
            this.addImageToolStripMenuItem.Size = new System.Drawing.Size(187, 22);
            this.addImageToolStripMenuItem.Text = "Add";
            this.addImageToolStripMenuItem.Click += new System.EventHandler(this.addImageToolStripMenuItem_Click);
            // 
            // signatoryToolStripMenuItem
            // 
            this.signatoryToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.editSignatoryToolStripMenuItem});
            this.signatoryToolStripMenuItem.Name = "signatoryToolStripMenuItem";
            this.signatoryToolStripMenuItem.Size = new System.Drawing.Size(69, 20);
            this.signatoryToolStripMenuItem.Text = "Signatory";
            // 
            // editSignatoryToolStripMenuItem
            // 
            this.editSignatoryToolStripMenuItem.Name = "editSignatoryToolStripMenuItem";
            this.editSignatoryToolStripMenuItem.Size = new System.Drawing.Size(94, 22);
            this.editSignatoryToolStripMenuItem.Text = "Edit";
            this.editSignatoryToolStripMenuItem.Click += new System.EventHandler(this.editSignatoryToolStripMenuItem_Click);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatus});
            this.statusStrip1.Location = new System.Drawing.Point(0, 455);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(878, 22);
            this.statusStrip1.TabIndex = 1;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStripStatus
            // 
            this.toolStripStatus.Name = "toolStripStatus";
            this.toolStripStatus.Size = new System.Drawing.Size(863, 17);
            this.toolStripStatus.Spring = true;
            this.toolStripStatus.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 24);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.dataGridViewImages);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.splitContainer2);
            this.splitContainer1.Size = new System.Drawing.Size(878, 431);
            this.splitContainer1.SplitterDistance = 323;
            this.splitContainer1.TabIndex = 2;
            // 
            // dataGridViewImages
            // 
            this.dataGridViewImages.AutoGenerateColumns = false;
            this.dataGridViewImages.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.AllCells;
            this.dataGridViewImages.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCells;
            this.dataGridViewImages.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridViewImages.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.iDDataGridViewTextBoxColumn,
            this.takenDataGridViewTextBoxColumn,
            this.fileNameDataGridViewTextBoxColumn,
            this.directoryDataGridViewTextBoxColumn,
            this.mD5DataGridViewTextBoxColumn});
            this.dataGridViewImages.DataMember = "ImageFile";
            this.dataGridViewImages.DataSource = this.signatureCounterDataSet1;
            this.dataGridViewImages.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dataGridViewImages.EditMode = System.Windows.Forms.DataGridViewEditMode.EditProgrammatically;
            this.dataGridViewImages.Location = new System.Drawing.Point(0, 0);
            this.dataGridViewImages.MultiSelect = false;
            this.dataGridViewImages.Name = "dataGridViewImages";
            this.dataGridViewImages.ReadOnly = true;
            this.dataGridViewImages.RowTemplate.Height = 21;
            this.dataGridViewImages.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dataGridViewImages.Size = new System.Drawing.Size(323, 431);
            this.dataGridViewImages.TabIndex = 0;
            this.dataGridViewImages.SelectionChanged += new System.EventHandler(this.dataGridViewImages_SelectionChanged);
            // 
            // signatureCounterDataSet1
            // 
            this.signatureCounterDataSet1.DataSetName = "SignatureCounterDataSet";
            this.signatureCounterDataSet1.Prefix = "SignatureCounterDataTable";
            this.signatureCounterDataSet1.SchemaSerializationMode = System.Data.SchemaSerializationMode.IncludeSchema;
            // 
            // splitContainer2
            // 
            this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer2.Location = new System.Drawing.Point(0, 0);
            this.splitContainer2.Name = "splitContainer2";
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.Controls.Add(this.comboBoxRectangleType);
            this.splitContainer2.Panel1.Controls.Add(this.buttonRemoveRectangle);
            this.splitContainer2.Panel1.Controls.Add(this.buttonAddRectangle);
            this.splitContainer2.Panel1.Controls.Add(this.pictureBoxTaken);
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.Controls.Add(this.splitContainer3);
            this.splitContainer2.Size = new System.Drawing.Size(551, 431);
            this.splitContainer2.SplitterDistance = 350;
            this.splitContainer2.TabIndex = 0;
            // 
            // comboBoxRectangleType
            // 
            this.comboBoxRectangleType.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.comboBoxRectangleType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxRectangleType.FormattingEnabled = true;
            this.comboBoxRectangleType.Location = new System.Drawing.Point(3, 404);
            this.comboBoxRectangleType.Name = "comboBoxRectangleType";
            this.comboBoxRectangleType.Size = new System.Drawing.Size(188, 23);
            this.comboBoxRectangleType.TabIndex = 3;
            // 
            // buttonRemoveRectangle
            // 
            this.buttonRemoveRectangle.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonRemoveRectangle.Location = new System.Drawing.Point(275, 405);
            this.buttonRemoveRectangle.Name = "buttonRemoveRectangle";
            this.buttonRemoveRectangle.Size = new System.Drawing.Size(72, 23);
            this.buttonRemoveRectangle.TabIndex = 2;
            this.buttonRemoveRectangle.Text = "Remove";
            this.buttonRemoveRectangle.UseVisualStyleBackColor = true;
            // 
            // buttonAddRectangle
            // 
            this.buttonAddRectangle.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonAddRectangle.Location = new System.Drawing.Point(197, 405);
            this.buttonAddRectangle.Name = "buttonAddRectangle";
            this.buttonAddRectangle.Size = new System.Drawing.Size(72, 23);
            this.buttonAddRectangle.TabIndex = 1;
            this.buttonAddRectangle.Text = "Add";
            this.buttonAddRectangle.UseVisualStyleBackColor = true;
            this.buttonAddRectangle.Click += new System.EventHandler(this.buttonAddRectangle_Click);
            // 
            // pictureBoxTaken
            // 
            this.pictureBoxTaken.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.pictureBoxTaken.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.pictureBoxTaken.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.pictureBoxTaken.Location = new System.Drawing.Point(3, 3);
            this.pictureBoxTaken.Name = "pictureBoxTaken";
            this.pictureBoxTaken.Size = new System.Drawing.Size(344, 395);
            this.pictureBoxTaken.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pictureBoxTaken.TabIndex = 0;
            this.pictureBoxTaken.TabStop = false;
            // 
            // splitContainer3
            // 
            this.splitContainer3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer3.Location = new System.Drawing.Point(0, 0);
            this.splitContainer3.Name = "splitContainer3";
            this.splitContainer3.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer3.Panel1
            // 
            this.splitContainer3.Panel1.Controls.Add(this.propertyGridTrim);
            // 
            // splitContainer3.Panel2
            // 
            this.splitContainer3.Panel2.Controls.Add(this.listBoxSignatures);
            this.splitContainer3.Size = new System.Drawing.Size(197, 431);
            this.splitContainer3.SplitterDistance = 314;
            this.splitContainer3.TabIndex = 0;
            // 
            // propertyGridTrim
            // 
            this.propertyGridTrim.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.propertyGridTrim.Location = new System.Drawing.Point(3, 3);
            this.propertyGridTrim.Name = "propertyGridTrim";
            this.propertyGridTrim.Size = new System.Drawing.Size(191, 310);
            this.propertyGridTrim.TabIndex = 0;
            // 
            // listBoxSignatures
            // 
            this.listBoxSignatures.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.listBoxSignatures.FormattingEnabled = true;
            this.listBoxSignatures.ItemHeight = 15;
            this.listBoxSignatures.Location = new System.Drawing.Point(3, 3);
            this.listBoxSignatures.Name = "listBoxSignatures";
            this.listBoxSignatures.Size = new System.Drawing.Size(191, 109);
            this.listBoxSignatures.TabIndex = 0;
            // 
            // iDDataGridViewTextBoxColumn
            // 
            this.iDDataGridViewTextBoxColumn.DataPropertyName = "ID";
            this.iDDataGridViewTextBoxColumn.HeaderText = "ID";
            this.iDDataGridViewTextBoxColumn.Name = "iDDataGridViewTextBoxColumn";
            this.iDDataGridViewTextBoxColumn.Width = 43;
            // 
            // takenDataGridViewTextBoxColumn
            // 
            this.takenDataGridViewTextBoxColumn.DataPropertyName = "Taken";
            this.takenDataGridViewTextBoxColumn.HeaderText = "Taken";
            this.takenDataGridViewTextBoxColumn.Name = "takenDataGridViewTextBoxColumn";
            this.takenDataGridViewTextBoxColumn.Width = 64;
            // 
            // fileNameDataGridViewTextBoxColumn
            // 
            this.fileNameDataGridViewTextBoxColumn.DataPropertyName = "FileName";
            this.fileNameDataGridViewTextBoxColumn.HeaderText = "FileName";
            this.fileNameDataGridViewTextBoxColumn.Name = "fileNameDataGridViewTextBoxColumn";
            this.fileNameDataGridViewTextBoxColumn.Width = 82;
            // 
            // directoryDataGridViewTextBoxColumn
            // 
            this.directoryDataGridViewTextBoxColumn.DataPropertyName = "Directory";
            this.directoryDataGridViewTextBoxColumn.HeaderText = "Directory";
            this.directoryDataGridViewTextBoxColumn.Name = "directoryDataGridViewTextBoxColumn";
            this.directoryDataGridViewTextBoxColumn.Width = 80;
            // 
            // mD5DataGridViewTextBoxColumn
            // 
            this.mD5DataGridViewTextBoxColumn.DataPropertyName = "MD5";
            this.mD5DataGridViewTextBoxColumn.HeaderText = "MD5";
            this.mD5DataGridViewTextBoxColumn.Name = "mD5DataGridViewTextBoxColumn";
            this.mD5DataGridViewTextBoxColumn.Width = 57;
            // 
            // ImageListingForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(878, 477);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.menuStrip1);
            this.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.MainMenuStrip = this.menuStrip1;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "ImageListingForm";
            this.Text = "ImageListingForm";
            this.Shown += new System.EventHandler(this.ImageListingForm_Shown);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewImages)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.signatureCounterDataSet1)).EndInit();
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).EndInit();
            this.splitContainer2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxTaken)).EndInit();
            this.splitContainer3.Panel1.ResumeLayout(false);
            this.splitContainer3.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer3)).EndInit();
            this.splitContainer3.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatus;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.SplitContainer splitContainer2;
        private System.Windows.Forms.SplitContainer splitContainer3;
        private System.Windows.Forms.Button buttonRemoveRectangle;
        private System.Windows.Forms.Button buttonAddRectangle;
        private System.Windows.Forms.PictureBox pictureBoxTaken;
        private System.Windows.Forms.PropertyGrid propertyGridTrim;
        private System.Windows.Forms.ListBox listBoxSignatures;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem quitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadModelToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveModelToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem startMatchingToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem imageToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem scanToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem addImageToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.DataGridView dataGridViewImages;
        private Database.SignatureCounterDataSet signatureCounterDataSet1;
        private System.Windows.Forms.ToolStripMenuItem signatoryToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editSignatoryToolStripMenuItem;
        private System.Windows.Forms.ComboBox comboBoxRectangleType;
        private System.Windows.Forms.DataGridViewTextBoxColumn iDDataGridViewTextBoxColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn takenDataGridViewTextBoxColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn fileNameDataGridViewTextBoxColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn directoryDataGridViewTextBoxColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn mD5DataGridViewTextBoxColumn;
    }
}