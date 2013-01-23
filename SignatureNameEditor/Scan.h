#pragma once
#include <string>
#include <vcclr.h>
#include "SignatureImage.h"
#include "../LibCameraScanner/CameraScanner.h"
#include "cv_utils_cli.h"

namespace SignatureNameEditor {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Xml::Serialization;
	using namespace System::IO;
	using namespace CVUtil;

	/// <summary>
	/// Scan の概要
	/// </summary>
	public ref class Scan : public System::Windows::Forms::Form
	{

	public:
		Scan()
		{
			InitializeComponent();
			init();
		}

	protected:
		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		~Scan()
		{
			quit();
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  pictureBoxPreview;
	private: System::Windows::Forms::PictureBox^  pictureBoxTrimming;
	private: System::Windows::Forms::Button^  buttonScan;




	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::StatusStrip^  statusStrip1;
	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatus;
	private: System::Windows::Forms::Timer^  timerTakenWait;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::CheckedListBox^  checkedListSignatures;
	private: System::Windows::Forms::Button^  buttonUse;
	private: System::Windows::Forms::Button^  buttonRescan;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripMenuItem^  quitToolStripMenuItem;
	private: System::Windows::Forms::CheckedListBox^  checkedListBoxImages;
	private: System::Windows::Forms::Button^  buttonDone;
	private: System::Windows::Forms::GroupBox^  groupBox1;

	private: System::Windows::Forms::Timer^  timerPreviewUpdate;

	private:
		/// <summary>
		/// 必要なデザイナー変数です。
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// デザイナー サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディターで変更しないでください。
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->pictureBoxPreview = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBoxTrimming = (gcnew System::Windows::Forms::PictureBox());
			this->buttonScan = (gcnew System::Windows::Forms::Button());
			this->timerPreviewUpdate = (gcnew System::Windows::Forms::Timer(this->components));
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->toolStripStatus = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->timerTakenWait = (gcnew System::Windows::Forms::Timer(this->components));
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->checkedListSignatures = (gcnew System::Windows::Forms::CheckedListBox());
			this->buttonUse = (gcnew System::Windows::Forms::Button());
			this->buttonRescan = (gcnew System::Windows::Forms::Button());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->quitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->checkedListBoxImages = (gcnew System::Windows::Forms::CheckedListBox());
			this->buttonDone = (gcnew System::Windows::Forms::Button());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxPreview))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxTrimming))->BeginInit();
			this->statusStrip1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->menuStrip1->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// pictureBoxPreview
			// 
			this->pictureBoxPreview->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->pictureBoxPreview->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureBoxPreview->Location = System::Drawing::Point(324, 38);
			this->pictureBoxPreview->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->pictureBoxPreview->Name = L"pictureBoxPreview";
			this->pictureBoxPreview->Size = System::Drawing::Size(434, 517);
			this->pictureBoxPreview->TabIndex = 0;
			this->pictureBoxPreview->TabStop = false;
			// 
			// pictureBoxTrimming
			// 
			this->pictureBoxTrimming->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->pictureBoxTrimming->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureBoxTrimming->Location = System::Drawing::Point(570, 562);
			this->pictureBoxTrimming->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->pictureBoxTrimming->Name = L"pictureBoxTrimming";
			this->pictureBoxTrimming->Size = System::Drawing::Size(292, 70);
			this->pictureBoxTrimming->TabIndex = 1;
			this->pictureBoxTrimming->TabStop = false;
			// 
			// buttonScan
			// 
			this->buttonScan->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->buttonScan->Location = System::Drawing::Point(324, 562);
			this->buttonScan->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->buttonScan->Name = L"buttonScan";
			this->buttonScan->Size = System::Drawing::Size(239, 70);
			this->buttonScan->TabIndex = 2;
			this->buttonScan->Text = L"Scan";
			this->buttonScan->UseVisualStyleBackColor = true;
			this->buttonScan->Click += gcnew System::EventHandler(this, &Scan::buttonScan_Click);
			// 
			// timerPreviewUpdate
			// 
			this->timerPreviewUpdate->Tick += gcnew System::EventHandler(this, &Scan::timerPreviewUpdate_Tick);
			// 
			// statusStrip1
			// 
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->toolStripStatus});
			this->statusStrip1->Location = System::Drawing::Point(0, 650);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Padding = System::Windows::Forms::Padding(1, 0, 16, 0);
			this->statusStrip1->Size = System::Drawing::Size(877, 22);
			this->statusStrip1->TabIndex = 4;
			// 
			// toolStripStatus
			// 
			this->toolStripStatus->Name = L"toolStripStatus";
			this->toolStripStatus->Size = System::Drawing::Size(0, 17);
			// 
			// groupBox2
			// 
			this->groupBox2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->groupBox2->Controls->Add(this->checkedListSignatures);
			this->groupBox2->Location = System::Drawing::Point(765, 38);
			this->groupBox2->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Padding = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->groupBox2->Size = System::Drawing::Size(98, 518);
			this->groupBox2->TabIndex = 5;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"signatures";
			// 
			// checkedListSignatures
			// 
			this->checkedListSignatures->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left));
			this->checkedListSignatures->FormattingEnabled = true;
			this->checkedListSignatures->Location = System::Drawing::Point(7, 22);
			this->checkedListSignatures->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->checkedListSignatures->Name = L"checkedListSignatures";
			this->checkedListSignatures->Size = System::Drawing::Size(83, 472);
			this->checkedListSignatures->TabIndex = 0;
			this->checkedListSignatures->SelectedValueChanged += gcnew System::EventHandler(this, &Scan::checkedListSignatures_SelectedValueChanged);
			// 
			// buttonUse
			// 
			this->buttonUse->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->buttonUse->Location = System::Drawing::Point(324, 562);
			this->buttonUse->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->buttonUse->Name = L"buttonUse";
			this->buttonUse->Size = System::Drawing::Size(106, 70);
			this->buttonUse->TabIndex = 6;
			this->buttonUse->Text = L"Use";
			this->buttonUse->UseVisualStyleBackColor = true;
			this->buttonUse->Visible = false;
			this->buttonUse->Click += gcnew System::EventHandler(this, &Scan::buttonUse_Click);
			// 
			// buttonRescan
			// 
			this->buttonRescan->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->buttonRescan->Location = System::Drawing::Point(457, 562);
			this->buttonRescan->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->buttonRescan->Name = L"buttonRescan";
			this->buttonRescan->Size = System::Drawing::Size(106, 70);
			this->buttonRescan->TabIndex = 7;
			this->buttonRescan->Text = L"Rescan";
			this->buttonRescan->UseVisualStyleBackColor = true;
			this->buttonRescan->Visible = false;
			this->buttonRescan->Click += gcnew System::EventHandler(this, &Scan::buttonRescan_Click);
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->fileToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Padding = System::Windows::Forms::Padding(7, 2, 0, 2);
			this->menuStrip1->Size = System::Drawing::Size(877, 26);
			this->menuStrip1->TabIndex = 8;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->openToolStripMenuItem, 
				this->saveToolStripMenuItem, this->toolStripSeparator1, this->quitToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(40, 22);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->openToolStripMenuItem->Text = L"Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &Scan::openToolStripMenuItem_Click);
			// 
			// saveToolStripMenuItem
			// 
			this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
			this->saveToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->saveToolStripMenuItem->Text = L"Save";
			this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &Scan::saveToolStripMenuItem_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(149, 6);
			// 
			// quitToolStripMenuItem
			// 
			this->quitToolStripMenuItem->Name = L"quitToolStripMenuItem";
			this->quitToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->quitToolStripMenuItem->Text = L"Quit";
			this->quitToolStripMenuItem->Click += gcnew System::EventHandler(this, &Scan::quitToolStripMenuItem_Click);
			// 
			// checkedListBoxImages
			// 
			this->checkedListBoxImages->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left));
			this->checkedListBoxImages->FormattingEnabled = true;
			this->checkedListBoxImages->Location = System::Drawing::Point(7, 22);
			this->checkedListBoxImages->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->checkedListBoxImages->Name = L"checkedListBoxImages";
			this->checkedListBoxImages->Size = System::Drawing::Size(289, 508);
			this->checkedListBoxImages->TabIndex = 0;
			this->checkedListBoxImages->SelectedIndexChanged += gcnew System::EventHandler(this, &Scan::checkedListBoxImages_SelectedIndexChanged);
			this->checkedListBoxImages->Enter += gcnew System::EventHandler(this, &Scan::checkedListBoxImages_Enter);
			// 
			// buttonDone
			// 
			this->buttonDone->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->buttonDone->Location = System::Drawing::Point(7, 556);
			this->buttonDone->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->buttonDone->Name = L"buttonDone";
			this->buttonDone->Size = System::Drawing::Size(289, 32);
			this->buttonDone->TabIndex = 2;
			this->buttonDone->Text = L"Done";
			this->buttonDone->UseVisualStyleBackColor = true;
			// 
			// groupBox1
			// 
			this->groupBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left));
			this->groupBox1->Controls->Add(this->buttonDone);
			this->groupBox1->Controls->Add(this->checkedListBoxImages);
			this->groupBox1->Location = System::Drawing::Point(14, 36);
			this->groupBox1->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Padding = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->groupBox1->Size = System::Drawing::Size(303, 596);
			this->groupBox1->TabIndex = 3;
			this->groupBox1->TabStop = false;
			// 
			// Scan
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 15);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(877, 672);
			this->Controls->Add(this->buttonRescan);
			this->Controls->Add(this->buttonUse);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->menuStrip1);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->buttonScan);
			this->Controls->Add(this->pictureBoxTrimming);
			this->Controls->Add(this->pictureBoxPreview);
			this->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->MainMenuStrip = this->menuStrip1;
			this->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->Name = L"Scan";
			this->Text = L"Scan";
			this->Load += gcnew System::EventHandler(this, &Scan::Scan_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxPreview))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxTrimming))->EndInit();
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->groupBox1->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

