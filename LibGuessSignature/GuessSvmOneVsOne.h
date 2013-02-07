#pragma once
#include <tuple>
#include <map>
#include <string>
#include <list>
#include "GuessSvm.h"

namespace Signature
{
	namespace Guess
	{
		class SvmOneVsOne : public SvmBase
		{
		protected:
			std::map<std::string, std::list<std::tuple<std::string, LibSVM::Model, LibSVM::Problem> > > models_by_name;
			LibSVM::ScalingSetting scaling;
		public:
			SvmOneVsOne(void);
			SvmOneVsOne(unsigned int k);
			SvmOneVsOne(const SvmOneVsOne& src);
			SvmOneVsOne& operator=(const SvmOneVsOne& src);
			virtual ~SvmOneVsOne(void);
			virtual SvmBase* clone() const;

			virtual void train(const std::list<Image::Conclusive >& trains);
			virtual Image::Candidate::Assessments match(const Image::Candidate& query) const;

		protected:
			virtual void train();
		};
	}
}
