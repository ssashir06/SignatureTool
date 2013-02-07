#pragma once
#include <tuple>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <fstream>
#include "LibSVM.h"
#include "GuessSvm.h"

namespace Signature{
	namespace Guess {
		class SvmOneVsAll : public SvmBase
		{
		protected:
			std::map<std::string, std::tuple<LibSVM::Model, LibSVM::Problem> > models_by_name;
			LibSVM::ScalingSetting scaling;
		public:
			SvmOneVsAll(void);
			SvmOneVsAll(unsigned int k, const LibSVM::Parameter& param = buildDefaultParam());
			SvmOneVsAll(const SvmOneVsAll& src);
			SvmOneVsAll& operator=(const SvmOneVsAll& src);
			virtual ~SvmOneVsAll(void);
			virtual SvmBase* clone() const;

			virtual void train(const std::list<Image::Conclusive >& images);
			virtual Image::Candidate::Assessments match(const Image::Candidate& query) const;

		protected:
			virtual void train();
		};
	}
}