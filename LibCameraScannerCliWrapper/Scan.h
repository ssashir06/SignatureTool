#pragma once
#include <string>
#include <vcclr.h>
#include "ScannedImageCV.h"
#include "../LibCameraScanner/CameraScanner.h"
#include "../cv_utils/cv_utils_cli.h"

namespace CameraScanner { namespace GUI {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Xml::Serialization;
	using namespace System::IO;
	using namespace System::Threading;
	using namespace CVUtil;
	using namespace CameraScanner::CLI;

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
	private:System::Windows::Forms::PictureBox^  pictureBox;
	private:System::Windows::Forms::Button^  buttonScan;
	private:System::ComponentModel::IContainer^  components;
	private:System::Windows::Forms::StatusStrip^  statusStrip1;
	private:System::Windows::Forms::ToolStripStatusLabel^  toolStripStatus;
	private:System::Windows::Forms::Button^  buttonUse;
	private:System::Windows::Forms::Button^  buttonRescan;
	private:System::Windows::Forms::MenuStrip^  menuStrip1;
	private:System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private:System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
	private:System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;
	private:System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private:System::Windows::Forms::ToolStripMenuItem^  quitToolStripMenuItem;
	private:System::Windows::Forms::CheckedListBox^  checkedListBoxImages;
	private:System::Windows::Forms::GroupBox^  groupBox1;
	private:System::Windows::Forms::Button^  buttonStartScan;
	private:System::Windows::Forms::Timer^  timerCapturingUpdate;

	private:
#pragma region Windows Form Designer generated code
		/// <summary>
		/// デザイナー サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディターで変更しないでください。
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->pictureBox = (gcnew System::Windows::Forms::PictureBox());
			this->buttonScan = (gcnew System::Windows::Forms::Button());
			this->timerCapturingUpdate = (gcnew System::Windows::Forms::Timer(this->components));
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->toolStripStatus = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->buttonUse = (gcnew System::Windows::Forms::Button());
			this->buttonRescan = (gcnew System::Windows::Forms::Button());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->quitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->checkedListBoxImages = (gcnew System::Windows::Forms::CheckedListBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->buttonStartScan = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox))->BeginInit();
			this->statusStrip1->SuspendLayout();
			this->menuStrip1->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// pictureBox
			// 
			this->pictureBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->pictureBox->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureBox->Location = System::Drawing::Point(324, 38);
			this->pictureBox->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->pictureBox->Name = L"pictureBox";
			this->pictureBox->Size = System::Drawing::Size(541, 517);
			this->pictureBox->TabIndex = 0;
			this->pictureBox->TabStop = false;
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
			// timerCapturingUpdate
			// 
			this->timerCapturingUpdate->Interval = 200;
			this->timerCapturingUpdate->Tick += gcnew System::EventHandler(this, &Scan::timerCapturingUpdate_Tick);
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
			this->openToolStripMenuItem->Size = System::Drawing::Size(106, 22);
			this->openToolStripMenuItem->Text = L"Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &Scan::openToolStripMenuItem_Click);
			// 
			// saveToolStripMenuItem
			// 
			this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
			this->saveToolStripMenuItem->Size = System::Drawing::Size(106, 22);
			this->saveToolStripMenuItem->Text = L"Save";
			this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &Scan::saveToolStripMenuItem_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(103, 6);
			// 
			// quitToolStripMenuItem
			// 
			this->quitToolStripMenuItem->Name = L"quitToolStripMenuItem";
			this->quitToolStripMenuItem->Size = System::Drawing::Size(106, 22);
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
			this->checkedListBoxImages->Size = System::Drawing::Size(289, 562);
			this->checkedListBoxImages->TabIndex = 0;
			this->checkedListBoxImages->SelectedIndexChanged += gcnew System::EventHandler(this, &Scan::checkedListBoxImages_SelectedIndexChanged);
			// 
			// groupBox1
			// 
			this->groupBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left));
			this->groupBox1->Controls->Add(this->checkedListBoxImages);
			this->groupBox1->Location = System::Drawing::Point(14, 36);
			this->groupBox1->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Padding = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->groupBox1->Size = System::Drawing::Size(303, 596);
			this->groupBox1->TabIndex = 3;
			this->groupBox1->TabStop = false;
			// 
			// buttonStartScan
			// 
			this->buttonStartScan->Location = System::Drawing::Point(325, 562);
			this->buttonStartScan->Name = L"buttonStartScan";
			this->buttonStartScan->Size = System::Drawing::Size(238, 69);
			this->buttonStartScan->TabIndex = 9;
			this->buttonStartScan->Text = L"start scan";
			this->buttonStartScan->UseVisualStyleBackColor = true;
			this->buttonStartScan->Click += gcnew System::EventHandler(this, &Scan::buttonStartScan_Click);
			// 
			// Scan
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 15);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(877, 672);
			this->Controls->Add(this->buttonStartScan);
			this->Controls->Add(this->buttonRescan);
			this->Controls->Add(this->buttonUse);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->menuStrip1);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->buttonScan);
			this->Controls->Add(this->pictureBox);
			this->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->MainMenuStrip = this->menuStrip1;
			this->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->Name = L"Scan";
			this->Text = L"Scan";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Scan::Scan_FormClosing);
			this->Load += gcnew System::EventHandler(this, &Scan::Scan_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox))->EndInit();
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->groupBox1->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

