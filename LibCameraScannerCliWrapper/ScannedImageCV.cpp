#include "StdAfx.h"
#include <opencv2/opencv.hpp>
#include "../cv_utils/cv_utils_cli.h"
#include "ScannedImageCV.h"

namespace CameraScanner { namespace CLI {
	using namespace System;
	using namespace System::Drawing::Imaging;

	ScannedImageCV::ScannedImageCV(void)
		: ScannedImagePure()
	{
	}

	ScannedImageCV::ScannedImageCV(cv::Mat& image)
		: ScannedImagePure()
	{
		CvPage = image;
	}

	ScannedImageCV::~ScannedImageCV(void)
	{
	}

	cv::Mat ScannedImageCV::CvPage::get()
	{
		return CVUtil::CLI::convertImage(img);
	}

	Void ScannedImageCV::CvPage::set(cv::Mat value)
	{
		img = CVUtil::CLI::convertImage(value);
	}
}}
