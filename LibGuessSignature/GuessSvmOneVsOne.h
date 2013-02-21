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
			std::list<Image::Conclusive> train_images;
		public:
			SvmOneVsOne(void);
			SvmOneVsOne(unsigned int k);
			SvmOneVsOne(const SvmOneVsOne& src);
			SvmOneVsOne& operator=(const SvmOneVsOne& src);
			virtual ~SvmOneVsOne(void);
			virtual SvmBase* clone() const;

			virtual void train(const std::list<Image::Conclusive >& images, bool adding);
			virtual Image::Candidate::Assessments match(const Image::Candidate& query) const;

			virtual void saveModel(const std::string& file_name) const;
			virtual void loadModel(const std::string& file_name);
			virtual std::pair<std::string, std::string> modelSuffix() const;

			virtual void strip(bool remove_keypoints);
		protected:
			virtual void train();
		};
	}
}