#pragma region 変数/型
		/// <summary>
		/// フォームの動作状態を表す
		/// </summary>
	private:enum class RunMode {
		/// <summary>
		/// カメラから得た像の表示．紙の辺を表示．スキャンボタンを表示．
		/// </summary>
		Capturinging, 
		/// <summary>
		/// スキャン後の画像を表示．採用・不採用を決定するボタンの表示．
		/// </summary>
		Checking, 
		/// <summary>
		/// 採用されたスキャン画像の表示
		/// </summary>
		Reviewing,
		/// <summary>
		/// ウィンドウ上では何もしない
		/// </summary>
		None
	};
	private:RunMode current_mode;

	private:Object^ lock_calcurating_scan_image;
	private:SynchronizationContext^ original_context;

	private:CameraScanner::ScanSpec* paper_spec;
	private:CameraScanner::Reshape* scanner;
	private:cv::VideoCapture* camera;
	private:static int capture_width = 800;//1600;
	private:static int capture_height = 600;//1200;
	private:static int preview_width = 640;
	private:static int preview_height = 480;

	private:IScannedImage^ current_scan_image;
	private:String^ xml_file_path;
	private:String^ xml_file_name;

	//マルチスレッド用
	private:ref class SetMessageData
			{
			public:
				SetMessageData() {}
				SetMessageData(String^ msg, Scan^ scanner) : msg(msg), scanner(scanner) {}
				~SetMessageData() {}
				String^ msg;
				Scan^ scanner;
				Void Write(Object^ obj)
				{
					if (scanner) scanner->toolStripStatus->Text = msg;
				}
			};
	private:ref class ImageUpdateData
			{
			public:
				ImageUpdateData() {}
				ImageUpdateData(cv::Mat& img, PictureBox^ picture_box, PictureBoxSizeMode fit_to_box) : img(CVUtil::CLI::convertImage(img)), picture_box(picture_box), fit_to_box(fit_to_box) {}
				ImageUpdateData(Image^ img, PictureBox^ picture_box, PictureBoxSizeMode fit_to_box) : img(img), picture_box(picture_box), fit_to_box(fit_to_box) {}
				~ImageUpdateData() {}
				Image^ img;
				PictureBox^ picture_box;
				PictureBoxSizeMode fit_to_box;
				Void Draw(Object^ obj)
				{
					if (!picture_box) return;
					picture_box->SizeMode = fit_to_box;
					picture_box->Image = img;
				}
			};
#pragma endregion
#pragma region 関数
	private:Void init()
			{
				paper_spec = new CameraScanner::ScanSpec(210, 297, 300);
				scanner = new CameraScanner::Reshape(*paper_spec);
				camera = new cv::VideoCapture();
				current_mode = RunMode::Capturinging;
				lock_calcurating_scan_image = gcnew Object;
				original_context = SynchronizationContext::Current;
			}
	private:Void quit()
			{
				Monitor::Enter(lock_calcurating_scan_image);// stop timerd scanning mode

				switch (current_mode) {
				case RunMode::Checking:
					addImage();
					//TODO: save dialog
					break;
				}

				if (paper_spec) delete paper_spec;
				if (scanner) delete scanner;
				if (camera) delete camera;
			}
	private:Void setMessage(String^ msg)
			{
				SetMessageData^ messanger = gcnew SetMessageData(msg, this);
				original_context->Post(gcnew SendOrPostCallback(messanger, &SetMessageData::Write), false);
			}
	private:Void setMessage(Exception^ e)
			{
				setMessage(e->Message);
			}
