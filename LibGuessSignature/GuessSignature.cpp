#include <memory>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/opencv.hpp>
#include "GuessSignature.h"
using namespace std;
using namespace cv;
using namespace CVUtil::ReadWrite;

namespace Signature
{
	namespace Image
	{
#pragma region MatchingMachines
		MatchingMachines::MatchingMachines()
			: detector(new SurfFeatureDetector()), extractor(new SurfDescriptorExtractor()), matcher(new FlannBasedMatcher())
		{
		}

		MatchingMachines::MatchingMachines(const Ptr<FeatureDetector>& detector, const Ptr<DescriptorExtractor>& extractor, const Ptr<DescriptorMatcher>& matcher)
			: detector(detector), extractor(extractor), matcher(matcher)
		{
		}

		MatchingMachines::MatchingMachines(const MatchingMachines& src)
			: detector(src.detector), extractor(src.extractor), matcher(src.matcher)
		{
		}

		MatchingMachines& MatchingMachines::operator=(const MatchingMachines& src)
		{
			detector = src.detector;
			extractor = src.extractor;
			matcher = src.matcher;
			return *this;
		}

		MatchingMachines::~MatchingMachines()
		{
		}

		Ptr<FeatureDetector> MatchingMachines::getDetector() const
		{
			return detector;
		}

		Ptr<DescriptorExtractor> MatchingMachines::getExtractor() const
		{
			return extractor;
		}

		Ptr<DescriptorMatcher> MatchingMachines::getMatcher() const
		{
			return matcher;
		}

		void MatchingMachines::save(FileStorage& fs) const
		{
			fs << "{";

			fs << "Detector" << "{";
			detector->write(fs);
			fs << "}";

			fs << "Extractor" << "{";
			extractor->write(fs);
			fs << "}";

			fs << "Matcher" << "{";
			matcher->write(fs);

			/// 不要．TrainImagesのDescriptorを利用する
			//fs << "TrainingDescriptors";
			//fs << matcher->getTrainDescriptors();

			fs << "}";

			fs << "}";
		}

		void MatchingMachines::load(FileNodeIterator& it)
		{
			const string node_name = (*it).name();

			if (node_name == "Detector")
			{
				detector->read(*it);
			}
			else if (node_name == "Extractor")
			{
				extractor->read(*it);
			}
			else if (node_name == "Matcher")
			{
				matcher->read(*it);

				/// 不要．TrainImagesのDescriptorを利用する
				//vector<Descriptor> training_descriptors;
				//(*it)["TrainingDescriptors"] >> training_descriptors;
				//matcher->add(training_descriptors);
			}
			else
			{
				throw exception(string("Unexpected node: " + node_name).c_str());
			}
		}
		
		void MatchingMachines::load(const FileNode& node)
		{
			for (FileNodeIterator it = node.begin(); it != node.end(); it++)
				load(it);
		}
#pragma endregion

#pragma region Base
		Base::Base()
			: keypoints(nullptr)
		{
		}

		Base::Base(const string& file_name)
			: file_name(file_name), keypoints(nullptr)
		{
		}

		Base::Base(const Mat& signature, const string& file_name)
			: signature(signature), file_name(file_name), keypoints(nullptr)
		{
		}

		Base::Base(const Base& src)
			: signature(src.signature), file_name(src.file_name), descriptor(src.descriptor), keypoints(src.keypoints)
		{
		}

		Base& Base::operator=(const Base& src)
		{
			signature = src.signature;
			file_name = src.file_name;
			descriptor = src.descriptor;
			keypoints = src.keypoints;
			return *this;
		}

		Base::~Base()
		{
		}

		Descriptor Base::getDescriptor()
		{
			if (!descriptor.empty()) return descriptor;

			machines.getExtractor()->compute(getImage(), *getKeyPoints(), descriptor);
			return descriptor;
		}

		Descriptor Base::getDescriptor() const
		{
			if (!descriptor.empty()) return descriptor;

			Descriptor descriptor;
			machines.getExtractor()->compute(getImage(), *getKeyPoints(), descriptor);
			return descriptor;
		}

		shared_ptr<KeyPoints> Base::getKeyPoints()
		{
			if (keypoints != nullptr && !keypoints->empty()) return keypoints;

			keypoints = shared_ptr<KeyPoints>(new KeyPoints());
			machines.getDetector()->detect(getImage(), *keypoints);
			return keypoints;
		}

		shared_ptr<KeyPoints> Base::getKeyPoints() const
		{
			if (keypoints != nullptr && !keypoints->empty()) return keypoints;

			shared_ptr<KeyPoints> keypoints = shared_ptr<KeyPoints>(new KeyPoints());
			machines.getDetector()->detect(getImage(), *keypoints);
			return keypoints;
		}

		Mat Base::getImage()
		{
			if (!signature.empty()) return signature;

			if (file_name.empty()) throw "unable to open image, file name is empty";
			Mat img = imread(file_name);
			if (img.empty()) throw "failed to open image";
			return signature = img;
		}

		Mat Base::getImage() const
		{
			if (!signature.empty()) return signature;

			if (file_name.empty()) throw "unable to open image, file name is empty";
			Mat img = imread(file_name);
			if (img.empty()) throw "failed to open image";
			return img;
		}

		void Base::strip()
		{
			if (file_name.size())
			{
				getKeyPoints();
				getDescriptor();
				signature = cv::Mat();
			}
		}

		void Base::saveData(cv::FileStorage& fs) const
		{
			fs << "KeyPoints" << *keypoints;
			fs << "Descriptor" << descriptor;
			fs << "ImageFileName" << file_name;
		}

		void Base::save(FileStorage& fs) const
		{
			fs << "{";
			saveData(fs);
			fs << "}";
		}

