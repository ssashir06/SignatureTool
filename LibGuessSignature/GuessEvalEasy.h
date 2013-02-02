#pragma once
#include <list>
#include "GuessSignature.h"

namespace Signature
{
	namespace Guess
	{
		class EvalEasy : public Base
		{
		protected:
			Image::Info info;
			static const int matching_count_weight_default = 100;
		public:
			double matching_count_weight;

		public:
			EvalEasy();
			EvalEasy(const EvalEasy& src);
			EvalEasy& operator=(const EvalEasy& src);
			virtual ~EvalEasy();

			const Image::Info& getInfo() const;
			virtual void train(const std::list<std::shared_ptr<Image::Base> >& trains);
			virtual Result match(const cv::Mat& query) const;
		};
	}
}