#pragma region モード切替/ボタン表示・非表示
	private:Void startCapturinging()
			{
				buttonScan->Text = L"Scan";
				buttonScan->Visible = true;
				buttonUse->Visible = false;
				buttonRescan->Visible = false;
				buttonStartScan->Visible = false;
				current_mode = RunMode::Capturinging;
				timerCapturingUpdate->Enabled = true;
			}
	private:Void stopCapturinging()
			{
				buttonScan->Text = L"Start previewing";
				current_mode = RunMode::None;
				timerCapturingUpdate->Enabled = false;
				Monitor::Enter(lock_calcurating_scan_image);// wait for finish scanning
				Monitor::Exit(lock_calcurating_scan_image);
			}
	private:Void startChecking()
			{
				buttonScan->Visible = false;
				buttonUse->Visible = true;
				buttonRescan->Visible = true;
				buttonStartScan->Visible = false;
				current_mode = RunMode::Checking;
			}
	private:Void stopChecking()
			{
				current_mode = RunMode::None;
			}
	private:Void startReviewing()
			{
				buttonScan->Visible = false;
				buttonUse->Visible = false;
				buttonRescan->Visible = false;
				buttonStartScan->Visible = true;
				current_mode = RunMode::Reviewing;
			}
	private:Void stopReviewing()
			{
				current_mode = RunMode::None;
			}
	private:Void endReviewing()
			{
				current_mode = RunMode::None;
			}
#pragma endregion
#pragma region Connect to camera / Disconnect camera / Take a shot
	private:bool connect()
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

	private:void disconnect()
			{
				if (camera) delete camera;
				camera = new cv::VideoCapture();
			}
	private:cv::Mat takeShot(bool as_preview)
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
			// For multi-threading
	private:Void showCapturing()
			{
				if (!Monitor::TryEnter(lock_calcurating_scan_image)) return;

				static int frame;
				if (!connect()) return;

				bool show_lines = (frame == 0);
				frame = (frame+1)%(int)(1.0 * 1000.0/timerCapturingUpdate->Interval);

				cv::Mat img;
				try {
					img = takeShot(true);
				} catch (Exception^ e) {
					setMessage(L"Error: " + e->Message);
					disconnect();
					goto end;
				}

				if (show_lines) {
					try {
						scanner->prepare(img);
						img = scanner->drawSideLines(true);
						setMessage(L"The edges of paper are detected.");
					} catch (Exception^ e) {
						setMessage(L"Error: " + e->Message);
					}
				} else {
					try {
						scanner->drawSideLines(img);
					} catch (Exception^ e) {
						setMessage(L"Error: " + e->Message);
					}
				}
				showImage(img);

				end:
				Monitor::Exit(lock_calcurating_scan_image);
			}
	private:template<typename T>
				List<T>^ deepCopyList(List<T>^ src)
			{
				List<T>^ dst = gcnew List<T>(src->Count);
				for each (T^ item in src)
					dst->Add(T(item));
				return dst;
			}
	private:IScannedImage^ scan()
			{
				if (!connect()) 
					throw(L"Camera is not connected");
				setMessage(L"Starting scan");

				cv::Mat img;
				try {
					img = takeShot(false);
				} catch (Exception^ e) {
					setMessage(L"Error: " + e->Message);
					disconnect();
					throw e;
				}

				cv::Mat paper;
				try
				{
					scanner->prepare(img);
					paper = scanner->scan();
				} catch (Exception^ e) {
					throw e;//FIXME
				}

				if (paper.empty())
					throw L"Paper scanning was failed";

				setMessage(L"Paper scanning was finished");
				IScannedImage^ scan = gcnew ScannedImageCV(paper);
				scan->TimeTaken = DateTime::Now;

				return scan;
			}
#pragma endregion
#pragma region 現在選択中の画像
	private:Image^ getCurrentScanImage()
			{
				if (!current_scan_image) throw L"current scan image is null";

				if (current_scan_image->Page) {
					return current_scan_image->Page;
				} else {
					setMessage(String::Format(L"Unable to load image: {0}", current_scan_image->FileName));
					return nullptr;
				}
			}
	private:Void showImage(cv::Mat& image)
			{
				ImageUpdateData^ updater = gcnew ImageUpdateData(image, pictureBox, PictureBoxSizeMode::Zoom); 
				original_context->Post(gcnew SendOrPostCallback(updater, &ImageUpdateData::Draw), false);
			}
	private:Void showImage(Image^ image)
			{
				ImageUpdateData^ updater = gcnew ImageUpdateData(image, pictureBox, PictureBoxSizeMode::Zoom); 
				original_context->Post(gcnew SendOrPostCallback(updater, &ImageUpdateData::Draw), false);
			}
	private:Void showImage()
			{
				if (!current_scan_image) return;
				showImage(current_scan_image->Page);
			}
	private:Void addImage()
			{
				checkedListBoxImages->Items->Add(current_scan_image, true);
			}
	private:Void selectImage()
			{
				int idx = checkedListBoxImages->SelectedIndex;
				if (!checkedListBoxImages->SelectedItem)
					current_scan_image = nullptr;
				else
					current_scan_image = (IScannedImage^)checkedListBoxImages->SelectedItem;
			}