#pragma region 変数/型
	private: enum class RunMode { Previewing, Trimming, Checking, None };
	private: RunMode current_mode;

	private: CameraScanner::ScanSpec* paper_spec;
	private: CameraScanner::Reshape* scanner;
	private: cv::VideoCapture* camera;
	private: static int capture_width = 800;//1600;
	private: static int capture_height = 600;//1200;
	private: static int preview_width = 640;
	private: static int preview_height = 480;

	private: SignatureImage^ current_scan_image;
	private: List<Rect>^ trim_area_default;
	private: String^ xml_file_path;
	//private: List<SignatureImage>^ image_list_result;

#pragma endregion
#pragma region 関数

	private: System::Void init()
		 {
			 paper_spec = new CameraScanner::ScanSpec(210, 297, 300);
			 scanner = new CameraScanner::Reshape(*paper_spec);
			 camera = new cv::VideoCapture();
			 trim_area_default = gcnew List<Rect>();
			 trim_area_default->Add(Rect(2000, 30, 475, 160, true));
			 trim_area_default->Add(Rect(2000, 170, 475, 160, true));
			 //this->image_list_result = image_list_result;
			 current_mode = RunMode::Trimming;
		 }
	private: System::Void quit()
		 {

			 switch (current_mode) {
			 case RunMode::Checking:
				 addSignature();
				 saveSignature();
				 break;
			 case RunMode::Trimming:
				 saveSignature();
				 break;
			 }

			 //for each (SignatureImage^ sign in checkedListSignatures->Items) {
				// image_list_result->Add(sign);
			 //}

			 if (paper_spec) delete paper_spec;
			 if (scanner) delete scanner;
			 if (camera) delete camera;
		 }
	private: System::Void setMessage(String^ msg)
		 {
			 toolStripStatus->Text = msg;
		 }
	private: System::Void setMessage(const wchar_t* const msg)
		 {
			 setMessage(gcnew String(msg));
		 }
