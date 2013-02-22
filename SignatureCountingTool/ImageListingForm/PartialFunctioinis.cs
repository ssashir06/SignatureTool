using System;
using System.Collections;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlClient;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using Signature.CountingTool.Database;
using Signature.CountingTool.Database.SignatureCounterDataSetTableAdapters;
using Signature.Guess.CLI;
using CameraScanner.CLI;
using CameraScanner.GUI.CLI;

namespace Signature.CountingTool
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
                return String.Format("{0}: {1} {2}", Type.Name, Trimming.ToString(), Signature.Trainer ? "a Trainer" : "Not a Trainer");
            }
        };
        #endregion

        #region Variable
        TableAdapterManager tables;
        Point? mouse_drag_offset = null;
        IGuessSignature guess;
        SignatoryEditorForm signatory_editor;
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

        #region Initialize function
		void SetupTables()
        {
            tables = new TableAdapterManager()
            {
                ImageFileTableAdapter = new ImageFileTableAdapter(),
                TypeTableAdapter = new TypeTableAdapter(),
                SignatureTableAdapter = new SignatureTableAdapter(),
                SignatoryTableAdapter = new SignatoryTableAdapter(),
                TrimTableAdapter = new TrimTableAdapter(),
				MatchingTableAdapter = new MatchingTableAdapter(),
            };
            tables.ImageFileTableAdapter.Fill(signatureCounterDataSet1.ImageFile);
        }

        void Start()
        {
            ShowTypeList();
            ShowSignatureList();
            ShowSignatoryEditorForm();
		}

        void InitializeMatchingModel()
        {
            guess = new GuessSignatureEvalEasy();
        }
        #endregion

        #region Show shomething
		void ShowSignatoryEditorForm()
        {
            if (signatory_editor == null || !signatory_editor.Visible)
            {
                signatory_editor = new SignatoryEditorForm();
                signatory_editor.Show();
            }
            else
            {
                signatory_editor.Activate();
            }
        }

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

			System.Drawing.Image image_loaded = Bitmap.FromFile(row.FullPath);
            pictureBoxTaken.Image = image_loaded;

            ShowSignatureList();
        }

        void ShowSignatureList()
        {
            SignatureCounterDataSet.ImageFileRow row_image_file = SelectedImage;
            if (row_image_file == null)
            {
                listBoxSignatures.Items.Clear();
                return;
            }

            tables.UpdateAll(signatureCounterDataSet1);

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
        void AddImageFiles()
        {
			OpenFileDialog dialog = new OpenFileDialog()
			{
				Filter = "Image files|*.jpg",
				Multiselect = true,
			};
			if (dialog.ShowDialog() == DialogResult.Cancel) return;

            AddImages(dialog.FileNames.ToList());
        }

        void AddScanedImages()
        {
            Scan scanner = new Scan()
            {
                ImageDirectory = Directory.GetCurrentDirectory(),
            };
            scanner.ShowDialog();
            List<IScannedImage> images = scanner.ScannedImages;

            if (images == null) return;
            AddImages(images.Select(X => Path.Combine(X.ImagePath, X.FileName)).ToList());
        }

        void AddImages(List<String> file_names)
		{
            var query_ids = from images in tables.ImageFileTableAdapter.GetData() select images.ID;
            int id = query_ids.Any() ? query_ids.Max() + 1 : 0;
            List<int> files_added = new List<int>();

			foreach (String filename_fullpath in file_names)
			{
                tables.ImageFileTableAdapter.Insert(
                    id, null, Path.GetFileName(filename_fullpath), Path.GetDirectoryName(filename_fullpath), null);
                files_added.Add(id++);
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
                return;
            }

            var type_id_aleady_used =
                (from type in tables.TypeTableAdapter.GetData()
                 join signature in tables.SignatureTableAdapter.GetData() on type.ID equals signature.TypeID
                 select type.ID).Distinct().ToList();
            var type_row_aleady_used =
                (from type in tables.TypeTableAdapter.GetData()
                 join type_id in type_id_aleady_used on type.ID equals type_id
                 select type).ToList();
            foreach (int file_id in files_added)
            {
                var query_image_file_row = from image in tables.ImageFileTableAdapter.GetData() where image.ID == file_id select image;
                if (!query_image_file_row.Any()) continue;
                SignatureCounterDataSet.ImageFileRow row_image = query_image_file_row.First();
                foreach (var row_type in type_row_aleady_used)
                {
                    AddSignature(row_image, row_type);
                }
            }
		}

        void AddSignature()
        {
            SignatureCounterDataSet.ImageFileRow row_image = SelectedImage;
            if (row_image == null) return;

            SignatureCounterDataSet.TypeRow row_type = comboBoxRectangleType.SelectedItem as SignatureCounterDataSet.TypeRow;
            if (row_type == null) return;

            AddSignature(row_image, row_type);
        }

		void AddSignature(SignatureCounterDataSet.ImageFileRow row_image, SignatureCounterDataSet.TypeRow row_type)
        {
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

            tables.SignatureTableAdapter.Insert(id, row_image.ID, trim_id, row_type.ID, null, false);
        }

        void RemoveSignature()
        {
            SignatureInfomationForListBox signature_selected = listBoxSignatures.SelectedItem as SignatureInfomationForListBox;
            if (signature_selected == null) return;

            tables.SignatureTableAdapter.Delete(signature_selected.Signature.ID);
            ShowSignatureList();
        }

        int AddTrimming(double x, double y, double w, double h)
        {
            var query_trim = from trim in tables.TrimTableAdapter.GetData() select trim.ID;
            int id = query_trim.Any() ? query_trim.Max() + 1 : 0;

            tables.TrimTableAdapter.Insert(id, w, h, x, y);
            return id;
        }
        #endregion

        #region Matching Model
        void LoadModel()
        {
            InitializeMatchingModel();

            OpenFileDialog dialog = new OpenFileDialog()
            {
                Filter = guess.ModelFileSuffixFilter(),
            };

            toolStripStatus.Text = null;
            if (dialog.ShowDialog() == DialogResult.Cancel) return;

            toolStripStatus.Text = "Loading Matching Model.";
            Refresh();

            try
            {
                guess.LoadModel(dialog.FileName);
            }
            catch (Exception e)
            {
                toolStripStatus.Text = e.Message;
                return;
            }

            toolStripStatus.Text = "Matching Model is loaded.";
        }

		void MatchSignatures()
        {
            if (guess == null)
            {
                MessageBox.Show("Matching Model is currently null.");
                return;
            }

            List<SignatureCounterDataSet.SignatureRow> target = new List<SignatureCounterDataSet.SignatureRow>();
            foreach (object obj_signature in listBoxSignatures.Items)
            {
                if (obj_signature as SignatureInfomationForListBox != null) target.Add((obj_signature as SignatureInfomationForListBox).Signature);
            }

            guess.Strip();
            (new SignaturesGuessForm(guess, tables, target)).ShowDialog();
        }
        #endregion
    }
}