#pragma endregion
#pragma region Save/Load
	private:String^ showSaveDialog()
			{
				SaveFileDialog^ dialog = gcnew SaveFileDialog();
				dialog->Filter = L"Scanned Image info(*.xml)|*.xml";
				if (xml_file_path) dialog->InitialDirectory = xml_file_path;
				if (dialog->ShowDialog() == System::Windows::Forms::DialogResult::Cancel) return nullptr;
				
				xml_file_path = Path::GetDirectoryName(dialog->FileName);
				xml_file_name = Path::GetFileName(dialog->FileName);
				return dialog->FileName;
			}
	private:String^ showLoadDialog()
			{
				OpenFileDialog^ dialog = gcnew OpenFileDialog();
				if (xml_file_path) dialog->InitialDirectory = xml_file_path;
				dialog->Filter = L"Scanned Image info(*.xml)|*.xml";
				if (dialog->ShowDialog() == System::Windows::Forms::DialogResult::Cancel) return nullptr;
				
				xml_file_path = Path::GetDirectoryName(dialog->FileName);
				xml_file_name = Path::GetFileName(dialog->FileName);
				return dialog->FileName;
			}
			// see http://msdn.microsoft.com/ja-jp/library/system.xml.serialization.xmlattributes.xmlignore(v=vs.100).aspx
			//     http://msdn.microsoft.com/ja-jp/library/71s92ee1(v=vs.100).aspx
	private:Void saveImages(String^ xml_file_name)
			{
				xml_file_path = Path::GetDirectoryName(xml_file_name);
				List<ScannedImageCV^ >^ items = gcnew List<ScannedImageCV^ >();
				for each (Object^ item in checkedListBoxImages->CheckedItems)
				{
					ScannedImageCV^ image = dynamic_cast<ScannedImageCV^>(item);
					if (!image) continue;

					if (image->NeedToBeSaved) 
					{
						try {
							image->ImagePath = xml_file_path;
							image->SaveImage();
						} catch (UnauthorizedAccessException^ e) {
							throw e;
						} catch (PathTooLongException^ e) {
							throw e;
						} catch (DirectoryNotFoundException^ e) {
							throw e;
						}
					}

					items->Add(image);
				}

				array<ScannedImageCV^ >^ images = items->ToArray();
				XmlSerializer^ xs = gcnew XmlSerializer(images->GetType());
				StreamWriter^ fs = nullptr;
				bool succeeded = true;
				try
				{
					//TODO: use Path::GetTempFileName() instead
					fs = (gcnew FileInfo(xml_file_name))->CreateText();
					xs->Serialize(fs, images);
				}
				catch (UnauthorizedAccessException^ e)
				{
					succeeded = false;
					MessageBox::Show(e->Message);
				}
				catch (DirectoryNotFoundException^ e)
				{
					succeeded = false;
					MessageBox::Show(e->Message);
				}
				catch (IOException^ e)
				{
					succeeded = false;
					MessageBox::Show(e->Message);
				}
				catch (Exception^ e)
				{
					// unexpected!
					succeeded = false;
					MessageBox::Show(e->Message);
					throw e;
				}
				finally
				{
					if (fs != nullptr) fs->Close();
				}
				if (!succeeded)
				{
					// TODO
				}
			}
	private:Void loadImages(String^ xml_file_name)
			{
				XmlSerializer^ xs = gcnew XmlSerializer(array<ScannedImageCV^ >::typeid);
				FileStream^ fs = nullptr;
				array<ScannedImageCV^ >^ items = nullptr;
				bool succeeded = true;
				try
				{
					fs = File::OpenRead(xml_file_name);
					items = dynamic_cast<array<ScannedImageCV^ >^ >(xs->Deserialize(fs));
				}
				catch (UnauthorizedAccessException^ e)
				{
					succeeded = false;
					MessageBox::Show(e->Message);
				}
				catch (DirectoryNotFoundException^ e)
				{
					succeeded = false;
					MessageBox::Show(e->Message);
				}
				catch (IOException^ e)
				{
					succeeded = false;
					MessageBox::Show(e->Message);
				}
				catch (InvalidOperationException^ e)
				{
					succeeded = false;
					MessageBox::Show(e->Message);
				}
				catch (Exception^ e)
				{
					// unexpected!
					succeeded = false;
					MessageBox::Show(e->Message);
					throw e;
				}
				finally
				{
					fs->Close();
				}
				if (!succeeded)
				{
					//TODO
				}

				checkedListBoxImages->Items->Clear();
				for each (IScannedImage^ image in items) {
					checkedListBoxImages->Items->Add(image, true);
				}
				xml_file_path = Path::GetDirectoryName(xml_file_name);
			}
	private:bool confirmClosing()
			{
				switch (
					MessageBox::Show(
					L"Scanning window will be now closed. Do you want to save images?", 
					L"Save images before closing?", 
					MessageBoxButtons::YesNoCancel))
				{
				case System::Windows::Forms::DialogResult::Yes: 
					showSaveDialog();//TODO: Check failure
					return true;
					break;
				case System::Windows::Forms::DialogResult::No:
					return true;
					break;
				case System::Windows::Forms::DialogResult::Cancel:
					break;
				default:
					throw;
					break;
				}
				return false;
			}
