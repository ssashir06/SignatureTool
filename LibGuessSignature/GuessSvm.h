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
			virtual SvmBase* clone() const = 0;

			void setBestParam(const std::list<Image::Conclusive >& images, unsigned int grid = 3);
			LibSVM::Parameter getParam() const;
			std::list<ParamScore> crossValidation(const std::list<Image::Conclusive >& images, unsigned int grid);

			virtual void train(const std::list<Image::Conclusive >& images) = 0;
		protected:
			virtual void train() = 0;
			static LibSVM::Parameter buildDefaultParam();
		};

#pragma region cv
		std::vector<LibSVM::NodeArray::Classified> buildClassfiedData(const Image::Descriptor& histgram, double label);
		LibSVM::NodeArray buildNodeArray(const Image::Descriptor& descriptor);
		LibSVM::ScalingSetting buildScalingSetting(const std::vector<std::pair<std::string, Image::Descriptor> >& histgrams_by_name);
#pragma endregion
	}
}
