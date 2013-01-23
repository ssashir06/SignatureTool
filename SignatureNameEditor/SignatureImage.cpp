#include "StdAfx.h"
#include <opencv2/opencv.hpp>
#include "cv_utils_cli.h"
#include "SignatureImage.h"

namespace SignatureNameEditor {
	Rect::Rect(cv::Rect& cvrect)
	{
		x = cvrect.x;
		y = cvrect.y;
		width = cvrect.width;
		height = cvrect.height;
	}
	
	Rect::Rect(int x, int y, int width, int height, bool used)
	{
		this->x = x, this->y = y;
		this->width = width, this->height = height;
		this->used = used;
	}

	String^ Rect::ToString()
	{
		return String::Format(L"({0},{1}) Size({2},{3})", x, y, width, height);
	}

	cv::Mat* SignatureImage::Image::get()
	{
		return image;
	}

	System::Void SignatureImage::Image::set(cv::Mat* value)
	{
		image = value;
		TimeTaken = DateTime::Now;
	}

	SignatureImage::SignatureImage(void)
	{
		Image = NULL;
		//guess_info = NULL;
	}

	SignatureImage::~SignatureImage(void)
	{
		//if (guess_info) delete guess_info;
		if (image) delete image;
	}

	//Signature::Image::Info* SignatureImage::GuessInfo::get()
	//{
	//	return guess_info;
	//}

	//void SignatureImage::GuessInfo::set(Signature::Image::Info* value)
	//{
	//	if (guess_info) delete guess_info;
	//	guess_info = value;
	//}

	System::Void SignatureImage::saveImage(String^ path, String^ file_name)
	{
		if (!image) return;

		if (!path) {
			path = Directory::GetCurrentDirectory();
		}
		if (!file_name) {
			file_name = String::Format(L"IMG_SCAN_{0}.jpg", TimeTaken.ToString(L"yyyy_MM_dd_HH_mm_ss"));
		}
		String^ file_name_full = Path::Combine(path, file_name);
		String^ file_name_full_wo_ext = Path::Combine(path, Path::GetFileNameWithoutExtension(file_name_full));
		String^ file_ext = Path::GetExtension(file_name_full);

		int i=0;
		while (File::Exists(file_name_full)) {
			file_name_full = String::Format(L"{0} ({2}){1}", file_name_full_wo_ext, file_ext, ++i);
		}

		// FIXME: is better to use try .. catch .. ?
		FileStream^ fs = File::OpenWrite(file_name_full);
		fs->Close();

		cv::imwrite(CVUtil::CLI::convertFileName(file_name_full), *image);
		FileName = file_name;
		delete image;
		image = NULL;
	}

	String^ SignatureImage::ToString()
	{
		return String::Format(L"Name:{0} Taken:{1} File:{2}", Name ? Name : "?", TimeTaken , FileName ? FileName : "?");
	}
}
