#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include "cv_utils_cli.h"

namespace CVUtil { namespace CLI {
	using namespace System;
	using namespace System::Drawing;
	using namespace System::Drawing::Imaging;
	using namespace System::Runtime::InteropServices;
	using namespace System::IO;

	Bitmap^ convertImage(cv::Mat& img_src)
	{
		PixelFormat fmt;

		if (img_src.depth() != CV_8U) 
			throw L"the depth of OpenCV image source is wrong.";
		if (img_src.channels() == 1) {
			fmt = PixelFormat::Format8bppIndexed;
		} else if (img_src.channels() == 3) {
			fmt = PixelFormat::Format24bppRgb;
		} else {
			throw L"the channels of OpenCV image source is wrong.";
		}

		IplImage img_ipl;
		cv::Mat img_merged;

		if (img_src.cols%4 == 0) {
			img_ipl = img_src;
		} else {
			cv::Mat alpha = cv::Mat_<unsigned char>(img_src.rows, img_src.cols);
			std::vector<cv::Mat> channels;
			cv::split(img_src, channels);
			channels.push_back(alpha);
			cv::merge(channels, img_merged);
			img_ipl = img_merged;

			fmt = PixelFormat::Format32bppRgb;
		}

		IntPtr ip(new unsigned char[img_ipl.widthStep*img_ipl.height]);
		memcpy(ip.ToPointer(),img_ipl.imageData,img_ipl.widthStep*img_ipl.height);

		Bitmap^ bmp = gcnew Bitmap(
			img_ipl.width, img_ipl.height, img_ipl.widthStep,
			fmt, ip);

		if (fmt == PixelFormat::Format8bppIndexed) {
			Imaging::ColorPalette^ pal = bmp->Palette;

			for (int i = 0; i < 256; i++)
				pal->Entries[i] = Color::FromArgb(255, i, i, i);
		}

		return bmp;
	}

	cv::Mat convertImage(Image^ image)
	{
		if (!image) return cv::Mat();

		//FIXME
		MemoryStream^ ms = gcnew MemoryStream();
		image->Save(ms, ImageFormat::Bmp);
		auto buffer_array = ms->GetBuffer();
		pin_ptr<unsigned char>buffer_pin_ptr = &buffer_array[0];
		unsigned char* buffer_ptr = buffer_pin_ptr;
		cv::Mat buffer_mat = cv::Mat(1, buffer_array->Length, CV_8UC1, (void*)buffer_ptr);
		cv::Mat image_mat;
		cv::imdecode(buffer_mat,1, &image_mat);
		return image_mat;
	}

	std::string convertString(String^ str)
	{
		IntPtr ptr = Marshal::StringToHGlobalAnsi(str);
		std::string string_stl((char*)(void*)ptr);
		Marshal::FreeHGlobal(ptr);
		return string_stl;
	}

	String^ convertString(const std::string& str)
	{
		return Marshal::PtrToStringAnsi((IntPtr)(void*)str.c_str());
	}

	namespace Control
	{
		using namespace System::Windows::Forms;
		System::Void showImage(cv::Mat& img_src, PictureBox^ picturebox, bool fit_to_pixture_box)
		{
			Bitmap^ bmp = convertImage(img_src);

			switch (fit_to_pixture_box) {
			case true:
				picturebox->SizeMode = PictureBoxSizeMode::Zoom;
				//picturebox->ClientSize = System::Drawing::Size(bmp->Width, bmp->Height);
				break;
			case false:
				picturebox->SizeMode = PictureBoxSizeMode::Normal;
				break;
			}
			picturebox->Image = dynamic_cast<Image^>(bmp);
		}
	}
}}