#pragma endregion
#pragma endregion
#pragma region プロパティ
	public:property String^ XmlFilePath
		   {
			   String^ get() { return xml_file_path; }
			   Void set(String^ value) { xml_file_path = value; }
		   }
	public:property String^ XmlFileNameFullPath
		   {
			   String^ get()
			   {
				   if (xml_file_path && xml_file_name)
					   return Path::Combine(xml_file_path, xml_file_name);
				   else if (xml_file_name)
					   return Path::Combine(Directory::GetCurrentDirectory(), xml_file_name);
				   else
					   return nullptr;
			   }
		   }
#pragma endregion
			// Events of controls
	private:Void buttonScan_Click(System::Object^  sender, System::EventArgs^  e)
			{
				switch (current_mode) {
				case RunMode::Capturinging:
					current_scan_image = scan();
					stopCapturinging();
					startChecking();
					showImage();
					break;
				case RunMode::None:
					startCapturinging();
					break;
				}
			}
	private:Void buttonUse_Click(System::Object^  sender, System::EventArgs^  e) 
			{
				addImage();
				stopChecking();
				startCapturinging();
			}
	private:Void buttonRescan_Click(System::Object^  sender, System::EventArgs^  e)
			{
				stopChecking();
				startCapturinging();
			}
	private:Void buttonStartScan_Click(System::Object^  sender, System::EventArgs^  e)
			{
				stopReviewing();
				startCapturinging();
			}
	private:Void checkedListBoxImages_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
			{
				switch (current_mode) {
				case RunMode::Capturinging:
					stopCapturinging();
					startReviewing();
					break;
				case RunMode::Checking:
					stopChecking();
					startReviewing();
					break;
				case RunMode::None:
					startReviewing();
					break;
				}

				current_scan_image = dynamic_cast<IScannedImage^>(checkedListBoxImages->SelectedItem);
				showImage();
			}
	private:Void timerCapturingUpdate_Tick(System::Object^  sender, System::EventArgs^  e)
			{
				Thread^ t = gcnew Thread(gcnew ThreadStart(this, &Scan::showCapturing));
				t->Start();
			}
	private:Void Scan_Load(System::Object^  sender, System::EventArgs^  e)
			{
				timerCapturingUpdate->Interval = 100;
				startCapturinging();
			}
	private:Void Scan_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
			{
				if (e->CloseReason == CloseReason::WindowsShutDown)
				{
					// TODO
				}
				else
				{
					e->Cancel = !confirmClosing();
				}
			}
	private:Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			{
				String^ file_name = showLoadDialog();
				if (!file_name) return;

				try {
					loadImages(file_name);
				} catch (String^ msg) {
					setMessage(L"Failed to load file: " + msg);
				}
			}
	private:Void saveToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			{
				String^ file_name = showSaveDialog();
				if (!file_name) return;

				try {
					saveImages(file_name);
				} catch (String^ msg) {
					setMessage(L"Failed to save file: " + msg);
				}
			}
	private:Void quitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			{
				if (confirmClosing()) Close();
			}
	};
}}
