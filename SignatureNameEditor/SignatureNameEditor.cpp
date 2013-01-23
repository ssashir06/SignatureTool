// SignatureNameEditor.cpp : メイン プロジェクト ファイルです。

#include "stdafx.h"
//#include "SignPreview.h"
#include "Scan.h"

using namespace SignatureNameEditor;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// コントロールが作成される前に、Windows XP ビジュアル効果を有効にします
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// メイン ウィンドウを作成して、実行します
	//Application::Run(gcnew SignPreview());
	Application::Run(gcnew Scan());
	return 0;
}