#pragma region モード切替/ボタン表示・非表示
	private: System::Void startPreview()
		 {
			 buttonScan->Text = L"Scan";
			 timerPreviewUpdate->Enabled = true;
			 buttonScan->Visible = true;
			 buttonUse->Visible = false;
			 buttonRescan->Visible = false;
			 checkedListSignatures->Items->Clear();
			 current_mode = RunMode::Previewing;
		 }
	private: System::Void stopPreviewing()
		 {
			 buttonScan->Text = L"Start previewing";
			 timerPreviewUpdate->Enabled = false;
			 current_mode = RunMode::None;
		 }
	private: System::Void startChecking()
		 {
			 buttonScan->Visible = false;
			 buttonUse->Visible = true;
			 buttonRescan->Visible = true;
			 checkedListSignatures->Items->Clear();
			 current_mode = RunMode::Checking;
		 }
	private: System::Void stopChecking()
		 {
			 current_mode = RunMode::None;
		 }
	private: System::Void startTrimming()
		 {
			 buttonScan->Visible = true;
			 buttonUse->Visible = false;
			 buttonRescan->Visible = false;
			 buttonScan->Text = L"Start previewing";
			 checkedListSignatures->Items->Clear();
			 current_mode = RunMode::Trimming;
		 }
	private: System::Void stopTrimming()
		 {
			 pictureBoxTrimming->Image = nullptr;
			 saveSignature();
			 current_mode = RunMode::None;
		 }
