#pragma once
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include "GuessSignature.h"

namespace Signature{
	namespace Guess {
		class KMeansBase : public Base
		{
		protected:
			std::vector<std::pair<std::string, cv::Mat> > histgrams_by_name;
			cv::Mat vocabularies;
			unsigned int k;
			static const unsigned int k_default = 1000;
		public:
			KMeansBase(void);
			KMeansBase(unsigned int k);
			KMeansBase(const KMeansBase& src);
			KMeansBase& operator=(const KMeansBase& src);
			virtual ~KMeansBase(void);

			virtual void train(const std::list<std::shared_ptr<Image::Base> >& trains);
			virtual void train(const std::string& file_name);
			void save(const std::string& file_name) const;
			void load(const std::string& file_name);
		protected:
			void makeVocaabularies(const std::list<std::shared_ptr<Image::Base> >& trains);
			virtual cv::BOWImgDescriptorExtractor makeBOWImageDescriptorExtractor() const;
		};
	}
}
