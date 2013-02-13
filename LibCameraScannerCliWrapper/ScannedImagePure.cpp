#include "stdafx.h"
#include "ScannedImagePure.h"

namespace CameraScanner
{
	using namespace System;
	using namespace System::Drawing::Imaging;
	using namespace System::IO;
	ScannedImagePure::ScannedImagePure(void)
		: path(Directory::GetCurrentDirectory())
	{
	}

	ScannedImagePure::~ScannedImagePure(void)
	{
	}

	Void ScannedImagePure::SaveImage()
	{
		SaveImage(nullptr);
	}

	Void ScannedImagePure::SaveImage(String^ file_name)
	{
		if (!img) throw gcnew Exception("image is empty");
		if (!file_name) {
			file_name = String::Format(L"IMG_SCAN_{0}.jpg", TimeTaken.ToString(L"yyyy_MM_dd_HH_mm_ss"));
		}
		String^ file_name_full = Path::Combine(ImagePath, file_name);
		String^ file_name_full_wo_ext = Path::Combine(ImagePath, Path::GetFileNameWithoutExtension(file_name_full));
		String^ file_ext = Path::GetExtension(file_name_full);

		int i=0;
		while (File::Exists(file_name_full)) {
			file_name_full = String::Format(L"{0} ({2}){1}", file_name_full_wo_ext, file_ext, ++i);
		}

		// FIXME: is better to use 'try .. catch ..' ?
		FileStream^ fs = File::OpenWrite(file_name_full);
		EncoderParameters^ encoder_params = gcnew EncoderParameters(1);
		encoder_params->Param[0] = gcnew EncoderParameter(Encoder::Quality, (Int64)25);

		// TODO: write Exif
		img->Save(fs, GetEncoderInfo(ImageFormat::Jpeg), encoder_params);
		fs->Close();

		FileName = file_name;
	}

	String^ ScannedImagePure::ToString()
	{
		return String::Format(L"Taken:{0} File:{1}", TimeTaken , FileName ? FileName : L"?");
	}

	Image^ ScannedImagePure::Page::get()
	{
		if (!img)
		{
			if (!file_name) return nullptr;
			String^ file_name_full = Path::Combine(ImagePath, file_name);
			try {
				img = Bitmap::FromFile(file_name_full);
			} catch (Exception^ e)
			{
				// Error
			}
		}
		return img;
	}

	Void ScannedImagePure::Page::set(Image^ value)
	{
		img = value;
	}

	String^ ScannedImagePure::FileName::get()
	{
		return file_name;
	}

	Void ScannedImagePure::FileName::set(String^ value)
	{
		file_name = value;
	}

	String^ ScannedImagePure::ImagePath::get()
	{
		if (!path) return Directory::GetCurrentDirectory();
		else return path;
	}

	Void ScannedImagePure::ImagePath::set(String^ value)
	{
		path = value;
	}

	DateTime ScannedImagePure::TimeTaken::get()
	{
		return time_taken;
	}

	Void ScannedImagePure::TimeTaken::set(DateTime value)
	{
		time_taken = value;
	}

	Boolean ScannedImagePure::NeedToBeSaved::get()
	{
		return !file_name && !!img;
	}

	ImageCodecInfo^ ScannedImagePure::GetEncoderInfo(ImageFormat^ format)
	{
		int j;
		array<ImageCodecInfo^>^ encoders;
		encoders = ImageCodecInfo::GetImageEncoders();
		for (j = 0; j < encoders->Length; ++j)
		{
			if (encoders[j]->FormatID == format->Guid)
				return encoders[ j ];

		}
		return nullptr;
	}
}
