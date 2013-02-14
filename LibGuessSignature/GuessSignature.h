#pragma once
#include <memory>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <ostream>
#include <map>
#include <opencv2/opencv.hpp>

namespace Signature
{
	namespace Image
	{
		typedef std::vector<cv::KeyPoint> KeyPoints;
		typedef cv::Mat Descriptor;

		class MatchingMachines
		{
		protected:
			cv::Ptr<cv::FeatureDetector> detector;
			cv::Ptr<cv::DescriptorExtractor> extractor;
			cv::Ptr<cv::DescriptorMatcher> matcher;

		public:
			MatchingMachines();// default is SURF.
			MatchingMachines(const cv::Ptr<cv::FeatureDetector>& detector, const cv::Ptr<cv::DescriptorExtractor>& extractor, const cv::Ptr<cv::DescriptorMatcher>& matcher = new cv::FlannBasedMatcher());
			MatchingMachines(const MatchingMachines& src);
			MatchingMachines& operator=(const MatchingMachines& src);
			virtual ~MatchingMachines();

			Descriptor calcurateDescriptor(const cv::Mat& image) const;

		public:
			cv::Ptr<cv::FeatureDetector> getDetector() const;
			cv::Ptr<cv::DescriptorExtractor> getExtractor() const;
			cv::Ptr<cv::DescriptorMatcher> getMatcher() const;
		};
		class Base
		{
		protected:
			std::shared_ptr<KeyPoints> keypoints;
			Descriptor descriptor;
			std::string file_name;
			cv::Mat signature;//a image of written signature
		public:
			MatchingMachines machines;
		public:
			Base();
			Base(const std::string& file_name);
			Base(const cv::Mat& signature, const std::string& file_name = std::string());
			Base(const Base& src);
			Base& operator=(const Base& src);
			virtual ~Base();

			Descriptor getDescriptor();
			Descriptor getDescriptor() const;
			std::shared_ptr<KeyPoints> getKeyPoints();
			std::shared_ptr<KeyPoints> getKeyPoints() const;
			virtual cv::Mat getImage();
			virtual cv::Mat getImage() const;

			void makeSmall();
		};
		class Conclusive : public Base
		{
		public:
			std::string name;
			Conclusive();
			Conclusive(const std::string& name, const std::string& file_name);
			Conclusive(const cv::Mat& signature, const std::string& name, const std::string& file_name = std::string());
			Conclusive(const Base& src);
			Conclusive(const Conclusive& src);
			Conclusive& operator=(const Conclusive& src);
			virtual ~Conclusive();
		};
		class Candidate : public Base
		{
		public:
			struct Assessment
			{
				std::string name;
				double score;
				Assessment();
				bool operator<(const Assessment& val) const;
			};
			typedef std::list<Assessment> Assessments;
			Assessments names;
			Candidate();
			Candidate(const std::string& file_name);
			Candidate(const cv::Mat& signature, const std::string& file_name = std::string());
			Candidate(std::list<Assessment> names, const std::string& file_name);
			Candidate(const cv::Mat& signature, std::list<Assessment> names, const std::string& file_name = std::string());
			Candidate(const Candidate& src);
			Candidate& operator=(const Candidate& src);
			Candidate(const Conclusive& src);
			Candidate& operator=(const Conclusive& src);
			operator Conclusive() const;
			virtual ~Candidate();
		};
	}
	namespace Guess
	{
		class Base
		{
		public:
			Base();
			Base(const Base& src);
			Base& operator=(const Base& src);
			virtual ~Base();

			virtual void train(const std::list<Image::Conclusive >& trains) = 0;
			virtual Image::Candidate::Assessments match(const Image::Candidate& query) const = 0;
			virtual void match(Image::Candidate& query) const;
		protected:
			virtual Image::Descriptor getDescriptor(const Image::Base& image) const;
		};
	}
}
