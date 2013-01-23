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
			this->saveSamplesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->imagesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->scanImagesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->loadImagesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
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
			this->menuStrip1->Size = System::Drawing::Size(836, 26);
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
			this->exitToolStripMenuItem->Size = System::Drawing::Size(152, 22);
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
			this->loadSamplesStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->loadSamplesStripMenuItem->Text = L"Load";
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->groupBox1);
			this->groupBox3->Controls->Add(this->pictureBoxPreview);
			this->groupBox3->Location = System::Drawing::Point(268, 38);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(556, 449);
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
			this->groupBox1->Location = System::Drawing::Point(6, 229);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(544, 216);
			this->groupBox1->TabIndex = 7;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Signature name";
			// 
			// checkBoxIgnore
			// 
			this->checkBoxIgnore->AutoSize = true;
			this->checkBoxIgnore->Location = System::Drawing::Point(326, 94);
			this->checkBoxIgnore->Name = L"checkBoxIgnore";
			this->checkBoxIgnore->Size = System::Drawing::Size(55, 16);
			this->checkBoxIgnore->TabIndex = 6;
			this->checkBoxIgnore->Text = L"ignore";
			this->checkBoxIgnore->UseVisualStyleBackColor = true;
			// 
			// checkBoxSampling
			// 
			this->checkBoxSampling->AutoSize = true;
			this->checkBoxSampling->Location = System::Drawing::Point(326, 72);
			this->checkBoxSampling->Name = L"checkBoxSampling";
			this->checkBoxSampling->Size = System::Drawing::Size(136, 16);
			this->checkBoxSampling->TabIndex = 5;
			this->checkBoxSampling->Text = L"add to sample images";
			this->checkBoxSampling->UseVisualStyleBackColor = true;
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->textBoxNewName);
			this->groupBox2->Location = System::Drawing::Point(326, 18);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(212, 48);
			this->groupBox2->TabIndex = 4;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"add new name";
			// 
			// textBoxNewName
			// 
			this->textBoxNewName->Location = System::Drawing::Point(6, 18);
			this->textBoxNewName->Name = L"textBoxNewName";
			this->textBoxNewName->Size = System::Drawing::Size(200, 19);
			this->textBoxNewName->TabIndex = 0;
			// 
			// listManual
			// 
			this->listManual->FormattingEnabled = true;
			this->listManual->ItemHeight = 12;
			this->listManual->Location = System::Drawing::Point(166, 18);
			this->listManual->Name = L"listManual";
			this->listManual->Size = System::Drawing::Size(154, 184);
			this->listManual->TabIndex = 3;
			// 
			// listAutomatic
			// 
			this->listAutomatic->FormattingEnabled = true;
			this->listAutomatic->ItemHeight = 12;
			this->listAutomatic->Location = System::Drawing::Point(6, 18);
			this->listAutomatic->Name = L"listAutomatic";
			this->listAutomatic->Size = System::Drawing::Size(154, 184);
			this->listAutomatic->TabIndex = 2;
			// 
			// pictureBoxPreview
			// 
			this->pictureBoxPreview->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureBoxPreview->Location = System::Drawing::Point(6, 18);
			this->pictureBoxPreview->Name = L"pictureBoxPreview";
			this->pictureBoxPreview->Size = System::Drawing::Size(544, 205);
			this->pictureBoxPreview->TabIndex = 6;
			this->pictureBoxPreview->TabStop = false;
			// 
			// groupBox4
			// 
			this->groupBox4->Controls->Add(this->listSignature);
			this->groupBox4->Location = System::Drawing::Point(12, 38);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(250, 449);
			this->groupBox4->TabIndex = 8;
			this->groupBox4->TabStop = false;
			this->groupBox4->Text = L"Signatures";
			// 
			// listSignature
			// 
			this->listSignature->FormattingEnabled = true;
			this->listSignature->ItemHeight = 12;
			this->listSignature->Location = System::Drawing::Point(6, 18);
			this->listSignature->Name = L"listSignature";
			this->listSignature->Size = System::Drawing::Size(238, 412);
			this->listSignature->TabIndex = 0;
			// 
			// saveSamplesToolStripMenuItem
			// 
			this->saveSamplesToolStripMenuItem->Name = L"saveSamplesToolStripMenuItem";
			this->saveSamplesToolStripMenuItem->Size = System::Drawing::Size(152, 22);
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
			this->scanImagesToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->scanImagesToolStripMenuItem->Text = L"Scan";
			// 
			// loadImagesToolStripMenuItem
			// 
			this->loadImagesToolStripMenuItem->Name = L"loadImagesToolStripMenuItem";
			this->loadImagesToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->loadImagesToolStripMenuItem->Text = L"Load";
			// 
			// SignPreview
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(836, 499);
			this->Controls->Add(this->groupBox4);
			this->Controls->Add(this->groupBox3);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
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

