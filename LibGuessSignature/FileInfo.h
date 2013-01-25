#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include "GuessSignature.h"
namespace Signature
{
	namespace Image
	{
		class FileInfo : public Info
		{
		public:
			struct FileDetails
			{
				std::string img_file_name;
				cv::Rect area;

				FileDetails();
				FileDetails(const FileDetails& src);
				FileDetails& operator=(const FileDetails& src);
				virtual ~FileDetails();

				void writeInfo(cv::FileStorage& fs) const;
				void readInfo(const cv::FileNode& fn);
			};
		protected:
			std::vector<FileDetails> infomations;
		public:
			FileInfo(void);
			FileInfo(const FileInfo& src);
			FileInfo& operator=(const FileInfo& src);
			virtual ~FileInfo(void);

			virtual void addSignature(const Base& signature, const FileDetails& file, const MatchingMachines& machines);

			FileDetails getFileDetails(Idx idx) const;
			void setFileDetails(Idx idx, const FileDetails& file);

			void write(cv::FileStorage& fs) const;
			void read(cv::FileStorage& fs, bool load_image = false);

			virtual void clear();
			virtual void resize(size_t size);
		};
	}
}