#pragma endregion
#pragma region スキャナ
	private: bool connect()
		 {
			 if (!camera->isOpened()) {
				 if (!camera->open(0)) {
					 setMessage(L"Connecting to camera was failed");
					 return false;
				 }

				 camera->set(CV_CAP_PROP_FPS, 30);
				 setMessage(L"Connecting to camera was succeeded");
			 }
			 return true;
		 }

	private: void disconnect()
		 {
			 if (camera) delete camera;
			 camera = new cv::VideoCapture();
		 }
	private: cv::Mat takeShot(bool as_preview)
		 {
			 if (!connect()) return cv::Mat();

			 static int last_width, last_height;
			 int width, height;
			 if (as_preview) 
				 width = preview_width, height = preview_height;
			 else
				 width = capture_width, height = capture_height;

			 camera->set(CV_CAP_PROP_FRAME_WIDTH, width);
			 camera->set(CV_CAP_PROP_FRAME_HEIGHT, height);

			 int repeat_count;
			 if (last_width != width || last_height != height)
				 repeat_count = 10;
			 else 
				 repeat_count = 1;

			 cv::Mat img_cap;
			 for (int i=0; i<repeat_count; i++) (*camera) >> img_cap;
			 if (img_cap.empty()) throw L"camera is disconnected";

			 cv::Mat img_trim = img_cap.clone();//FIXME bug? slow
			 CameraScanner::trimBorder(img_trim);//bugfix for cheap camera
			 return img_trim;
		 }
	private: System::Void showPreview(bool show_lines)
		 {
			 if (!connect()) return;
			 cv::Mat img;
			 try {
				 img = takeShot(true);
			 } catch (const wchar_t* const msg) {
				 setMessage(msg);
				 disconnect();
				 return;
			 }

			 if (show_lines) {
				 try {
					 scanner->prepare(img);
					 img = scanner->drawSideLines();
					 setMessage(L"The edges of paper are detected.");
				 } catch (const wchar_t* const msg) {
					 setMessage(msg);
				 }
			 } else {
				 try {
					 scanner->drawSideLines(img);
				 } catch (const wchar_t* const msg) {
					 setMessage(msg);
				 }
			 }
			 CLI::Control::showImage(img, pictureBoxPreview, true);
		 }
	private: template<typename T>
		 List<T>^ deepCopyList(List<T>^ src)
		 {
			 List<T>^ dst = gcnew List<T>(src->Count);
			 for each (T^ item in src)
				 dst->Add(T(item));
			 return dst;
		 }
	private: SignatureImage^ scan()
		 {
			 if (!connect()) 
				 throw(L"Camera is not connected");
			 setMessage(L"Starting scan");

			 cv::Mat img;
			 try {
				 img = takeShot(false);
			 } catch (const wchar_t* const msg) {
				 setMessage(msg);
				 disconnect();
				 throw msg;
			 }

			 cv::Mat paper;
			 scanner->prepare(img);
			 paper = scanner->scan();

			 if (paper.empty())
				 throw L"Paper scanning was failed";

			 setMessage(L"Paper scanning was finished");

			 SignatureImage^ scan = gcnew SignatureImage();
			 scan->Image = new cv::Mat(paper);
			 scan->TrimmingAreas = deepCopyList(trim_area_default);

			 return scan;
		 }
