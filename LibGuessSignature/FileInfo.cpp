#include <iostream>
#include <opencv2/opencv.hpp>
#include "FileInfo.h"
using namespace std;
using namespace cv;

namespace Signature
{
	namespace Image
	{
		FileInfo::FileDetails::FileDetails()
		{
		}

		FileInfo::FileDetails::FileDetails(const FileInfo::FileDetails& src)
		{
			img_file_name = src.img_file_name;
			area = src.area;
		}

		FileInfo::FileDetails& FileInfo::FileDetails::operator=(const FileInfo::FileDetails& src)
		{
			img_file_name = src.img_file_name;
			area = src.area;
			return *this;
		}

		FileInfo::FileDetails::~FileDetails()
		{
		}

		void FileInfo::FileDetails::writeInfo(FileStorage& fs) const
		{
			fs << "FileName" << img_file_name;
			fs << "Trim" << "{";
			fs << "x" << area.x;
			fs << "y" << area.y;
			fs << "width" << area.width;
			fs << "height" << area.height;
			fs << "}";
		}

		void FileInfo::FileDetails::readInfo(const FileNode& fn)
		{
			fn["FileName"] >> img_file_name;
			FileNode fn_a= fn["Trim"];
			fn_a["x"] >> area.x;
			fn_a["y"] >> area.y;
			fn_a["width"] >> area.width;
			fn_a["height"] >> area.height;
		}

		FileInfo::FileInfo(void)
			: Info()
		{
		}

		FileInfo::FileInfo(const FileInfo& src)
			: Info(src)
		{
			infomations = src.infomations;
		}

		FileInfo& FileInfo::operator=(const FileInfo& src)
		{
			Info::operator=(src);
			infomations = src.infomations;
			return *this;
		}

		FileInfo::~FileInfo(void)
		{
		}

		void FileInfo::addSignature(const Conclusive& signature, const FileDetails& file, const cv::FeatureDetector& detector, const cv::DescriptorExtractor& extractor)//FIXME
		{
			Idx idx = size();
			Info::addSignature(signature, detector, extractor);
			setFileDetails(idx, file);
		}

		void FileInfo::addSignature(const Candidate& signature, const FileDetails& file, const cv::FeatureDetector& detector, const cv::DescriptorExtractor& extractor)//FIXME
		{
			Idx idx = size();
			Info::addSignature(signature, detector, extractor);
			setFileDetails(idx, file);
		}

		FileInfo::FileDetails FileInfo::getFileDetails(Idx idx) const
		{
			if (idx < 0 || size() <= idx) throw "Invalid idx";
			return infomations[idx];
		}

		void FileInfo::setFileDetails(Idx idx, const FileDetails& file)
		{
			if (idx < 0 || size() <= idx) throw "Invalid idx";
			infomations[idx] = file;
		}

		void FileInfo::write(FileStorage& fs) const
		{
			fs << "ImageInfo" << "{";
			for (int i=0; i<size(); i++) {
				fs << "Id" << getID(i);
				fs << "Descriptor" << getDescriptor(i);
				fs << "Name" << getName(i);
				cv::write(fs, "KeyPoints",getKeyPoints(i));

				fs << "FileDetails" << "{";
				infomations[i].writeInfo(fs);
				fs << "}";
			}
			fs << "}";
		}

		void FileInfo::read(FileStorage& fs, bool load_image)
		{
			FileNode fn;
			Idx i = 0;
			while (!(fn = fs["ImageInfo"]).empty()) {
				ID id;
				KeyPoints keypoints;
				Descriptor descriptor;
				string name;

				fn["Id"] >> id;
				fn["Descriptor"] >> descriptor;
				fn["Name"] >> name;
				cv::read(fn["KeyPoints"], keypoints);

				FileNode info_node;
				FileDetails info;
				info.readInfo(fn["FileDetails"]);

				if (load_image) {
					Mat img_src = imread(info.img_file_name);
					if (img_src.empty()) {
						cerr << "Unable to load image file: " << info.img_file_name << endl;
						continue;
					} else {
						setImage(i, Mat(img_src, info.area));
					}
				}

				setID(i, id);
				setKeyPoints(i, keypoints);
				setDescriptor(i, descriptor);
				setName(i, name);
				setFileDetails(i, info);

				i++;
			}
		}

		void FileInfo::clear()
		{
			Info::clear();
			infomations.clear();
		}

		void FileInfo::resize(size_t size)
		{
			Info::resize(size);
			infomations.resize(size);
		}
	}
}
