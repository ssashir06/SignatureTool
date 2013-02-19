using System;
using System.Collections;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlClient;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Windows.Forms;
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

        #region variables
        TableAdapterManager tables;
        Point? mouse_drag_offset = null;
        #endregion

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

        SignatureInfomationForListBox SelectedSignature
        {
            get
            {
                return listBoxSignatures.SelectedItem as SignatureInfomationForListBox;
            }
        }

        Double? SignatureImageZoom
        {
            get
            {
                if (pictureBoxTaken.Image == null) return null;
                return new double[]{
					pictureBoxTaken.Width / (double)pictureBoxTaken.Image.Width,
					pictureBoxTaken.Height / (double)pictureBoxTaken.Image.Height }.Min();
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

            ShowSignatureList();
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

            ShowTrimmingRectangle();
        }

        void ShowTrimmingRectangle()
        {
            SignatureInfomationForListBox signature = SelectedSignature;
            if (signature == null)
            {
                panelTrimmingRectangle.Visible = false;
                return;
            }

            Rectangle r = new Rectangle()
            {
                X = (int)signature.Trimming.x,
                Y = (int)signature.Trimming.y,
                Width = (int)signature.Trimming.w,
                Height = (int)signature.Trimming.h,
            };

            ShowTrimmingRectangle(r);
        }

        void ShowTrimmingRectangle(Rectangle r)
        {
            double? zoom = SignatureImageZoom;
            if (zoom == null)
            {
                panelTrimmingRectangle.Visible = false;
                return;
            }

            var image_position = new
            {
                x = (pictureBoxTaken.Width - pictureBoxTaken.Image.Width * zoom) / 2,
                y = (pictureBoxTaken.Height - pictureBoxTaken.Image.Height * zoom) / 2,
            };

            panelTrimmingRectangle.Location = new Point()
            {
                X = (int)(r.X * zoom + pictureBoxTaken.Location.X + image_position.x),
                Y = (int)(r.Y * zoom + pictureBoxTaken.Location.Y + image_position.y),
            };
            panelTrimmingRectangle.Width =  (int)(r.Width * zoom);
            panelTrimmingRectangle.Height = (int)(r.Height * zoom);
            panelTrimmingRectangle.Visible = true;

            Bitmap trimmed = new Bitmap(panelTrimmingRectangle.Size.Width, panelTrimmingRectangle.Size.Height);
            Graphics g = Graphics.FromImage(trimmed);
            g.DrawImage(pictureBoxTaken.Image, new Rectangle(new Point(0, 0), panelTrimmingRectangle.Size), r, GraphicsUnit.Pixel);
            panelTrimmingRectangle.BackgroundImage = trimmed;
        }
        #endregion

        #region Trimming Rectangle
        void UpdateTrimmingRectangle(SignatureInfomationForListBox signature, Rectangle r)
        {
            tables.TrimTableAdapter.Update(r.Width, r.Height, r.X, r.Y, signature.Trimming.ID);
            ShowSignatureList();

            foreach (object obj_item in listBoxSignatures.Items)
            {
                if (obj_item as SignatureInfomationForListBox == null) continue;
                if (((SignatureInfomationForListBox)obj_item).Trimming.ID == signature.Trimming.ID)
                {
                    listBoxSignatures.SelectedItem = obj_item;
                    break;
                }
            }
            ShowTrimmingRectangle();
        }

        void ResizeTrimmingRectangle(MouseEventArgs e, bool moving)
        {
            double? zoom = SignatureImageZoom;
            if (zoom == null || (e.Button != MouseButtons.Left && moving)) return;
            if (mouse_drag_offset == null) mouse_drag_offset = e.Location;

            SignatureInfomationForListBox signature = SelectedSignature;
            if (signature == null) return;
            Rectangle r = new Rectangle()
            {
                X = (int)signature.Trimming.x,
                Y = (int)signature.Trimming.y,
                Width = (int)((panelTrimmingRectangle.Width + e.X - mouse_drag_offset.Value.X) / zoom),
                Height = (int)((panelTrimmingRectangle.Height + e.Y - mouse_drag_offset.Value.Y) / zoom),
            };
            if (r.Width < pictureBoxDraggingBR.Width * zoom || r.Height < pictureBoxDraggingBR.Height * zoom) return;

            if (e.Button == MouseButtons.Left && moving)
            {
                ShowTrimmingRectangle(r);
            }
            else if (!moving)
            {
                UpdateTrimmingRectangle(signature, r);
                mouse_drag_offset = null;
            }
        }

        void MoveTrimmingRectangle(MouseEventArgs e, bool moving)
        {
            double? zoom = SignatureImageZoom;
            if (zoom == null || (e.Button != MouseButtons.Left && moving)) return;
            if (mouse_drag_offset == null) mouse_drag_offset = e.Location;

            var image_position = new
            {
                x = (pictureBoxTaken.Width - pictureBoxTaken.Image.Width * zoom) / 2,
                y = (pictureBoxTaken.Height - pictureBoxTaken.Image.Height * zoom) / 2,
            };

            SignatureInfomationForListBox signature = SelectedSignature;
            if (signature == null) return;
            Rectangle r = new Rectangle()
            {
                X = (int)((panelTrimmingRectangle.Location.X + e.X - image_position.x - mouse_drag_offset.Value.X) / zoom),
                Y = (int)((panelTrimmingRectangle.Location.Y + e.Y - image_position.y - mouse_drag_offset.Value.Y) / zoom),
                Width = (int)signature.Trimming.w,
                Height = (int)signature.Trimming.h,
            };

            if (e.Button == MouseButtons.Left && moving)
            {
                ShowTrimmingRectangle(r);
            }
            else if (!moving)
            {
                UpdateTrimmingRectangle(signature, r);
                mouse_drag_offset = null;
            }
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
                signatureCounterDataSet1.Signature.Clear();
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

            var query_last_added =
                from signature in tables.SignatureTableAdapter.GetData()
                join trimming in tables.TrimTableAdapter.GetData() on signature.TrimID equals trimming.ID
                where signature.TypeID == row_type.ID
                orderby signature.ID
                select new { Signature = signature, Trimming = trimming };
            int trim_id;
            if (query_last_added.Any())
            {
                SignatureCounterDataSet.TrimRow t = query_last_added.Last().Trimming;
                signatureCounterDataSet1.Signature.Clear();
                trim_id = AddTrimming(t.x, t.y, t.w, t.h);
            }
            else
            {
                signatureCounterDataSet1.Signature.Clear();
                trim_id = AddTrimming(10, 10, 100, 100);
            }

            tables.SignatureTableAdapter.Insert(id, row_image.ID, trim_id, row_type.ID);
            tables.SignatureTableAdapter.Fill(signatureCounterDataSet1.Signature);
        }

        int AddTrimming(double x, double y, double w, double h)
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