#pragma endregion
#pragma region トリミング
	 private: cv::Mat getCurrentScanImage()
		 {
			 if (!current_scan_image) throw L"current scan image is null";

			 if (current_scan_image->Image) {
				 cv::Mat img = *current_scan_image->Image;
				 if (img.empty()) 
					 setMessage(L"Current image is empty!");
				 return img;
			 } else {
				 String^ file_name_full = Path::Combine(xml_file_path ? xml_file_path : Directory::GetCurrentDirectory(), current_scan_image->FileName);
				 const std::string file_name = CVUtil::CLI::convertFileName(file_name_full);
				 cv::Mat img = cv::imread(file_name);
				 if (img.empty())
					 setMessage(String::Format(L"Unable to load image: {0}", file_name_full));
				 else
					 setMessage(String::Format(L"Image is loaded: {0}", file_name_full));
				 return img;
			 }
		 }
	private: System::Void showSignature()
		 {
			 if (!current_scan_image)
				 return;

			 cv::Mat img = getCurrentScanImage();
			 if (!img.empty())
				 CLI::Control::showImage(img, pictureBoxPreview, true);

			 checkedListSignatures->BeginUpdate();
			 for each (Rect^ rect in current_scan_image->TrimmingAreas) {
				 checkedListSignatures->Items->Add(rect, rect->used);
			 }
			 checkedListSignatures->EndUpdate();
		 }
	private: System::Void showTrimmingArea()
		 {
			 if (!current_scan_image)
				 return;
			 if (!checkedListSignatures->SelectedItem)
				 return;

			 cv::Mat img = getCurrentScanImage();
			 if (img.empty()) {
				 setMessage(L"Cannot trim empty image.");
				 return;
			 }

			 Rect^ sig_rect = (Rect^)checkedListSignatures->SelectedItem;
			 cv::Rect cv_rect(sig_rect->x, sig_rect->y, sig_rect->width, sig_rect->height);
			 CLI::Control::showImage(cv::Mat(img, cv_rect), pictureBoxTrimming, true);
		 }
	private: System::Void addSignature()
		 {
			 checkedListBoxImages->Items->Add(current_scan_image, true);
		 }
			 //配列に保存します
	private: System::Void saveSignature()
		 {
			 if (!current_scan_image)
				 return;
			 for (int i=0; i<checkedListSignatures->Items->Count; i++) {
				 Rect^ r = %current_scan_image->TrimmingAreas[i];
				 r->used = checkedListSignatures->GetItemChecked(i);
				 current_scan_image->TrimmingAreas[i] = *r;
				 //current_scan_image->TrimmingAreas[i].used = checkedListSignatures->GetItemChecked(i);//FIXME
			 }
		 }
	private: System::Void selectSignature()
		 {
			 int idx = checkedListBoxImages->SelectedIndex;
			 if (idx == -1)
				 current_scan_image = nullptr;
			 else
				 current_scan_image = (SignatureImage^)checkedListBoxImages->Items[idx];
		 }
#pragma endregion
#pragma region Save/Load
	private: System::Void showSaveDialog()
		 {
			 SaveFileDialog^ dialog = gcnew SaveFileDialog();
			 dialog->Filter = L"Scanned Image info(*.xml)|*.xml";
			 if (dialog->ShowDialog() == System::Windows::Forms::DialogResult::Cancel) return;

			 try {
				 saveImages(dialog->FileName);
			 } catch (String^ msg) {
				 setMessage(L"Failed to save file: " + msg);
			 }
		 }
	private: System::Void showLoadDialog()
		 {
			 OpenFileDialog^ dialog = gcnew OpenFileDialog();
			 dialog->Filter = L"Scanned Image info(*.xml)|*.xml";
			 if (dialog->ShowDialog() == System::Windows::Forms::DialogResult::Cancel) return;

			 try {
				 loadImages(dialog->FileName);
			 } catch (String^ msg) {
				 setMessage(L"Failed to load file: " + msg);
			 }
		 }
	// see http://msdn.microsoft.com/ja-jp/library/system.xml.serialization.xmlattributes.xmlignore(v=vs.100).aspx
	//     http://msdn.microsoft.com/ja-jp/library/71s92ee1(v=vs.100).aspx
			 //XMLに保存します
	private: System::Void saveImages(String^ xml_file_name)
		 {
			 xml_file_path = Path::GetDirectoryName(xml_file_name);
			 List<SignatureImage^ >^ items = gcnew List<SignatureImage^ >();
			 for each (Object^ item in checkedListBoxImages->Items) {
				 SignatureImage^ image = (SignatureImage^)item;
				 try {
					 image->saveImage(xml_file_path, nullptr);
				 } catch (UnauthorizedAccessException^ e) {
					 throw e;
				 } catch (PathTooLongException^ e) {
					 throw e;
				 } catch (DirectoryNotFoundException^ e) {
					 throw e;
				 }

				 items->Add(image);
			 }
			 XmlSerializer^ xs = gcnew XmlSerializer(List<SignatureImage^ >::typeid);
			 // TODO: try catch
			 TextWriter^ writer = gcnew StreamWriter(xml_file_name);
			 xs->Serialize(writer, items);
			 writer->Close();
		 }
	private: System::Void loadImages(String^ xml_file_name)
		 {
			 XmlSerializer^ xs = gcnew XmlSerializer(List<SignatureImage^ >::typeid);
			 // TODO: try catch
			 TextReader^ reader = gcnew StreamReader(xml_file_name);
			 List<SignatureImage^ >^ items = dynamic_cast<List<SignatureImage^ >^ >(xs->Deserialize(reader));
			 reader->Close();

			 checkedListBoxImages->Items->Clear();
			 for each (SignatureImage^ image in items) {
				 checkedListBoxImages->Items->Add(image, true);
			 }
			 xml_file_path = Path::GetDirectoryName(xml_file_name);
		 }
