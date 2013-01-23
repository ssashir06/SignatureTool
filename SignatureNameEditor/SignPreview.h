#pragma once
#include "../LibGuessSignature/EvalEasy.h"
#include "../LibGuessSignature/FileInfo.h"
#include "Scan.h"

namespace SignatureNameEditor {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// SignPreview の概要
	/// </summary>
	public ref class SignPreview : public System::Windows::Forms::Form
	{
	public:
		SignPreview(void)
		{
			InitializeComponent();
			//
			//TODO: ここにコンストラクター コードを追加します
			//
		}

	protected:
		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		~SignPreview()
		{
			if (components)
			{
				delete components;
			}
		}

	protected: 


	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
	private: System::Windows::Forms::GroupBox^  groupBox3;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::CheckBox^  checkBoxSampling;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::TextBox^  textBoxNewName;
	private: System::Windows::Forms::ListBox^  listManual;
	private: System::Windows::Forms::ListBox^  listAutomatic;
	private: System::Windows::Forms::PictureBox^  pictureBoxPreview;
	private: System::Windows::Forms::GroupBox^  groupBox4;
	private: System::Windows::Forms::ListBox^  listSignature;
	private: System::Windows::Forms::CheckBox^  checkBoxIgnore;
	private: System::Windows::Forms::ToolStripMenuItem^  samplesToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  loadSamplesStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveSamplesToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  imagesToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  scanImagesToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  loadImagesToolStripMenuItem;



	protected: 



	private:
		/// <summary>
		/// 必要なデザイナー変数です。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// デザイナー サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディターで変更しないでください。
		/// </summary>
		void InitializeComponent(void)
		{
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->samplesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->loadSamplesStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveSamplesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->imagesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->scanImagesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->loadImagesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->checkBoxIgnore = (gcnew System::Windows::Forms::CheckBox());
			this->checkBoxSampling = (gcnew System::Windows::Forms::CheckBox());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->textBoxNewName = (gcnew System::Windows::Forms::TextBox());
			this->listManual = (gcnew System::Windows::Forms::ListBox());
			this->listAutomatic = (gcnew System::Windows::Forms::ListBox());
			this->pictureBoxPreview = (gcnew System::Windows::Forms::PictureBox());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->listSignature = (gcnew System::Windows::Forms::ListBox());
			this->menuStrip1->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxPreview))->BeginInit();
			this->groupBox4->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->fileToolStripMenuItem, 
				this->samplesToolStripMenuItem, this->imagesToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Padding = System::Windows::Forms::Padding(7, 2, 0, 2);
			this->menuStrip1->Size = System::Drawing::Size(975, 26);
			this->menuStrip1->TabIndex = 6;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->exitToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(40, 22);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(98, 22);
			this->exitToolStripMenuItem->Text = L"Exit";
			// 
			// samplesToolStripMenuItem
			// 
			this->samplesToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->loadSamplesStripMenuItem, 
				this->saveSamplesToolStripMenuItem});
			this->samplesToolStripMenuItem->Name = L"samplesToolStripMenuItem";
			this->samplesToolStripMenuItem->Size = System::Drawing::Size(70, 22);
			this->samplesToolStripMenuItem->Text = L"Samples";
			// 
			// loadSamplesStripMenuItem
			// 
			this->loadSamplesStripMenuItem->Name = L"loadSamplesStripMenuItem";
			this->loadSamplesStripMenuItem->Size = System::Drawing::Size(105, 22);
			this->loadSamplesStripMenuItem->Text = L"Load";
			// 
			// saveSamplesToolStripMenuItem
			// 
			this->saveSamplesToolStripMenuItem->Name = L"saveSamplesToolStripMenuItem";
			this->saveSamplesToolStripMenuItem->Size = System::Drawing::Size(105, 22);
			this->saveSamplesToolStripMenuItem->Text = L"Save";
			// 
			// imagesToolStripMenuItem
			// 
			this->imagesToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->scanImagesToolStripMenuItem, 
				this->loadImagesToolStripMenuItem});
			this->imagesToolStripMenuItem->Name = L"imagesToolStripMenuItem";
			this->imagesToolStripMenuItem->Size = System::Drawing::Size(64, 22);
			this->imagesToolStripMenuItem->Text = L"Images";
			// 
			// scanImagesToolStripMenuItem
			// 
			this->scanImagesToolStripMenuItem->Name = L"scanImagesToolStripMenuItem";
			this->scanImagesToolStripMenuItem->Size = System::Drawing::Size(104, 22);
			this->scanImagesToolStripMenuItem->Text = L"Scan";
			// 
			// loadImagesToolStripMenuItem
			// 
			this->loadImagesToolStripMenuItem->Name = L"loadImagesToolStripMenuItem";
			this->loadImagesToolStripMenuItem->Size = System::Drawing::Size(104, 22);
			this->loadImagesToolStripMenuItem->Text = L"Load";
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->groupBox1);
			this->groupBox3->Controls->Add(this->pictureBoxPreview);
			this->groupBox3->Location = System::Drawing::Point(313, 48);
			this->groupBox3->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Padding = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->groupBox3->Size = System::Drawing::Size(649, 561);
			this->groupBox3->TabIndex = 7;
			this->groupBox3->TabStop = false;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->checkBoxIgnore);
			this->groupBox1->Controls->Add(this->checkBoxSampling);
			this->groupBox1->Controls->Add(this->groupBox2);
			this->groupBox1->Controls->Add(this->listManual);
			this->groupBox1->Controls->Add(this->listAutomatic);
			this->groupBox1->Location = System::Drawing::Point(7, 286);
			this->groupBox1->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Padding = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->groupBox1->Size = System::Drawing::Size(635, 270);
			this->groupBox1->TabIndex = 7;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Signature name";
			// 
			// checkBoxIgnore
			// 
			this->checkBoxIgnore->AutoSize = true;
			this->checkBoxIgnore->Location = System::Drawing::Point(380, 118);
			this->checkBoxIgnore->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->checkBoxIgnore->Name = L"checkBoxIgnore";
			this->checkBoxIgnore->Size = System::Drawing::Size(60, 19);
			this->checkBoxIgnore->TabIndex = 6;
			this->checkBoxIgnore->Text = L"ignore";
			this->checkBoxIgnore->UseVisualStyleBackColor = true;
			// 
			// checkBoxSampling
			// 
			this->checkBoxSampling->AutoSize = true;
			this->checkBoxSampling->Location = System::Drawing::Point(380, 90);
			this->checkBoxSampling->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->checkBoxSampling->Name = L"checkBoxSampling";
			this->checkBoxSampling->Size = System::Drawing::Size(142, 19);
			this->checkBoxSampling->TabIndex = 5;
			this->checkBoxSampling->Text = L"add to sample images";
			this->checkBoxSampling->UseVisualStyleBackColor = true;
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->textBoxNewName);
			this->groupBox2->Location = System::Drawing::Point(380, 22);
			this->groupBox2->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Padding = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->groupBox2->Size = System::Drawing::Size(247, 60);
			this->groupBox2->TabIndex = 4;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"add new name";
			// 
			// textBoxNewName
			// 
			this->textBoxNewName->Location = System::Drawing::Point(7, 22);
			this->textBoxNewName->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->textBoxNewName->Name = L"textBoxNewName";
			this->textBoxNewName->Size = System::Drawing::Size(233, 23);
			this->textBoxNewName->TabIndex = 0;
			// 
			// listManual
			// 
			this->listManual->FormattingEnabled = true;
			this->listManual->ItemHeight = 15;
			this->listManual->Location = System::Drawing::Point(194, 22);
			this->listManual->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->listManual->Name = L"listManual";
			this->listManual->Size = System::Drawing::Size(179, 229);
			this->listManual->TabIndex = 3;
			// 
			// listAutomatic
			// 
			this->listAutomatic->FormattingEnabled = true;
			this->listAutomatic->ItemHeight = 15;
			this->listAutomatic->Location = System::Drawing::Point(7, 22);
			this->listAutomatic->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->listAutomatic->Name = L"listAutomatic";
			this->listAutomatic->Size = System::Drawing::Size(179, 229);
			this->listAutomatic->TabIndex = 2;
			// 
			// pictureBoxPreview
			// 
			this->pictureBoxPreview->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureBoxPreview->Location = System::Drawing::Point(7, 22);
			this->pictureBoxPreview->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->pictureBoxPreview->Name = L"pictureBoxPreview";
			this->pictureBoxPreview->Size = System::Drawing::Size(634, 256);
			this->pictureBoxPreview->TabIndex = 6;
			this->pictureBoxPreview->TabStop = false;
			// 
			// groupBox4
			// 
			this->groupBox4->Controls->Add(this->listSignature);
			this->groupBox4->Location = System::Drawing::Point(14, 48);
			this->groupBox4->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Padding = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->groupBox4->Size = System::Drawing::Size(292, 561);
			this->groupBox4->TabIndex = 8;
			this->groupBox4->TabStop = false;
			this->groupBox4->Text = L"Signatures";
			// 
			// listSignature
			// 
			this->listSignature->FormattingEnabled = true;
			this->listSignature->ItemHeight = 15;
			this->listSignature->Location = System::Drawing::Point(7, 22);
			this->listSignature->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->listSignature->Name = L"listSignature";
			this->listSignature->Size = System::Drawing::Size(277, 514);
			this->listSignature->TabIndex = 0;
			// 
			// SignPreview
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 15);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(975, 624);
			this->Controls->Add(this->groupBox4);
			this->Controls->Add(this->groupBox3);
			this->Controls->Add(this->menuStrip1);
			this->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->MainMenuStrip = this->menuStrip1;
			this->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->Name = L"SignPreview";
			this->Text = L"SignPreview";
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->groupBox3->ResumeLayout(false);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxPreview))->EndInit();
			this->groupBox4->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
};
}

