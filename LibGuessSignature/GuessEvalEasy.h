#pragma once
#include <list>
#include "GuessSignature.h"

namespace Signature
{
	namespace Guess
	{
		class Info
		{
		public:
			typedef int Idx;
		protected:
			std::vector<cv::Mat> images;
			std::vector<Image::KeyPoints> keypoints;
			std::vector<Image::Descriptor> descriptors;
			std::vector<std::string> names;
		public:
			Info();
			Info(const Info& src);
			Info& operator=(const Info& src);
			virtual ~Info();

			void prepare(const std::list<std::shared_ptr<Image::Base> >& signatures, const Image::MatchingMachines& machines);
			virtual void addSignature(const Image::Base& signature, const Image::MatchingMachines& machines);

			cv::Mat getImage(Idx idx) const;
			Image::KeyPoints getKeyPoints(Idx idx) const;
			Image::Descriptor getDescriptor(Idx idx) const;
			std::vector<Image::Descriptor> getDescriptors() const;
			std::string getName(Idx idx) const;

			size_t count() const;
			virtual void clear();
			virtual void resize(size_t size);
		protected:
			void setImage(Idx idx, const cv::Mat& image, bool make_clone = false);
			void setKeyPoints(Idx idx, const Image::KeyPoints& keypoints);
			void setDescriptor(Idx idx, const Image::Descriptor& descriptor);
			void setName(Idx idx, const std::string& name);

			void addSignature(const Image::Base& signature, int idx);
		};
		class EvalEasy : public Base
		{
		protected:
			Info info;
			static const int matching_count_weight_default = 100;
		public:
			double matching_count_weight;

		public:
			EvalEasy();
			EvalEasy(const EvalEasy& src);
			EvalEasy& operator=(const EvalEasy& src);
			virtual ~EvalEasy();

			const Info& getInfo() const;
			virtual void train(const std::list<std::shared_ptr<Image::Conclusive> >& trains);
			virtual Image::Candidate::Assessments match(const Image::Candidate& query) const;
		};
	}
}