#pragma endregion
#pragma endregion
		 // Events of controls
	private: System::Void buttonScan_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 switch (current_mode) {
			 case RunMode::Trimming:
				 stopTrimming();
				 startPreview();
				 break;
			 case RunMode::Previewing:
				 current_scan_image = scan();
				 stopPreviewing();
				 startChecking();
				 showSignature();
				 break;
			 case RunMode::None:
				 startPreview();
				 break;
			 }
		 }
	private: System::Void buttonUse_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 saveSignature();
			 addSignature();
			 stopChecking();
			 startPreview();
		 }
	private: System::Void buttonRescan_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 stopChecking();
			 startPreview();
		 }
	private: System::Void checkedListBoxImages_Enter(System::Object^  sender, System::EventArgs^  e)
		 {
			 switch (current_mode) {
			 case RunMode::Previewing:
				 stopPreviewing();
				 startTrimming();
				 break;
			 case RunMode::Checking:
				 //stopChecking();
				 //startTrimming();
				 break;
			 case RunMode::None:
				 startTrimming();
				 break;
			 }
		 }
	private: System::Void checkedListBoxImages_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 switch (current_mode) {
			 case RunMode::Previewing:
				 stopPreviewing();
				 startTrimming();
				 break;
			 case RunMode::Checking:
				 stopChecking();
				 startTrimming();
				 break;
			 case RunMode::Trimming:
				 stopTrimming();
				 startTrimming();
				 selectSignature();
				 showSignature();
				 break;
			 case RunMode::None:
				 startTrimming();
				 break;
			 }
		 }
	private: System::Void checkedListSignatures_SelectedValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 switch (current_mode) {
			 case RunMode::Trimming:
				 showTrimmingArea();
				 break;
			 }
		 }
	private: System::Void timerPreviewUpdate_Tick(System::Object^  sender, System::EventArgs^  e)
		 {
			 static int frame;
			 if (frame==0) showPreview(true);
			 else showPreview(false);
			 frame = (frame+1)%5;
		 }
	private: System::Void Scan_Load(System::Object^  sender, System::EventArgs^  e)
		 {
			 timerPreviewUpdate->Interval = 100;
			 startPreview();
		 }
	private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 showLoadDialog();
		 }
	private: System::Void saveToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 showSaveDialog();
		 }
	private: System::Void quitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 switch (
				 MessageBox::Show(
					L"Scanning window will be now closed. Do you want to save images?", 
					L"Save images before closing?", 
					MessageBoxButtons::YesNoCancel)) {
			 case System::Windows::Forms::DialogResult::Yes: 
				 showSaveDialog();//TODO: Check failure
				 Close();
				 break;
			 case System::Windows::Forms::DialogResult::No:
				 Close();
				 break;
			 case System::Windows::Forms::DialogResult::Cancel:
				 break;
			 default:
				 throw;
				 break;
			 }
		 }
	};
}
