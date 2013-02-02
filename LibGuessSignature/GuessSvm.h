#pragma once
#include <tuple>
#include <list>
#include "LibSVM.h"
#include "GuessKMeans.h"

namespace Signature{
	namespace Guess {
		class SvmBase : public KMeansBase
		{
		protected:
			typedef std::pair<double, LibSVM::Parameter> ParamScore;
		protected:
			LibSVM::Parameter param;
		public:
			SvmBase(void);
			SvmBase(unsigned int k, const LibSVM::Parameter& param = buildDefaultParam());
			SvmBase(const SvmBase& src);
			SvmBase& operator=(const SvmBase& src);
			virtual ~SvmBase(void);

			void setBestParam(const std::list<std::shared_ptr<Image::Base> >& images, unsigned int grid = 3);
			LibSVM::Parameter getParam() const;
			std::list<ParamScore> crossValidation(const std::list<std::shared_ptr<Image::Base> >& images, unsigned int grid);
			
			virtual void train(const std::list<std::shared_ptr<Image::Base> >& images) = 0;
		protected:
			virtual void train() = 0;
			static LibSVM::Parameter buildDefaultParam();
		};

#pragma region cv
		std::vector<LibSVM::NodeArray::Classified> buildClassfiedData(const cv::Mat& histgram, double label);
		LibSVM::NodeArray buildNodeArray(const cv::Mat& descriptor);
		LibSVM::ScalingSetting buildScalingSetting(const std::map<std::string, cv::Mat>& histgrams_by_name);
#pragma endregion
	}
}
