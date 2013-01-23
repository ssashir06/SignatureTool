#pragma once
#include "stdafx.h"
#include <opencv2/opencv.hpp>

namespace CVUtil { namespace CLI
{
	using namespace System;
	using namespace System::Drawing;
	Bitmap^ convertMatToBitmap(cv::Mat& img_src);
	std::string convertFileName(String^ file_name);
	String^ convertFileName(const std::string& file_name);

	namespace Control
	{
		using namespace System::Windows::Forms;
		System::Void showImage(cv::Mat& img_src, PictureBox^ picturebox, bool fit_to_pixture_box);
	}
}}
