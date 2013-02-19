using System;
using System.IO;
using System.Data;
using System.Data.SqlClient; 
using System.Windows.Forms;
using System.Linq;
using System.Drawing;
using System.Collections;
using System.Collections.Generic;
using SignatureCountingTool.Database;
using SignatureCountingTool.Database.SignatureCounterDataSetTableAdapters;

namespace SignatureCountingTool
{
	public partial class ImageListingForm
	{
        #region Definetion of class/signature
        protected class SignatureInfomationForListBox
        {
            public SignatureCounterDataSet.SignatureRow Signature { get; set; }
            public SignatureCounterDataSet.TrimRow Trimming { get; set; }
            public SignatureCounterDataSet.TypeRow Type { get; set; }
            public override string ToString()
            {
                return String.Format("{0}: {1}", Type.Name, Trimming.ToString());
            }
        };
        #endregion

        TableAdapterManager tables;

        #region Property
        SignatureCounterDataSet.ImageFileRow SelectedImage
        {
            get
            {
                if (dataGridViewImages.SelectedRows.Count == 0) return null;

                DataRowView row_view = dataGridViewImages.SelectedRows[0].DataBoundItem as DataRowView;
                if (row_view == null) return null;

                SignatureCounterDataSet.ImageFileRow row_image_file = row_view.Row as SignatureCounterDataSet.ImageFileRow;
                return row_image_file;
            }
        }
        #endregion

        void Start()
        {
            tables = new TableAdapterManager()
            {
                ImageFileTableAdapter = new ImageFileTableAdapter(),
                TypeTableAdapter = new TypeTableAdapter(),
                SignatureTableAdapter = new SignatureTableAdapter(),
                SignatoryTableAdapter = new SignatoryTableAdapter(),
                TrimTableAdapter = new TrimTableAdapter()
            };
			tables.ImageFileTableAdapter.Fill(signatureCounterDataSet1.ImageFile);
            tables.TrimTableAdapter.Fill(signatureCounterDataSet1.Trim);
            tables.TypeTableAdapter.Fill(signatureCounterDataSet1.Type);
            tables.SignatureTableAdapter.Fill(signatureCounterDataSet1.Signature);

            ShowTypeList();
            ShowSignatureList();
		}

        #region Show shomething
        void ShowTypeList()
        {
            var query_types = from type in tables.TypeTableAdapter.GetData() select type;
            comboBoxRectangleType.BeginUpdate();
            comboBoxRectangleType.Items.Clear();
            foreach (var type in query_types)
            {
                comboBoxRectangleType.Items.Add(type);
            }
            if (query_types.Any()) comboBoxRectangleType.SelectedIndex = 0;
            comboBoxRectangleType.EndUpdate();
        }

        void ShowSelectedImage()
        {
            SignatureCounterDataSet.ImageFileRow row = SelectedImage;
            if (row == null)
            {
                pictureBoxTaken.Image = null;
                return;
            }

			String image_fullpath = Path.Combine(row.Directory, row.FileName);
			Image image_loaded = Bitmap.FromFile(image_fullpath);
            pictureBoxTaken.Image = image_loaded;
        }

        void ShowSignatureList()
        {
            SignatureCounterDataSet.ImageFileRow row_image_file = SelectedImage;
            if (row_image_file == null) return;

            var query_signatures =
                from signature in tables.SignatureTableAdapter.GetData()
                join triming in tables.TrimTableAdapter.GetData() on signature.TrimID equals triming.ID
                join type in tables.TypeTableAdapter.GetData() on signature.TypeID equals type.ID
                where signature.ImageFileID == row_image_file.ID
                select new SignatureInfomationForListBox()
                {
                    Signature = signature,
                    Trimming = triming,
                    Type = type
                };

            listBoxSignatures.BeginUpdate();
            listBoxSignatures.Items.Clear();
            foreach (var signature in query_signatures)
            {
                listBoxSignatures.Items.Add(signature);
            }
            listBoxSignatures.EndUpdate();
        }
        #endregion

        #region Add/Remove
        void AddImages()
		{
			OpenFileDialog dialog = new OpenFileDialog()
			{
				Filter = "Image files|*.jpg",
				Multiselect = true,
			};
			if (dialog.ShowDialog() == DialogResult.Cancel) return;

            var query_ids = from images in tables.ImageFileTableAdapter.GetData() select images.ID;
            int id = query_ids.Any() ? query_ids.Max() + 1 : 0;

			foreach (String filename_fullpath in dialog.FileNames)
			{
                tables.ImageFileTableAdapter.Insert(
                    id++, null, Path.GetFileName(filename_fullpath), Path.GetDirectoryName(filename_fullpath), null);
			}

            try
            {
                Validate();
                dataGridViewImages.EndEdit();
                tables.ImageFileTableAdapter.Fill(signatureCounterDataSet1.ImageFile);
            }
            catch (Exception e)
            {
                toolStripStatus.Text = e.Message;
            }
		}

        void AddSignature()
        {
            SignatureCounterDataSet.ImageFileRow row_image = SelectedImage;
            if (row_image == null) return;

            SignatureCounterDataSet.TypeRow row_type = comboBoxRectangleType.SelectedItem as SignatureCounterDataSet.TypeRow;
            if (row_type == null) return;

            var query_ids = from signature in tables.SignatureTableAdapter.GetData() select signature.ID;
            int id = query_ids.Any() ? query_ids.Max() + 1 : 0;

            var query_recent_signature =
                from signature in tables.SignatureTableAdapter.GetData()
                where signature.TypeID == row_type.ID
				orderby signature.ID
                select signature;
            int trim_id;
            if (query_recent_signature.Any())
            {
                trim_id = query_recent_signature.Last().TrimID;
            }
            else
            {
                trim_id = AddTrimming(0.1f, 0.1f, 0.8f, 0.8f);//TODO
            }

            tables.SignatureTableAdapter.Insert(id, row_image.ID, trim_id, row_type.ID);
            tables.SignatureTableAdapter.Fill(signatureCounterDataSet1.Signature);
        }

        int AddTrimming(float w, float h, float x, float y)
        {
            var query_trim = from trim in tables.TrimTableAdapter.GetData() select trim.ID;
            int id = query_trim.Any() ? query_trim.Max() + 1 : 0;

            tables.TrimTableAdapter.Insert(id, w, h, x, y);
            tables.TrimTableAdapter.Fill(signatureCounterDataSet1.Trim);
            return id;
        }
        #endregion
    }
}