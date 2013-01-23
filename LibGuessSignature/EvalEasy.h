#pragma once
#include <memory>
#include <list>
#include "GuessSignature.h"

namespace Signature
{
	namespace Guess
	{
		class EvalEasy : public Base
		{
		protected:
			std::shared_ptr<cv::DescriptorMatcher> matcher;
			static const int matching_count_weight_default = 100;
		public:
			double matching_count_weight;

		public:
			EvalEasy();
			EvalEasy(const EvalEasy& src);
			EvalEasy& operator=(const EvalEasy& src);
			virtual ~EvalEasy();
			virtual void buildInfo(const std::list<Image::Conclusive>& images);
			virtual void buildInfo(const std::list<Image::Candidate>& images);
			virtual Result match(const Image::Info& query, int idx);
		};
	}
}
