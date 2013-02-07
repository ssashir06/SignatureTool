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
			Image::MatchingMachines machines;
			std::vector<std::pair<std::string, cv::Mat> > histgrams_by_name;
			cv::BOWImgDescriptorExtractor bowde;
			unsigned int k;
			static const unsigned int k_default = 1000;
		public:
			KMeansBase(void);
			KMeansBase(unsigned int k, const Image::MatchingMachines& machines = Image::MatchingMachines());
			KMeansBase(const KMeansBase& src);
			KMeansBase& operator=(const KMeansBase& src);
			virtual ~KMeansBase(void);

			virtual void train(const std::list<Image::Conclusive >& trains);
		protected:
			void makeVocaabularies(const std::list<Image::Conclusive >& trains);
			virtual Image::Descriptor getDescriptor(const Image::Base& image) const;
			cv::BOWImgDescriptorExtractor copyBOWDescriptrExtractor() const;
		};
	}
}
