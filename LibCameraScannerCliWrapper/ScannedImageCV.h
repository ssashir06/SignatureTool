#pragma once
#include <opencv2/opencv.hpp>
#include "ScannedImagePure.h"
#include "../LibGuessSignature/GuessSignature.h"

namespace CameraScanner { namespace CLI {
	using namespace System;
	using namespace System::Xml::Serialization;

	public ref class ScannedImageCV : public ScannedImagePure
	{
	public:
		ScannedImageCV(void);
		ScannedImageCV(cv::Mat& image);
		virtual ~ScannedImageCV(void);
	public:
		[XmlIgnore]
		property cv::Mat CvPage
		{
			cv::Mat get();
			Void set(cv::Mat value);
		}
	};
}}
