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
			: keypoints(nullptr), grayscale(false), monochrome(false), monochrome_threshold(192)
		{
		}

		Base::Base(const string& file_name, const shared_ptr<Rect>& rect)
			: file_name(file_name), keypoints(nullptr), trimming(rect), grayscale(false), monochrome(false), monochrome_threshold(192)
		{
		}

		Base::Base(const Mat& signature, const string& file_name, const shared_ptr<Rect>& rect)
			: signature(signature), file_name(file_name), keypoints(nullptr), trimming(rect), grayscale(false), monochrome(false), monochrome_threshold(192)
		{
		}

		Base::Base(const Base& src)
			: 
				signature(src.signature), file_name(src.file_name), descriptor(src.descriptor), keypoints(src.keypoints), trimming(src.trimming), 
				grayscale(src.grayscale), monochrome(src.monochrome), monochrome_threshold(src.monochrome_threshold),
				machines(src.machines)
				//machines()
		{
		}

		Base& Base::operator=(const Base& src)
		{
			signature = src.signature;
			file_name = src.file_name;
			descriptor = src.descriptor;
			keypoints = src.keypoints;
			trimming = src.trimming;
			grayscale = src.grayscale;
			monochrome = src.monochrome;
			monochrome_threshold = src.monochrome_threshold;
			machines = src.machines;
			//machines = MatchingMachines();
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

			signature = ((const Base*)this)->getImage();
			return signature;
		}

		Mat Base::getImage() const
		{
			if (!signature.empty()) return signature;

			if (file_name.empty()) throw exception("unable to open image, file name is empty");
			Mat img = imread(file_name, (grayscale || monochrome)?0:1);
			if (img.empty()) throw exception("failed to open image");

#ifdef _DEBUG
			cout << "Image file is loaded: " << file_name << "\t(" << img.cols << " x " << img.rows << ")" << endl;
#endif

			if (trimming) {
				Rect r(*trimming);
				if (r.x < 0) { r.width += r.x; r.x = 0; }
				if (r.y < 0) { r.height += r.y; r.y = 0; }
				if (r.x + r.width > img.cols) r.width = img.cols - r.x;
				if (r.y + r.height > img.rows) r.height = img.rows - r.y;

#ifdef _DEBUG
				cout << "Original trimming rect is " << *trimming << endl;
				cout << "Resized trimming rect is " << r << endl;
#endif
				if (r.x < 0 || r.y < 0 || r.width < 0 || r.height < 0) throw exception("Trimming rect is wrong.");
				if (r.width == 0 || r.height == 0) throw exception("Trimming rect size is zero.");

				img = Mat(img, r).clone();
			}
			if (monochrome) threshold(img, img, monochrome_threshold, 255, THRESH_BINARY);

			return img;
		}

		void Base::strip(bool remove_keypoints)
		{
			if (file_name.size())
			{
				getKeyPoints();
				getDescriptor();
				signature = cv::Mat();
				if (remove_keypoints) keypoints = shared_ptr<KeyPoints>(nullptr);
			}
		}

		void Base::saveData(cv::FileStorage& fs) const
		{
			if (keypoints) fs << "KeyPoints" << *keypoints;
			fs << "Descriptor" << descriptor;
			fs << "ImageFileName" << file_name;
			if (trimming) fs << "TrimmingRectangle" << *trimming;
			fs << "Grayscale" << grayscale;
			fs << "Monochrome" << monochrome;
			fs << "MonochromeThreshold" << (int)monochrome_threshold;
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
			else if (node_name == "TrimmingRectangle")
			{
				vector<int> r;
				(*it) >> r;
				trimming = make_shared<Rect>(r[0], r[1], r[2], r[3]);
			}
			else if (node_name == "Grayscale")
			{
				(*it) >> grayscale;
			}
			else if (node_name == "Monochrome")
			{
				(*it) >> monochrome;
			}
			else if (node_name == "MonochromeThreshold")
			{
				int t;
				(*it) >> t;
				monochrome_threshold = (unsigned char)t;
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

		Conclusive::Conclusive(const string& name, const string& file_name, const shared_ptr<Rect>& rect)
			: Base(file_name, rect), name(name)
		{
		}

		Conclusive::Conclusive(const Mat& signature, const string& name, const string& file_name, const shared_ptr<Rect>& rect)
			: Base(signature, file_name, rect), name(name)
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

		Candidate::Candidate(const string& file_name, const shared_ptr<Rect>& rect)
			: Base(file_name, rect)
		{
		}

		Candidate::Candidate(const Mat& signature, const string& file_name, const shared_ptr<Rect>& rect)
			: Base(signature, file_name, rect)
		{
		}

		Candidate::Candidate(list<Assessment> names, const string& file_name, const shared_ptr<Rect>& rect)
			: Base(file_name, rect), names(names)
		{
		}

		Candidate::Candidate(const Mat& signature, list<Assessment> names, const string& file_name, const shared_ptr<Rect>& rect)
			: Base(signature, file_name, rect), names(names)
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

		Image::Candidate::Assessments Base::match(const Image::Candidate& query, const Rect& trimming_area) const
		{
			Mat trimmed(query.getImage(), trimming_area);
			Image::Candidate query_trim(trimmed);
			return match((const Image::Candidate)query_trim);
		}

		Image::Descriptor Base::getDescriptor(const Image::Base& image) const
		{
			return image.getDescriptor();
		}
#pragma endregion
	}
}