		void Base::load(FileNodeIterator& it)
		{
			string node_name = (*it).name();

			if (node_name == "KeyPoints")
			{
				if (!keypoints) keypoints = make_shared<KeyPoints>();
				else keypoints->clear();
				//(*it) >> *keypoints; //FIXME: compile error
				read(*it, *keypoints);
			}
			else if (node_name == "Descriptor")
			{
				(*it) >> descriptor;
			}
			else if (node_name == "ImageFileName")
			{
				(*it) >> file_name;
			}
		}

		void Base::load(const FileNode& node)
		{
			for (FileNodeIterator it = node.begin(); it != node.end(); it++)
			{
				load(it); // call virtual function
			}
		}
#pragma endregion

#pragma region Conclusive
		Conclusive::Conclusive() : Base()
		{
		}

		Conclusive::Conclusive(const string& name, const string& file_name)
			: Base(file_name), name(name)
		{
		}

		Conclusive::Conclusive(const Mat& signature, const string& name, const string& file_name)
			: Base(signature, file_name), name(name)
		{
		}

		Conclusive::Conclusive(const Base& src)
			: Base(src)
		{
		}

		Conclusive::Conclusive(const Conclusive& src)
			: Base(src), name(src.name)
		{
		}

		Conclusive& Conclusive::operator=(const Conclusive& src)
		{
			Base::operator=(src);
			name = src.name;
			return *this;
		}

		Conclusive::~Conclusive()
		{
		}

		void Conclusive::saveData(FileStorage& fs) const
		{
			Base::saveData(fs);
			fs << "Name" << name;
		}

		void Conclusive::load(FileNodeIterator& it)
		{
			Base::load(it);

			string node_name = (*it).name();
			if (node_name == "Name")
			{
				(*it) >> name;
			}
		}
#pragma endregion

#pragma region Candidate
#pragma region Candidate::Assessment
		Candidate::Assessment::Assessment()
		{
			score = 0;
		}

		Candidate::Assessment::Assessment(const Assessment& src)
			: score(src.score), name(src.name)
		{
		}

		Candidate::Assessment& Candidate::Assessment::operator=(const Assessment& src)
		{
			score = src.score;
			name = src.name;
			return *this;
		}

		bool Candidate::Assessment::operator<(const Candidate::Assessment& val) const
		{
			return score < val.score;
		}

		void Candidate::Assessment::save(FileStorage& fs) const
		{
			fs << "{";

			fs << "Name" << name;
			fs << "Score" << score;

			fs << "}";
		}

		void Candidate::Assessment::load(FileNodeIterator& it)
		{
			const string node_name = (*it).name();

			if (node_name == "Name")
			{
				(*it) >> name;
			}
			else if (node_name == "Score")
			{
				(*it) >> score;
			}
			else
			{
				throw exception(string("Unexpected node: " + node_name).c_str());
			}
		}

		void Candidate::Assessment::load(const FileNode& node)
		{
			for (FileNodeIterator it = node.begin(); it != node.end(); it++)
				load(it);
		}
#pragma endregion

		Candidate::Candidate() : Base()
		{
		}

		Candidate::Candidate(const string& file_name)
			: Base(file_name)
		{
		}

		Candidate::Candidate(const Mat& signature, const string& file_name)
			: Base(signature, file_name)
		{
		}

		Candidate::Candidate(list<Assessment> names, const string& file_name)
			: Base(file_name), names(names)
		{
		}

		Candidate::Candidate(const Mat& signature, list<Assessment> names, const string& file_name)
			: Base(signature, file_name), names(names)
		{
		}

		Candidate::Candidate(const Candidate& src)
			: Base(src), names(src.names)
		{
		}

		Candidate& Candidate::operator=(const Candidate& src)
		{
			Base::operator=(src);
			names = src.names;
			return *this;
		}

		Candidate::Candidate(const Conclusive& src)
			: Base(src)
		{
		}

		Candidate& Candidate::operator=(const Conclusive& src)
		{
			Base::operator=(src);
			return *this;
		}

		Candidate::operator Conclusive() const
		{
			list<Assessment> names_copy = names;
			names_copy.sort();
			Conclusive conclusive(*this);
			conclusive.name = names_copy.front().name;
			return conclusive;
		}

		Candidate::~Candidate()
		{
		}

		void Candidate::saveData(FileStorage& fs) const
		{
			Base::saveData(fs);
			fs << "AssessmentedNames" << convertArray<Assessments, list<Assessment::Capsule<Assessment> > >(names);
		}

		void Candidate::load(FileNodeIterator& it)
		{
			Base::load(it);

			string node_name = (*it).name();
			if (node_name == "AssessmentedNames")
			{
				list<Assessment::Capsule<Assessment> > assessment_capsules;
				(*it) >> assessment_capsules;
				names = convertArray<list<Assessment::Capsule<Assessment> >, Assessments>(assessment_capsules);
			}
		}
#pragma endregion
	}
	namespace Guess
	{
#pragma region Base
		Base::Base()
		{
		}

		Base::Base(const Base& src)
		{
		}

		Base& Base::operator=(const Base& src)
		{
			return *this;
		}

		Base::~Base()
		{
		}
		
		void Base::match(Image::Candidate& query) const
		{
			query.names = match((const Image::Candidate)query);
		}
		
		void Base::match(Image::Candidate& query, const Rect& trimming_area) const
		{
			Mat trimmed(query.getImage(), trimming_area);
			Image::Candidate query_trim(trimmed);
			match(query_trim);
			query.names = query_trim.names;
		}

		Image::Descriptor Base::getDescriptor(const Image::Base& image) const
		{
			return image.getDescriptor();
		}
#pragma endregion
	}
}
