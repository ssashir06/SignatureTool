#pragma once
#include <tuple>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <opencv2/ml/ml.hpp>
#include "GuessKMeans.h"

namespace Signature{
	namespace Guess {
		class SvmOneVsAll : public KMeansBase
		{
		protected:
			std::map<std::string, std::tuple<std::shared_ptr<CvSVM>, cv::Mat, cv::Mat, cv::Mat, cv::Mat> > svm_models_by_name;
			CvSVMParams params;
		public:
			SvmOneVsAll(void);
			SvmOneVsAll(unsigned int k, const CvSVMParams& svm_params = CvSVMParams());
			SvmOneVsAll(const SvmOneVsAll& src);
			SvmOneVsAll& operator=(const SvmOneVsAll& src);
			virtual ~SvmOneVsAll(void);

			virtual void setImages(const std::list<std::shared_ptr<Image::Base> >& images);
			virtual Result match(const cv::Mat& query) const;
		};
	}
}