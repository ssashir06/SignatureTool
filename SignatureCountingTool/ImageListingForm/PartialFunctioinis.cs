﻿using System;
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
        protected class SignatureInfomationForListBox // FIXME: Rowを保持し続けると異常が発生する(フォームの反応がなくなり，終了する)
        {
            public SignatureCounterDataSet.ImageFileRow ImageFile { get; set; }
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
        const String model_file_name_wo_suffix = "modeldata";
        #endregion

        #region Property
		List<SignatureCounterDataSet.ImageFileRow> SelectedImages
        {
            get
            {
                List<SignatureCounterDataSet.ImageFileRow> rows = new List<SignatureCounterDataSet.ImageFileRow>();
                foreach (DataGridViewRow image in dataGridViewImages.SelectedRows)
                {
                    DataRowView row_view = image.DataBoundItem as DataRowView;
                    if (row_view == null) continue;
                    SignatureCounterDataSet.ImageFileRow row_image = row_view.Row as SignatureCounterDataSet.ImageFileRow;
                    if (row_image == null) continue;
                    rows.Add(row_image);
                }
                return rows;
            }
        }
        SignatureInfomationForListBox SelectedSignature
        {
            get
            {
                if (listBoxSignatures.SelectedItems.Count == 0) return null;

                SignatureInfomationForListBox signature_info = listBoxSignatures.SelectedItem as SignatureInfomationForListBox;
                if (signature_info == null) return null;

                return signature_info;
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

        void ShowSignatureList()
        {
            tables.UpdateAll(signatureCounterDataSet1);

            SignatureInfomationForListBox last_selected = SelectedSignature;
			int? select_index = null;

            listBoxSignatures.BeginUpdate();
            listBoxSignatures.Items.Clear();

            foreach (SignatureCounterDataSet.ImageFileRow image in SelectedImages)
            {
                var query_signatures =
					from signature in tables.SignatureTableAdapter.GetData()
					join trimming in tables.TrimTableAdapter.GetData() on signature.TrimID equals trimming.ID
					join type in tables.TypeTableAdapter.GetData() on signature.TypeID equals type.ID
					where signature.ImageFileID == image.ID
                    select new SignatureInfomationForListBox()
                    {
                        Signature = signature,
                        ImageFile = image,
                        Trimming = trimming,
                        Type = type
                    };

                foreach (var signature_info in query_signatures)
                {
                    if (last_selected != null && signature_info.Signature.ID == last_selected.Signature.ID)
                        select_index = listBoxSignatures.Items.Count;
                    listBoxSignatures.Items.Add(signature_info);
                }
            }

            listBoxSignatures.EndUpdate();
            if (select_index.HasValue) listBoxSignatures.SelectedIndex = select_index.Value;

        }

        void ShowSelectedImage()
        {
            pictureBoxTaken.Image = null;

            if (SelectedSignature != null)
            {
                pictureBoxTaken.Image = Bitmap.FromFile(SelectedSignature.ImageFile.FullPath);
            }
            else if (SelectedImages.Any())
            {
                pictureBoxTaken.Image = Bitmap.FromFile(SelectedImages[0].FullPath);
            }

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
            listBoxSignatures.Items.Clear();

            var query_ids = from images in tables.ImageFileTableAdapter.GetData() select images.ID;
            int id = query_ids.Any() ? query_ids.Max() + 1 : 0;
            List<int> files_added = new List<int>();

			foreach (String filename_fullpath in file_names)
			{
                tables.ImageFileTableAdapter.Insert(
                    id, null, Path.GetFileName(filename_fullpath), Path.GetDirectoryName(filename_fullpath), null);
                files_added.Add(id++);
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
            SignatureCounterDataSet.TypeRow row_type = comboBoxRectangleType.SelectedItem as SignatureCounterDataSet.TypeRow;
            if (row_type == null) return;

            foreach (SignatureCounterDataSet.ImageFileRow row_image in SelectedImages)
            {
                AddSignature(row_image, row_type);
            }
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
		void MatchSignatures()
        {
            if (guess == null && !LoadModel()) return;

            List<SignatureCounterDataSet.SignatureRow> target = new List<SignatureCounterDataSet.SignatureRow>();
            foreach (object obj_signature in listBoxSignatures.Items)
            {
                if (obj_signature as SignatureInfomationForListBox != null) target.Add((obj_signature as SignatureInfomationForListBox).Signature);
            }

            guess.Strip();
            (new SignaturesGuessForm(guess, tables, target)).ShowDialog();
        }

		bool LoadModel()
        {
            toolStripStatus.Text = "Loading Model Data.";
            Refresh();
            guess = new GuessSignatureEvalEasy();
            String model_data = Path.ChangeExtension(model_file_name_wo_suffix, guess.ModelFileSuffix);
            if (File.Exists(model_data))
            {
                try
                {
                    guess.LoadModel(model_data);
                }
                catch (Exception e)
                {
                    MessageBox.Show("Unable to load model data." + e.Message);
                    toolStripStatus.Text = null;
                    guess = null;
                    return false;
                }
                toolStripStatus.Text = "Model Data is loaded.";
                Refresh();
            }
            return true;
        }

        bool SaveModel()
        {
            if (guess == null) return false;
            String model_data = Path.ChangeExtension(model_file_name_wo_suffix, guess.ModelFileSuffix);
            toolStripStatus.Text = "Saving Model Data.";
            Refresh();
            try
            {
                guess.SaveModel(model_data);
            }
            catch (Exception e)
            {
                MessageBox.Show("Unable to save model data: " + model_data + "\n" + e.Message);
                toolStripStatus.Text = null;
                return false;
            }
            toolStripStatus.Text = "Model Data is saved.";
            return true;
        }

		// TOOD: xml spreadsheet での出力
        void ExportSummary()
        {
			SaveFileDialog dialog = new SaveFileDialog()
            {
                Filter = "CSV File|*.csv",
            };
            if (dialog.ShowDialog() == DialogResult.Cancel) return;

            StreamWriter fs = null;
            try
            {
                fs = new FileInfo(dialog.FileName).CreateText();
                foreach (DataGridViewRow row in dataGridViewImages.SelectedRows)
                {
                    DataRowView row_view = row.DataBoundItem as DataRowView;
                    if (row_view == null) continue;
                    SignatureCounterDataSet.ImageFileRow row_image = row_view.Row as SignatureCounterDataSet.ImageFileRow;
                    if (row_image == null) continue;

                    var query_image_detail =
                        from matching in tables.MatchingTableAdapter.GetData()
                        join signature in tables.SignatureTableAdapter.GetData() on matching.SignatureID equals signature.ID
                        join signatory in tables.SignatoryTableAdapter.GetData() on matching.SignatoryID equals signatory.ID
                        where !signature.IsConclusiveMatchingIDNull() && signature.ConclusiveMatchingID == matching.ID
                        select new { Signature = signature, Matching = matching, Signatory = signatory };
                    foreach (var matchings in query_image_detail)
                    {
                        //TODO: ,のエスケープ
						//TODO: UTF-8を出力するとExcelで開けない
                        fs.WriteLine(String.Format("{0},{1},{2}", row_image.FullPath, matchings.Signatory.ID, matchings.Signatory.Name));
                    }
                }
            }
            catch (UnauthorizedAccessException e)
            {
                MessageBox.Show(String.Format("Unable to open file:\n{0}", e.Message));
            }
            catch (DirectoryNotFoundException e)
            {
                MessageBox.Show(String.Format("Directory name is wrong:\n{0}", e.Message));
            }
            catch (IOException e)
            {
                MessageBox.Show(String.Format("IO error:\n{0}", e.Message));
            }
            catch (InvalidOperationException e)
            {
                MessageBox.Show(String.Format("Operation error:\n{0}", e.Message));
            }
            catch (Exception e)
            {
                MessageBox.Show("Exporting file is failed:\n" + e.Message);
            }
            finally
            {
				if (fs!=null) fs.Close();
            }
        }
        #endregion
    }
}