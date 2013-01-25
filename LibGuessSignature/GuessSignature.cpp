#include <memory>
#include "GuessSignature.h"
using namespace std;
using namespace cv;

namespace Signature
{
	namespace Image
	{
#pragma region Base
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

		Base::Base(const Base* src)
		{
			id = src->id;
			signature = src->signature;
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
#pragma endregion

#pragma region Conclusive
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

		string Conclusive::getName() const
		{
			return name;
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
			names_copy.sort();
			Conclusive conclusive(*this);
			conclusive.name = names_copy.front().name;
			return conclusive;
		}

		Candidate::~Candidate()
		{
		}

		string Candidate::getName() const
		{
			return "UNKNOWN";
		}
#pragma endregion

#pragma region MatchingMachines
		MatchingMachines::MatchingMachines()
		{
			type = default_type;
			make();
		}

		MatchingMachines::MatchingMachines(const string& type)
		{
			this->type = type;
			make();
		}

		MatchingMachines::MatchingMachines(const Ptr<FeatureDetector>& detector, const Ptr<DescriptorExtractor>& extractor, const std::string& type)
		{
			this->type = type;
			this->detector = detector;
			this->extractor = extractor;
		}

		MatchingMachines::MatchingMachines(const MatchingMachines& src)
		{
			type = src.type;
			detector = src.detector;
			extractor = src.extractor;
		}

		MatchingMachines& MatchingMachines::operator=(const MatchingMachines& src)
		{
			type = src.type;
			detector = src.detector;
			extractor = src.extractor;
			return *this;
		}

		MatchingMachines::~MatchingMachines()
		{
		}

		Ptr<cv::FeatureDetector> MatchingMachines::getDetector() const
		{
			return detector;
		}

		Ptr<cv::DescriptorExtractor> MatchingMachines::getExtractor() const
		{
			return extractor;
		}

		string MatchingMachines::getName() const
		{
			return type;
		}

		void MatchingMachines::make()
		{
			detector = FeatureDetector::create(type);
			extractor = DescriptorExtractor::create(type);
		}
#pragma endregion

#pragma region Info
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
		void Info::prepare(const list<shared_ptr<Base> >& signatures, const MatchingMachines& machines)
		{
			resize(signatures.size());
			int i = 0;
			for (list<shared_ptr<Base> >::const_iterator it=signatures.begin(); it!=signatures.end(); it++, i++) {
				addSignature(**it, i);
			}
			machines.getDetector()->detect(images, keypoints);
			machines.getExtractor()->compute(images, keypoints, descriptors);
		}

		void Info::addSignature(const Base& signature, int idx)
		{
			if (size() <= idx) throw "bad idx";
			setID(idx, signature.id);
			setImage(idx, signature.signature);
			setName(idx, signature.getName());
		}

		void Info::addSignature(const Base& signature, const MatchingMachines& machines)
		{
			Idx idx = size();
			resize(idx);
			addSignature(signature, size());
			machines.getDetector()->detect(getImage(idx), getKeyPoints(idx));
			machines.getExtractor()->compute(getImage(idx), getKeyPoints(idx), getDescriptor(idx));
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
			info = src.info;
			machines = src.machines;
		}

		Base& Base::operator=(const Base& src)
		{
			info = src.info;
			machines = src.machines;
			return *this;
		}

		Base::~Base()
		{
		}

		const Image::Info& Base::getInfo() const
		{
			return info;
		}
		
		void Base::setMatchingMachine(const std::string& type)
		{
			machines = Image::MatchingMachines(type);
		}

		void Base::setMatchingMachine(const Image::MatchingMachines& machines)
		{
			this->machines = machines;
		}

		void Base::buildInfo(const list<shared_ptr<Image::Base> >& images)
		{
			info.prepare(images, machines);
		}
#pragma endregion
	}
}
