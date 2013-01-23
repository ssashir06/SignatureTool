#include "GuessSignature.h"
using namespace std;
using namespace cv;

namespace Signature
{
	namespace Image
	{
		Base::Base()
		{
			id = 0;
		}

		Base::Base(ID id, const Mat& signature)
		{
			this->id = id;
			this->signature = signature;
		}

		Base::Base(const Base& src)
		{
			id = src.id;
			signature = src.signature;
		}

		Base& Base::operator=(const Base& src)
		{
			id = src.id;
			signature = src.signature;
			return *this;
		}

		Base::~Base()
		{
		}

		Conclusive::Conclusive() : Base()
		{
		}

		Conclusive::Conclusive(ID id, const Mat& signature, const string& name) : Base(id, signature)
		{
			this->name = name;
		}

		Conclusive::Conclusive(const Base& src) : Base(src)
		{
		}

		Conclusive::Conclusive(const Conclusive& src) : Base(src)
		{
			name = src.name;
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

		Candidate::Candidate(ID id, const Mat& signature) : Base(id, signature)
		{
		}

		Candidate::Candidate(ID id, const Mat& signature, list<Assessment> names) : Base(id, signature)
		{
			this->names = names;
		}

		Candidate::Candidate(const Candidate& src) : Base(src)
		{
			names = src.names;
		}

		Candidate& Candidate::operator=(const Candidate& src)
		{
			Base::operator=(src);
			names = src.names;
			return *this;
		}

		Candidate::operator Conclusive() const
		{
			list<Assessment> names_copy = names;
			names_copy.sort();//FIXME
			Conclusive conclusive((Base)*this);
			conclusive.name = names_copy.front().name;
			return conclusive;
		}

		Candidate::~Candidate()
		{
		}

		Info::Info()
		{
		}

		Info::Info(const Info& src)
		{
			ids = src.ids;
			images = src.images;
			keypoints = src.keypoints;
			descriptors = src.descriptors;
		}

		Info& Info::operator=(const Info& src)
		{
			ids = src.ids;
			images = src.images;
			keypoints = src.keypoints;
			descriptors = src.descriptors;
			return *this;
		}

		Info::~Info()
		{
		}

#pragma region Set signature(s)
		void Info::prepare(const list<Conclusive>& signatures, const FeatureDetector& detector, const DescriptorExtractor& extractor)
		{
			resize(signatures.size());
			int i = 0;
			for (list<Conclusive>::const_iterator it=signatures.begin(); it!=signatures.end(); it++, i++) {
				addSignature(*it, i);
			}
			detector.detect(images, keypoints);
			extractor.compute(images, keypoints, descriptors);
		}

		void Info::prepare(const list<Candidate>& signatures, const FeatureDetector& detector, const DescriptorExtractor& extractor)
		{
			resize(signatures.size());
			int i = 0;
			for (list<Candidate>::const_iterator it=signatures.begin(); it!=signatures.end(); it++, i++) {
				addSignature(*it, i);
			}
			detector.detect(images, keypoints);
			extractor.compute(images, keypoints, descriptors);
		}

		void Info::addSignature(const Conclusive& signature, int idx)
		{
			if (size() <= idx) throw "bad idx";
			setID(idx, signature.id);
			setImage(idx, signature.signature);
			setName(idx, signature.name);
		}

		void Info::addSignature(const Candidate& signature, int idx)
		{
			if (size() <= idx) throw "bad idx";
			setID(idx, signature.id);
			setImage(idx, signature.signature);
			setName(idx, "Unknown");
		}

		void Info::addSignature(const Conclusive& signature, const FeatureDetector& detector, const DescriptorExtractor& extractor)
		{
			Idx idx = size();
			resize(idx);
			addSignature(signature, size());
			detector.detect(getImage(idx), getKeyPoints(idx));
			extractor.compute(getImage(idx), getKeyPoints(idx), getDescriptor(idx));
		}

		void Info::addSignature(const Candidate& signature, const FeatureDetector& detector, const DescriptorExtractor& extractor)
		{
			Idx idx = size();
			resize(idx);
			addSignature(signature, size());
			detector.detect(getImage(idx), getKeyPoints(idx));
			extractor.compute(getImage(idx), getKeyPoints(idx), getDescriptor(idx));
		}
#pragma endregion

#pragma region Get/Set
		ID Info::getID(int idx) const
		{
			if (idx < 0 || size() <= idx) throw "Invalid idx";
			return ids[idx];
		}

		Mat Info::getImage(int idx) const
		{
			if (idx < 0 || size() <= idx) throw "Invalid idx";
			return images[idx];
		}

		Info::KeyPoints Info::getKeyPoints(int idx) const
		{
			if (idx < 0 || size() <= idx) throw "Invalid idx";
			return keypoints[idx];
		}

		Info::Descriptor Info::getDescriptor(int idx) const
		{
			if (idx < 0 || ids.size() <= idx) throw "Invalid idx";
			return descriptors[idx];
		}

		vector<Info::Descriptor> Info::getDescriptors() const
		{
			return descriptors;
		}

		string Info::getName(int idx) const
		{
			if (idx < 0 || ids.size() <= idx) throw "Invalid idx";
			return names[idx];
		}

		void Info::setID(Idx idx, ID id)
		{
			if (idx < 0 || ids.size() <= idx) throw "Invalid idx";
			ids[idx] = id;
		}

		void Info::setImage(Idx idx, const cv::Mat& image, bool make_clone)
		{
			if (idx < 0 || ids.size() <= idx) throw "Invalid idx";
			images[idx] = make_clone ? image.clone() : image;
		}

		void Info::setKeyPoints(Idx idx, const KeyPoints& keypoints)
		{
			if (idx < 0 || ids.size() <= idx) throw "Invalid idx";
			(this->keypoints)[idx] = keypoints;
		}

		void Info::setDescriptor(Idx idx, const Descriptor& descriptor)
		{
			if (idx < 0 || ids.size() <= idx) throw "Invalid idx";
			descriptors[idx] = descriptor;
		}

		void Info::setName(Idx idx, const std::string& name)
		{
			if (idx < 0 || ids.size() <= idx) throw "Invalid idx";
			names[idx] = name;
		}
#pragma endregion

		size_t Info::size() const
		{
			return ids.size();
		}

		void Info::clear()
		{
			ids.clear();
			images.clear();
			keypoints.clear();
			descriptors.clear();
			names.clear();
		}

		void Info::resize(size_t size)
		{
			ids.resize(size);
			images.resize(size);
			keypoints.resize(size);
			descriptors.resize(size);
			names.resize(size);
		}
	}
	namespace Guess
	{
		Base::Base()
		{
		}

		Base::Base(const Base& src)
		{
			info = src.info;
			detector = src.detector;
			extractor = src.extractor;
		}

		Base& Base::operator=(const Base& src)
		{
			info = src.info;
			detector = src.detector;
			extractor = src.extractor;
			return *this;
		}

		Base::~Base()
		{
		}

		const Image::Info& Base::getInfo() const
		{
			return info;
		}

		void Base::setMatchingMachine(Ptr<FeatureDetector>& detector, Ptr<DescriptorExtractor>& extractor)
		{
			this->detector = detector;
			this->extractor = extractor;
		}

		void Base::buildInfo(const list<Image::Conclusive>& images)
		{
			info.prepare(images, *detector, *extractor);
		}

		void Base::buildInfo(const list<Image::Candidate>& images)
		{
			info.prepare(images, *detector, *extractor);
		}
	}
}
