#pragma once
#include "IScannedImage.h"
namespace CameraScanner
{
	using namespace System;
	using namespace System::Xml::Serialization;
	using namespace System::Drawing;
	using namespace System::Drawing::Imaging;

	public ref class ScannedImagePure : public IScannedImage
	{
	protected:
		Image^ img;
		String^ file_name;
		String^ path;
		DateTime time_taken;
	public:
		ScannedImagePure(void);
		~ScannedImagePure(void);
		virtual Void SaveImage();
		virtual Void SaveImage(String^ file_name);
		virtual String^ ToString() override;

		[XmlIgnore]
		property Image^ Page
		{
			virtual Image^ get();
			virtual Void set(Image^ value);
		}
		property String^ FileName
		{
			virtual String^ get();
			virtual Void set(String^ value);
		}
		property String^ ImagePath
		{
			virtual String^ get();
			virtual Void set(String^ valaue);
		}
		property DateTime TimeTaken
		{
			virtual DateTime get();
			virtual Void set(DateTime value);
		}
		property Boolean NeedToBeSaved
		{
			virtual Boolean get();
		}
	protected:
		static ImageCodecInfo^ GetEncoderInfo(ImageFormat^ format);
	};
}
