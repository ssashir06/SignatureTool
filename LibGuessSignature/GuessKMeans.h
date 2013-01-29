#pragma once
#include <vector>
#include <list>
#include <string>
#include <map>
#include <opencv2/opencv.hpp>
#include "GuessSignature.h"

namespace Signature{
	namespace Guess {
		class KMeansBase : public Base
		{
		protected:
			std::map<std::string, cv::Mat> histgrams_by_name;
			cv::Mat vocabularies;
			unsigned int k;
			static const unsigned int k_default = 1000;
		public:
			KMeansBase(void);
			KMeansBase(unsigned int k);
			KMeansBase(const KMeansBase& src);
			KMeansBase& operator=(const KMeansBase& src);
			virtual ~KMeansBase(void);

			virtual void setImages(const std::list<std::shared_ptr<Image::Base> >& trains);
		protected:
			virtual cv::BOWImgDescriptorExtractor makeBOWImageDescriptorExtractor() const;
		};
	}
}
