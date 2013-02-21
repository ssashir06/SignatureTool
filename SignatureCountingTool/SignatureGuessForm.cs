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
            public String ComboBoxText { get { return String.Format("{0}: {1}", Rank != null ? Rank.Value.ToString() : "*", Name); } }
            public int SignatoryID { get; set; }
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
                SelectedAssessment = null;
                SignatureRow = signature;
                ImageFileRow = image;
                TrimRow = trimming;
            }

            [Browsable(false)]
            public SignatureCounterDataSet.SignatureRow SignatureRow { get; protected set; }
            [Browsable(false)]
            public SignatureCounterDataSet.ImageFileRow ImageFileRow { get; protected set; }
            [Browsable(false)]
            public SignatureCounterDataSet.TrimRow TrimRow { get; protected set; }
            [Browsable(false)]
            public List<AssessmentCapsule> Assessments { get; set; }
            [Browsable(false)]
            public int? SelectedAssessment { get; set; }

			[DisplayName("Signature Preview")]
            public System.Drawing.Image SignatureImage { get; protected set; }
			[DisplayName("Use as a Trainer")]
            public Boolean ApplyToTrailer { get; set; }
        }

        #region Variables
        IGuessSignature guess;
        TableAdapterManager tables;
        SignatureCounterDataSet dataset;
        List<SignatureCounterDataSet.SignatureRow> target_signature;
        #endregion

        public SignaturesGuessForm(IGuessSignature guess, TableAdapterManager table_adapter, SignatureCounterDataSet dataset, List<SignatureCounterDataSet.SignatureRow> target)
        {
            InitializeComponent();

            this.guess = guess;
            this.tables = table_adapter;
            this.dataset = dataset;
            this.target_signature = target;
        }

        private void SignaturesGuessForm_Shown(object sender, EventArgs e)
        {
            ShowGuessedList();
        }

        private void additionalTrainingToolStripMenuItem_Click(object sender, EventArgs e)
        {
            StartTraining();
        }

        #region Functions
        void ShowGuessedList()
        {
            List<GuessRowForDataGridView> matches = new List<GuessRowForDataGridView>();

            toolStripStatus.Text = "Matching....";
            Refresh();

            List<int> query_all_signatory_ids = (from signatory in tables.SignatoryTableAdapter.GetData() select signatory.ID).ToList();

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

                GuessRowForDataGridView row = new GuessRowForDataGridView(signature, query_trimming.First(), query_image.First());

                List<int> suggested_signatory_ids = new List<int>();
                List<Assessment> assessments = guess.Match(new SamplingImage(row.ImageFileRow.FullPath, (Rectangle)row.TrimRow));
				SignatureCounterDataSet.SignatoryDataTable signatories = tables.SignatoryTableAdapter.GetData();

                int ranking = 0;
                foreach (Assessment assessment in assessments)
                {
                    var query_signatory =
                        from signatory in signatories
                        where signatory.ID.ToString() == assessment.Name // signatory.ID.ToString()を識別ラベルとして使用する
                        select signatory;
                    if (!query_signatory.Any()) continue;

                    row.Assessments.Add(new AssessmentCapsule()
                    {
                        Rank = ranking++,
                        Name = query_signatory.First().Name,
                        SignatoryID = query_signatory.First().ID,
                    });
                    suggested_signatory_ids.Add(query_signatory.First().ID);
                }

                IEnumerable<int> missing_signatory_ids = query_all_signatory_ids.Except(suggested_signatory_ids);
                foreach (int missing_signatory_id in missing_signatory_ids)
                {
                    var query_signatory = from signatory in signatories where signatory.ID == missing_signatory_id select signatory;
                    if (!query_signatory.Any()) continue;// not expected

                    row.Assessments.Add(new AssessmentCapsule()
                        {
                            Rank = null,
                            Name = query_signatory.First().Name,
                            SignatoryID = query_signatory.First().ID,
                        });
                }

                matches.Add(row);
            }
            dataGridViewSignatures.DataSource = matches;

            dataGridViewSignatures.Columns.Add(new DataGridViewComboBoxColumn()
            {
                Name = "Signatory",
                ValueMember = "SignatoryID",
                DisplayMember = "ComboBoxText"
            });
            foreach (DataGridViewRow row in dataGridViewSignatures.Rows)
            {
                GuessRowForDataGridView row_guessed = row.DataBoundItem as GuessRowForDataGridView;
                DataGridViewComboBoxCell combo_box = row.Cells["Signatory"] as DataGridViewComboBoxCell;
                if (row_guessed == null || combo_box == null) continue;

                combo_box.DataSource = row_guessed.Assessments;
                combo_box.ValueMember = "SignatoryID";
                combo_box.DisplayMember = "ComboBoxText";

				if (row_guessed.Assessments.Any())
                    combo_box.Value = row_guessed.Assessments.First().SignatoryID;
            }

            toolStripStatus.Text = "Matching jobs are finished.";
        }

		void StartTraining()
        {
            Validate();

            List<Conclusive> conclusives = new List<Conclusive>();
            List<SignatureCounterDataSet.SignatureRow> signature_new_trainers = new List<SignatureCounterDataSet.SignatureRow>();

            foreach (DataGridViewRow row in dataGridViewSignatures.Rows)
            {
                GuessRowForDataGridView row_guessed = row.DataBoundItem as GuessRowForDataGridView;
                DataGridViewComboBoxCell combo_box = row.Cells["Signatory"] as DataGridViewComboBoxCell;
                DataGridViewCell use_as_trainer = row.Cells["ApplyToTrailer"];
                if (row_guessed == null || combo_box == null) continue;

                if (use_as_trainer.Value as bool? == true && !row_guessed.SignatureRow.Trainer)
                {
                    conclusives.Add(new Conclusive(row_guessed.ImageFileRow.FullPath, (Rectangle)row_guessed.TrimRow, combo_box.Value.ToString()));// signatory.ID.ToString()を識別ラベルとして使用する
                    signature_new_trainers.Add(row_guessed.SignatureRow);
                }
            }

            if (!conclusives.Any()) return;

            toolStripStatus.Text = "Training is started.";
            Refresh();
            guess.Train(conclusives, true);
            toolStripStatus.Text = "Training is finished.";

            foreach (SignatureCounterDataSet.SignatureRow signature in signature_new_trainers)
            {
                signature.Trainer = true;
                tables.SignatureTableAdapter.Update(signature);
            }
        }
        #endregion
    }
}
