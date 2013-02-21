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
			std::list<Image::Conclusive> train_images;
		public:
			SvmOneVsAll(void);
			SvmOneVsAll(unsigned int k, const LibSVM::Parameter& param = buildDefaultParam());
			SvmOneVsAll(const SvmOneVsAll& src);
			SvmOneVsAll& operator=(const SvmOneVsAll& src);
			virtual ~SvmOneVsAll(void);
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