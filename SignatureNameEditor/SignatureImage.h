#pragma once
#include <opencv2/opencv.hpp>
#include "../LibGuessSignature/GuessSignature.h"

namespace SignatureNameEditor {
	using namespace System;
	using namespace System::IO;
	using namespace System::Collections::Generic;
	using namespace System::Collections;
	using namespace System::Xml::Serialization;

	public value struct Rect
	{
		property bool used;
		property int x;
		property int y;
		property int width;
		property int height;

		Rect(cv::Rect& cvrect);
		Rect(int x, int y, int width, int height, bool used);
		virtual String^ ToString() override;
	};
	public ref class SignatureImage
	{
	protected:
		cv::Mat* image;
		//Signature::Image::Info* guess_info;
	public:
		property String^ FileName;
		property String^ Name;
		property List<Rect>^ TrimmingAreas;
		property DateTime TimeTaken;
		[XmlIgnore]
		property cv::Mat* Image{
			cv::Mat* get();
			System::Void set(cv::Mat* value);
		}
		//property Signature::Image::Info* GuessInfo
		//{
		//	Signature::Image::Info* get();
		//	void set(Signature::Image::Info* value);
		//}

		SignatureImage(void);
		virtual ~SignatureImage(void);
		System::Void saveImage(String^ path, String^ file_name);
		virtual String^ ToString() override;
	};
}
