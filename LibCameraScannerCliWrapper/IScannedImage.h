#pragma once
namespace CameraScanner {
	using namespace System;
	using namespace System::Xml::Serialization;
	using namespace System::Drawing;

	// –½–¼‹K‘¥ ŠÖ”‚Í‘å•¶š‚©‚çn‚ß‚é

	public interface class IScannedImage
	{
	public:
		Void SaveImage();
		Void SaveImage(String^ file_name);
		virtual String^ ToString() override;

		[XmlIgnore]
		property Image^ Page
		{
			Image^ get();
			Void set(Image^ value);
		}
		property String^ FileName
		{
			String^ get();
			Void set(String^ value);
		}
		property String^ ImagePath
		{
			String^ get();
			Void set(String^ valaue);
		}
		property DateTime TimeTaken
		{
			DateTime get();
			Void set(DateTime value);
		}
		property Boolean NeedToBeSaved
		{
			Boolean get();
		}
	};
}
