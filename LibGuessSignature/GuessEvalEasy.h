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
			template<typename ArrayIn, typename ArrayOut>
			static ArrayOut convertArray(const ArrayIn& src)
			{
				ArrayOut out;
				for (ArrayIn::const_iterator it=src.begin(); it!=src.end(); it++)
					out.push_back(*it);
				return out;
			}

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

			virtual void train(const std::list<Image::Conclusive>& trains);
			virtual Image::Candidate::Assessments match(const Image::Candidate& query) const;
		};
	}
}
