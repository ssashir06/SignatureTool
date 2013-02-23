using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Runtime.InteropServices;
using Signature.CountingTool.Database;
using Signature.CountingTool.Database.SignatureCounterDataSetTableAdapters;
using Signature.Guess.CLI;

namespace Signature.CountingTool
{
    public partial class SignaturesGuessForm : Form
    {
        class AssessmentCapsule
        {
            public int? Rank { get; set; }
            public String Name { get; set; }
            public int SignatoryID { get; set; }
            public int MatchingID { get; set; }
            public String ComboBoxText { get { return String.Format("{0}: {1}", Rank != null ? Rank.Value.ToString() : "*", Name); } }
            public int ComboBoxValue { get { return MatchingID; } set { MatchingID = value; } }
        }
        class GuessRowForDataGridView
        {
            static int max_width = 200, max_height = 50;
            public GuessRowForDataGridView(SignatureCounterDataSet.SignatureRow signature, SignatureCounterDataSet.TrimRow trimming, SignatureCounterDataSet.ImageFileRow image)
            {
                try
                {
                    System.Drawing.Image original_image = System.Drawing.Image.FromFile(image.FullPath);
                    double zoom = new double[] { max_width / trimming.w, max_height / trimming.h }.Min();
                    Bitmap trimmed = new Bitmap((int)(trimming.w * zoom), (int)(trimming.h * zoom));
                    Graphics g = Graphics.FromImage(trimmed);
                    g.DrawImage(original_image, new Rectangle(new Point(0, 0), trimmed.Size), (Rectangle)trimming, GraphicsUnit.Pixel);
                    SignatureImage = trimmed;
                }
                catch (Exception e)
                {
                    //TODO
                }

                ApplyToTrailer = false;
                Assessments = new List<AssessmentCapsule>();
                SignatureID = signature.ID;
                ImageFileID = image.ID;
                TrimID = trimming.ID;
            }

            [Browsable(false)]
            public int SignatureID { get; protected set; }
            [Browsable(false)]
            public int ImageFileID { get; protected set; }
            [Browsable(false)]
            public int TrimID { get; protected set; }
            [Browsable(false)]
            public List<AssessmentCapsule> Assessments { get; set; }

			[DisplayName("Signature Preview")]
            public System.Drawing.Image SignatureImage { get; protected set; }
			[DisplayName("Use as a Trainer")]
            public Boolean ApplyToTrailer { get; set; }
			[DisplayName("Determine")]
            public Boolean Determine { get; set; }
        }

        #region Variables
        IGuessSignature guess;
        TableAdapterManager tables;
        List<SignatureCounterDataSet.SignatureRow> target_signature;
        #endregion

        public SignaturesGuessForm(IGuessSignature guess, TableAdapterManager table_adapter, List<SignatureCounterDataSet.SignatureRow> target)
        {
            InitializeComponent();

            this.guess = guess;
            this.tables = table_adapter;
            this.target_signature = target;
        }

        #region Events
        private void SignaturesGuessForm_Shown(object sender, EventArgs e)
        {
            toolStripStatus.Text = "Matching is not started yet.";
            ShowGuessedList();
        }

        private void startMatchingToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Match();
            ShowGuessedList();
        }

        private void dataGridViewSignatures_CellValueChanged(object sender, DataGridViewCellEventArgs e)
        {
            DataGridViewColumn column = dataGridViewSignatures.Columns[e.ColumnIndex];
            DataGridViewRow row = dataGridViewSignatures.Rows[e.RowIndex];
            DataGridViewCell cell = row.Cells[e.ColumnIndex];
            if (column.Name == "Signatory")
            {
                row.Cells["Determine"].Value = true;
                row.Cells["ApplyToTrailer"].Value = true;
            }
        }

        private void quitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void SignaturesGuessForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (e.CloseReason != CloseReason.UserClosing) return;

            SetDeterminedMatchingID();

			List<Conclusive> conclusives;
			List<int> signature_new_trainers;
			int deneied;
			GetTrainTargets(out conclusives, out signature_new_trainers, out deneied);

