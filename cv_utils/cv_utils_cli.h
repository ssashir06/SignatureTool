#pragma once
#include <opencv2/opencv.hpp>

namespace CVUtil { namespace CLI {
	using namespace System;
	using namespace System::Drawing;
	Bitmap^ convertImage(cv::Mat& img_src);
	//FIXME: not tested
	cv::Mat convertImage(Image^ img_src);
	std::string convertString(String^ file_name);
	String^ convertString(const std::string& file_name);

	namespace Control
	{
		using namespace System::Windows::Forms;
		System::Void showImage(cv::Mat& img_src, PictureBox^ picturebox, bool fit_to_pixture_box);
	}
}}
