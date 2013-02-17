#pragma once
#include <memory>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <ostream>
#include <map>
#include <opencv2/opencv.hpp>
#include "SaveLoadCV.h"

namespace Signature
{
	namespace Image
	{
		typedef std::vector<cv::KeyPoint> KeyPoints;
		typedef cv::Mat Descriptor;
		class MatchingMachines : public CVUtil::ReadWrite::ICVSaveLoad
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

		public:
			//CVUtil::ReadWrite::ICVSaveLoad
			virtual void save(cv::FileStorage& fs) const;
			virtual void load(cv::FileNodeIterator& it);
			virtual void load(const cv::FileNode& node);
			//virtual operator ICVSaveLoad::Capsule();
			//virtual operator const ICVSaveLoad::Capsule() const;
		};
		class Base : public CVUtil::ReadWrite::ICVSaveLoad
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

			void strip();

		public:
			virtual void saveData(cv::FileStorage& fs) const;
			//CVUtil::ReadWrite::ICVSaveLoad
			virtual void save(cv::FileStorage& fs) const;
			virtual void load(cv::FileNodeIterator& it);
			virtual void load(const cv::FileNode& node);
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

		public:
			virtual void saveData(cv::FileStorage& fs) const;
			//CVUtil::ReadWrite::ICVSaveLoad
			virtual void load(cv::FileNodeIterator& it);
			operator ICVSaveLoad::Capsule<Conclusive>() { return ICVSaveLoad::Capsule<Conclusive>((ICVSaveLoad*)this); } //TODO
			operator const ICVSaveLoad::Capsule<Conclusive>() const { return ICVSaveLoad::Capsule<Conclusive>((ICVSaveLoad*)this); } //TODO
		};
		class Candidate : public Base
		{
		public:
			class Assessment : public CVUtil::ReadWrite::ICVSaveLoad
			{
			public:
				std::string name;
				double score;
				Assessment();
				Assessment(const Assessment& src);
				Assessment& operator=(const Assessment& src);
				bool operator<(const Assessment& val) const;

			public:
				//CVUtil::ReadWrite::ICVSaveLoad
				virtual void save(cv::FileStorage& fs) const;
				virtual void load(cv::FileNodeIterator& it);
				virtual void load(const cv::FileNode& node);
				operator ICVSaveLoad::Capsule<Assessment>() { return ICVSaveLoad::Capsule<Assessment>((ICVSaveLoad*)this); } //TODO
				operator const ICVSaveLoad::Capsule<Assessment>() const { return ICVSaveLoad::Capsule<Assessment>((ICVSaveLoad*)this); } //TODO
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

		public:
			virtual void saveData(cv::FileStorage& fs) const;
			//CVUtil::ReadWrite::ICVSaveLoad
			virtual void load(cv::FileNodeIterator& it);
			operator ICVSaveLoad::Capsule<Candidate>() { return ICVSaveLoad::Capsule<Candidate>((ICVSaveLoad*)this); } //TODO
			operator const ICVSaveLoad::Capsule<Candidate>() const { return ICVSaveLoad::Capsule<Candidate>((ICVSaveLoad*)this); } //TODO
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

			virtual void saveModel(const std::string& file_name) const = 0;
			virtual void loadModel(const std::string& file_name) = 0;
		protected:
			virtual Image::Descriptor getDescriptor(const Image::Base& image) const;
		};
	}
}