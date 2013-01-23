#pragma once
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <map>
#include <opencv2/opencv.hpp>

namespace Signature
{
	template<typename ArrayIn, typename ArrayOut>
	ArrayOut convertArray(const ArrayIn& src)
	{
		ArrayOut out;
		for (ArrayIn::const_iterator it=src.begin(); it!=src.end(); it++)
			out.push_back(*it);
		return out;
	}

	namespace Image
	{
		typedef int ID;
		struct Base
		{
			ID id;
			cv::Mat signature;
			Base();
			Base(ID id, const cv::Mat& signature);
			Base(const Base& src);
			Base& operator=(const Base& src);
			virtual ~Base();
		};
		struct Conclusive : public Base
		{
			std::string name;
			Conclusive();
			Conclusive(ID id, const cv::Mat& signature, const std::string& name);
			Conclusive(const Base& src);
			Conclusive(const Conclusive& src);
			Conclusive& operator=(const Conclusive& src);
			operator Base() const;
			virtual ~Conclusive();
		};
		struct Candidate : public Base
		{
			struct Assessment
			{
				std::string name;
				double score;
				Assessment();
				bool operator<(const Assessment& val) const;
			};
			std::list<Assessment> names;
			Candidate();
			Candidate(ID id, const cv::Mat& signature);
			Candidate(ID id, const cv::Mat& signature, std::list<Assessment> names);
			Candidate(const Candidate& src);
			Candidate& operator=(const Candidate& src);
			operator Conclusive() const;
			virtual ~Candidate();
		};
		class Info
		{
		protected:
			typedef std::vector<cv::KeyPoint> KeyPoints;
			typedef cv::Mat Descriptor;
			typedef int Idx;
			std::vector<ID> ids;
			std::vector<cv::Mat> images;
			std::vector<KeyPoints> keypoints;
			std::vector<Descriptor> descriptors;
			std::vector<std::string> names;
		public:
			Info();
			Info(const Info& src);
			Info& operator=(const Info& src);
			virtual ~Info();

			virtual void prepare(const std::list<Conclusive>& signatures, const cv::FeatureDetector& detector, const cv::DescriptorExtractor& extractor);//FIXME
			virtual void prepare(const std::list<Candidate>& signatures, const cv::FeatureDetector& detector, const cv::DescriptorExtractor& extractor);//FIXME
			virtual void addSignature(const Conclusive& signature, const cv::FeatureDetector& detector, const cv::DescriptorExtractor& extractor);//FIXME
			virtual void addSignature(const Candidate& signature, const cv::FeatureDetector& detector, const cv::DescriptorExtractor& extractor);//FIXME

			ID getID(Idx idx) const;
			cv::Mat getImage(Idx idx) const;
			KeyPoints getKeyPoints(Idx idx) const;
			Descriptor getDescriptor(Idx idx) const;
			std::vector<Descriptor> getDescriptors() const;
			std::string getName(Idx idx) const;

			size_t size() const;
			virtual void clear();
			virtual void resize(size_t size);
		protected:
			void setID(Idx idx, ID id);
			void setImage(Idx idx, const cv::Mat& image, bool make_clone = false);
			void setKeyPoints(Idx idx, const KeyPoints& keypoints);
			void setDescriptor(Idx idx, const Descriptor& descriptor);
			void setName(Idx idx, const std::string& name);

			void addSignature(const Conclusive& signature, int idx);//FIXME
			void addSignature(const Candidate& signature, int idx);//FIXME
		};
	}

	namespace Guess
	{
		typedef std::list<Image::Candidate::Assessment> Result;
		class Base
		{
		protected:
			Image::Info info;
			cv::Ptr<cv::FeatureDetector> detector;
			cv::Ptr<cv::DescriptorExtractor> extractor;
		public:
			Base();
			Base(const Base& src);
			Base& operator=(const Base& src);
			virtual ~Base();

			const Image::Info& getInfo() const;
			void setMatchingMachine(cv::Ptr<cv::FeatureDetector>& detector, cv::Ptr<cv::DescriptorExtractor>& extractor);
			virtual void buildInfo(const std::list<Image::Conclusive>& images);
			virtual void buildInfo(const std::list<Image::Candidate>& images);
			virtual Result match(const Image::Info& query, int idx) = 0;
		};
	}
}
