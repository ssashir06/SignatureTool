#include <opencv2/opencv.hpp>
#include "SaveLoadCV.h"

using namespace cv;

namespace CVUtil
{
	namespace ReadWrite
	{
		cv::FileStorage& operator<<(cv::FileStorage& fs, const ICVSaveLoad& c)
		{
			if (!fs.isOpened()) return fs;
			c.save(fs);
			return fs;
		}

		cv::FileNodeIterator& operator>>(cv::FileNodeIterator& it, ICVSaveLoad& c)
		{
			c.load(it);
			return it;
		}

		void operator>>(const cv::FileNode& fn, ICVSaveLoad& c)
		{
			c.load(fn);
		}
	}
}