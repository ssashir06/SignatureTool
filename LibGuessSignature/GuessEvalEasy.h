#pragma once
#include <vector>
#include "GuessSignature.h"

namespace Signature
{
	namespace Guess
	{
		class EvalEasy : public Base
		{
		protected:
			static const int matching_count_weight_default = 100;
			std::vector<Image::Conclusive> train_images;
		public:
			double matching_count_weight;
			Image::MatchingMachines machines;

		public:
			EvalEasy();
			EvalEasy(const EvalEasy& src);
			EvalEasy& operator=(const EvalEasy& src);
			virtual ~EvalEasy();

			virtual void train(const std::list<Image::Conclusive>& train, bool adding);
			virtual Image::Candidate::Assessments match(const Image::Candidate& query) const;

			virtual void saveModel(const std::string& file_name) const;
			virtual void loadModel(const std::string& file_name);
			virtual std::pair<std::string, std::string> modelSuffix() const;

			virtual void strip(bool remove_keypoints);
		};
	}
}