            if (conclusives.Any())
            {
                switch (MessageBox.Show("Start training: " + conclusives.Count().ToString() + " signature(s)\n", Text, MessageBoxButtons.YesNoCancel))
                {
                    case DialogResult.Yes:
                        if (!Train(conclusives)) return;
						var query_signature =
							(from signature in tables.SignatureTableAdapter.GetData()
                             join signiture_id in signature_new_trainers on signature.ID equals signiture_id
                             select signature).ToList();
                        foreach (SignatureCounterDataSet.SignatureRow row_signature in query_signature)
                        {
                            row_signature.Trainer = true;
                        }
                        tables.SignatureTableAdapter.Update(query_signature.ToArray());
						
                        break;
                    case DialogResult.Cancel:
                        return;
                    case DialogResult.No:
                        break;
                }
            }
        }
        #endregion

        #region Functions
		void Match()
        {
            toolStripStatus.Text = "Matching....";
            Refresh();

            foreach (var row_signature in target_signature) CleanMatchingRows(row_signature);

            List<int> all_signatory_ids = (
                from signatory in tables.SignatoryTableAdapter.GetData()
                select signatory.ID).ToList();

            var query_matching_ids = from matching in tables.MatchingTableAdapter.GetData() select matching.ID;
            int matching_id = query_matching_ids.Any() ? query_matching_ids.Max() + 1 : 0;

            foreach (SignatureCounterDataSet.SignatureRow signature in target_signature)
            {
                var query_trimming =
                    from trimming in tables.TrimTableAdapter.GetData()
                    where signature.TrimID == trimming.ID
                    select trimming;
                var query_image =
                    from image in tables.ImageFileTableAdapter.GetData()
                    where signature.ImageFileID == image.ID
                    select image;

                if (!query_image.Any() || !query_trimming.Any()) continue;
                SignatureCounterDataSet.ImageFileRow row_image = query_image.First();
                SignatureCounterDataSet.TrimRow row_trim = query_trimming.First();

                List<int> suggested_signatory_ids = new List<int>();
                List<Assessment> assessments = guess.Match(new SamplingImage(row_image.FullPath, (Rectangle)row_trim));
				SignatureCounterDataSet.SignatoryDataTable signatories = tables.SignatoryTableAdapter.GetData();

                int ranking = 0;
                List<int> added_signatory_ids = new List<int>();
                foreach (Assessment assessment in assessments)
                {
					int signatory_id;
                    if (!int.TryParse(assessment.Name, out signatory_id)) continue;// signatory.ID.ToString()を識別ラベルとして使用する

                    tables.MatchingTableAdapter.Insert(matching_id++, signatory_id, signature.ID, ranking++);
                    added_signatory_ids.Add(signatory_id);
                }

                foreach (int not_added_signatory_id in all_signatory_ids.Except(added_signatory_ids))
                {
                    tables.MatchingTableAdapter.Insert(matching_id++, not_added_signatory_id, signature.ID, null);
                }
            }

            toolStripStatus.Text = "Matching jobs are finished.";
        }

        void ShowGuessedList()
        {
            List<GuessRowForDataGridView> matches = new List<GuessRowForDataGridView>();
            foreach (SignatureCounterDataSet.SignatureRow signature in target_signature)
            {
                var query_signature_detail =
                    from image in tables.ImageFileTableAdapter.GetData()
                    join trimming in tables.TrimTableAdapter.GetData() on signature.TrimID equals trimming.ID
                    where image.ID == signature.ImageFileID
                    select new { Signature = signature, Trimming = trimming, Image = image };
                if (!query_signature_detail.Any()) continue;
                var signature_detail = query_signature_detail.First();

                GuessRowForDataGridView row = new GuessRowForDataGridView(signature_detail.Signature, signature_detail.Trimming, signature_detail.Image);

                var query_matching =
                    from matching in tables.MatchingTableAdapter.GetData()
                    join signatory in tables.SignatoryTableAdapter.GetData() on matching.SignatoryID equals signatory.ID
                    where matching.SignatureID == signature.ID
                    orderby matching.IsRankNull() ? int.MaxValue : matching.Rank
                    select new { Matching = matching, Signatory = signatory };
                foreach (var details in query_matching)
                {
                    row.Assessments.Add(new AssessmentCapsule()
                    {
                        Name = details.Signatory.Name,
                        Rank = details.Matching.IsRankNull() ? null : (Nullable<int>)details.Matching.Rank,
                        SignatoryID = details.Signatory.ID,
						MatchingID = details.Matching.ID,
                    });
                }
                matches.Add(row);
			}
            dataGridViewSignatures.DataSource = matches;

            const String guess_select_column = "Signatory";
			if (dataGridViewSignatures.Columns[guess_select_column] == null)
                dataGridViewSignatures.Columns.Add(new DataGridViewComboBoxColumn()
                {
                    Name = guess_select_column,
                    ValueMember = "ComboBoxValue",
                    DisplayMember = "ComboBoxText",
                });
            foreach (DataGridViewRow row in dataGridViewSignatures.Rows)
            {
                GuessRowForDataGridView row_guessed = row.DataBoundItem as GuessRowForDataGridView;
                DataGridViewComboBoxCell combo_box = row.Cells[guess_select_column] as DataGridViewComboBoxCell;
                if (row_guessed == null || combo_box == null) continue;

                combo_box.DataSource = row_guessed.Assessments;
                combo_box.ValueMember = "ComboBoxValue";
                combo_box.DisplayMember = "ComboBoxText";

                if (row_guessed.Assessments.Any())
                {
                    var query_signature = from signature in tables.SignatureTableAdapter.GetData() where signature.ID == row_guessed.SignatureID select signature;
                    if (query_signature.Any() && !query_signature.First().IsConclusiveMatchingIDNull())
                        combo_box.Value = query_signature.First().ConclusiveMatchingID;
                    else
                        combo_box.Value = row_guessed.Assessments.First().ComboBoxValue;
                }

                foreach (DataGridViewCell cell in row.Cells)
                {
                    String column_name = dataGridViewSignatures.Columns[cell.ColumnIndex].Name;
                    if (column_name == "ApplyToTrailer" || column_name == "Determine")
                        cell.Value = false;
                }
            }
        }

        void CleanMatchingRows(SignatureCounterDataSet.SignatureRow row_signature)
        {
            var query_matching =
                from matching in tables.MatchingTableAdapter.GetData()
                where matching.SignatureID == row_signature.ID
                select matching;

            foreach (var matching in query_matching)
                tables.MatchingTableAdapter.Delete(matching.ID);
        }

		void GetTrainTargets(out List<Conclusive> conclusives, out List<int> signature_new_trainers, out int denied_count)
        {
            Validate();

            conclusives = new List<Conclusive>();
            signature_new_trainers = new List<int>();
            denied_count = 0;

            foreach (DataGridViewRow row in dataGridViewSignatures.Rows)
            {
                GuessRowForDataGridView row_guessed = row.DataBoundItem as GuessRowForDataGridView;
                DataGridViewComboBoxCell combo_box = row.Cells["Signatory"] as DataGridViewComboBoxCell;
                DataGridViewCell use_as_trainer = row.Cells["ApplyToTrailer"];
                DataGridViewCell determine = row.Cells["Determine"];
                if (row_guessed == null || combo_box == null) continue;

                // signatory.IDを識別ラベルとして使用する
				int? matching_id = combo_box.Value as int?;
                var query_matchings = from matching in tables.MatchingTableAdapter.GetData() where matching.ID == matching_id select matching;
                if (!query_matchings.Any()) continue;
                SignatureCounterDataSet.MatchingRow row_matching = query_matchings.First();
                int signatory_id = row_matching.SignatoryID;

                var query_signature_detail =
                    (from signature in tables.SignatureTableAdapter.GetData()
                     join image in tables.ImageFileTableAdapter.GetData() on signature.ImageFileID equals image.ID
                     join trim in tables.TrimTableAdapter.GetData() on signature.TrimID equals trim.ID
                     where signature.ID == row_guessed.SignatureID
                     select new { Signature = signature, Image = image, Trimming = trim }).ToList();

                if (use_as_trainer.Value as bool? != true)
                {
                    continue;
                }
                else if (determine.Value as bool? == true && query_signature_detail.Any() && !query_signature_detail.First().Signature.Trainer)
                {
                    conclusives.Add(new Conclusive(query_signature_detail.First().Image.FullPath, (Rectangle)query_signature_detail.First().Trimming, signatory_id.ToString()));
                    signature_new_trainers.Add(query_signature_detail.First().Signature.ID);
                }
                else
                {
                    denied_count++;
                }
            }
        }

		bool Train(List<Conclusive> conclusives)
        {
            try
            {
                toolStripStatus.Text = "Training is started.";
                Refresh();
                guess.Train(conclusives, true);
                toolStripStatus.Text = "Training is finished.";
            }
            catch (SEHException exp)
            {
                MessageBox.Show("Failed to train: \n" + exp.Message);
                toolStripStatus.Text = "Training was failed.";
                return false;
            }
            return true;
        }

		void SetDeterminedMatchingID()
        {
            Validate();

            foreach (DataGridViewRow row in dataGridViewSignatures.Rows)
            {
                GuessRowForDataGridView row_guessed = row.DataBoundItem as GuessRowForDataGridView;
                DataGridViewComboBoxCell combo_box = row.Cells["Signatory"] as DataGridViewComboBoxCell;
                DataGridViewCell determine = row.Cells["Determine"];
                if (row_guessed == null || combo_box == null) continue;

                if (determine.Value as bool? == true && combo_box.Value as int? != null)
                {
                    var query_signature =
                        (from signature in tables.SignatureTableAdapter.GetData()
                         where signature.ID == row_guessed.SignatureID
                         select signature).ToList();
                    if (!query_signature.Any()) continue;
                    SignatureCounterDataSet.SignatureRow row_signature = query_signature.First();

                    row_signature.ConclusiveMatchingID = (combo_box.Value as int?).Value;
                    tables.SignatureTableAdapter.Update(row_signature);
                }
            }

            ShowGuessedList();
        }
        #endregion
    }
}
