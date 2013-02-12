#include <memory>
#include <opencv2/nonfree/features2d.hpp>
#include "GuessSignature.h"
using namespace std;
using namespace cv;

namespace Signature
{
	namespace Image
	{
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

		void Base::makeSmall()
		{
			if (file_name.size())
			{
				getKeyPoints();
				getDescriptor();
				signature = cv::Mat();
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
#pragma endregion

#pragma region Candidate
		Candidate::Assessment::Assessment()
		{
			score = 0;
		}

		bool Candidate::Assessment::operator<(const Candidate::Assessment& val) const
		{
			return score < val.score;
		}

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
#pragma endregion

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

		Image::Descriptor Base::getDescriptor(const Image::Base& image) const
		{
			return image.getDescriptor();
		}
#pragma endregion
	}
}
