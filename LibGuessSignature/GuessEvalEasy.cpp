#include <iostream>
#include <map>
#include <opencv2/opencv.hpp>
#include "GuessEvalEasy.h"
using namespace std;
using namespace cv;

namespace Signature
{
	namespace Guess
	{
#pragma region Info
		Info::Info()
		{
		}

		Info::Info(const Info& src)
		{
			images = src.images;
			keypoints = src.keypoints;
			descriptors = src.descriptors;
		}

		Info& Info::operator=(const Info& src)
		{
			images = src.images;
			keypoints = src.keypoints;
			descriptors = src.descriptors;
			return *this;
		}

		Info::~Info()
		{
		}

#pragma region Set signature(s)
		void Info::prepare(const list<shared_ptr<Image::Base> >& signatures, const Image::MatchingMachines& machines)
		{
			resize(signatures.size());
			int i = 0;
			for (list<shared_ptr<Image::Base> >::const_iterator it=signatures.begin(); it!=signatures.end(); it++, i++) {
				addSignature(**it, i);
			}
			machines.getDetector()->detect(images, keypoints);
			machines.getExtractor()->compute(images, keypoints, descriptors);
		}

		void Info::addSignature(const Image::Base& signature, int idx)
		{
			if (count() <= idx) throw "bad idx";
			setImage(idx, signature.getImage());
			setName(idx, signature.getName());
		}

		void Info::addSignature(const Image::Base& signature, const Image::MatchingMachines& machines)
		{
			Idx idx = count();
			resize(idx);
			addSignature(signature, count());

			Image::KeyPoints keypoints;
			Image::Descriptor descriptor;

			machines.getDetector()->detect(getImage(idx), keypoints);
			machines.getExtractor()->compute(getImage(idx), keypoints, descriptor);

			setKeyPoints(idx, keypoints);
			setDescriptor(idx, descriptor);
		}
#pragma endregion

#pragma region Get/Set
		Image::ID Info::getID(int idx) const
		{
			if (idx < 0 || count() <= idx) throw "Invalid idx";
			return ids[idx];
		}

		Mat Info::getImage(int idx) const
		{
			if (idx < 0 || count() <= idx) throw "Invalid idx";
			return images[idx];
		}

		Image::KeyPoints Info::getKeyPoints(int idx) const
		{
			if (idx < 0 || count() <= idx) throw "Invalid idx";
			return keypoints[idx];
		}

		Image::Descriptor Info::getDescriptor(int idx) const
		{
			if (idx < 0 || ids.size() <= idx) throw "Invalid idx";
			return descriptors[idx];
		}

		vector<Image::Descriptor> Info::getDescriptors() const
		{
			return descriptors;
		}

		string Info::getName(int idx) const
		{
			if (idx < 0 || ids.size() <= idx) throw "Invalid idx";
			return names[idx];
		}

		void Info::setID(Idx idx, Image::ID id)
		{
			if (idx < 0 || ids.size() <= idx) throw "Invalid idx";
			ids[idx] = id;
		}

		void Info::setImage(Idx idx, const Mat& image, bool make_clone)
		{
			if (idx < 0 || ids.size() <= idx) throw "Invalid idx";
			images[idx] = make_clone ? image.clone() : image;
		}

		void Info::setKeyPoints(Idx idx, const Image::KeyPoints& keypoints)
		{
			if (idx < 0 || ids.size() <= idx) throw "Invalid idx";
			(this->keypoints)[idx] = keypoints;
		}

		void Info::setDescriptor(Idx idx, const Image::Descriptor& descriptor)
		{
			if (idx < 0 || ids.size() <= idx) throw "Invalid idx";
			descriptors[idx] = descriptor;
		}

		void Info::setName(Idx idx, const string& name)
		{
			if (idx < 0 || ids.size() <= idx) throw "Invalid idx";
			names[idx] = name;
		}

		size_t Info::count() const
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
		EvalEasy::EvalEasy()
			: Base(), matching_count_weight(matching_count_weight_default)
		{
		}

		EvalEasy::EvalEasy(const EvalEasy& src)
			: Base(src), matching_count_weight(src.matching_count_weight), info(src.info)
		{
		}

		EvalEasy& EvalEasy::operator=(const EvalEasy& src)
		{
			Base::operator=(src);
			matching_count_weight = src.matching_count_weight;
			info = src.info;
			return *this;
		}

		EvalEasy::~EvalEasy()
		{
		}

		const Info& EvalEasy::getInfo() const
		{
			return info;
		}

		void EvalEasy::train(const list<shared_ptr<Image::Conclusive> >& trains)
		{
			info.prepare(trains, machines);
			machines.getMatcher()->add(info.getDescriptors());
		}

		Image::Candidate::Assessments EvalEasy::match(const Image::Candidate& query) const
		{
			const bool show_matching_image = false;
			vector<DMatch> matches;
			Image::KeyPoints keypoints;
			Image::Descriptor descriptor;

			machines.getDetector()->detect(query, keypoints);
			machines.getExtractor()->compute(query, keypoints, descriptor);
			machines.getMatcher()->match(descriptor, matches);

			map<int, list<DMatch> > matches_grouped;
			for (vector<DMatch>::const_iterator it = matches.begin(); it!=matches.end(); it++) {
				matches_grouped[it->imgIdx].push_back(*it);
			}

			if (show_matching_image)
				namedWindow("matching");

			map<string, double> scores;
			map<string, int> count_by_group;
			for (map<int, list<DMatch> >::const_iterator it = matches_grouped.begin(); it!=matches_grouped.end(); it++) {
				if (it->second.empty()) continue;

				int train_idx = it->first;
				int match_count = it->second.size();
				string name = info.getName(train_idx);

				double distance_sum = 0, score = 0;
				for (list<DMatch>::const_iterator it_dmatch=it->second.begin(); it_dmatch!=it->second.end(); it_dmatch++) {
					distance_sum += it_dmatch->distance;
				}
				score += distance_sum / match_count;
				score += matching_count_weight * (double)(max(info.getKeyPoints(train_idx).size(), keypoints.size()) - match_count)
					/ it->second.size();
				scores[name] += score;
				count_by_group[name]++;

				cout << 
					"matching score: " << score << 
					"\tscore sum:" << scores[name] << 
					"\ttrain name: " << name << 
					endl;
				if (show_matching_image) {
					cv::Mat image_out;
					drawMatches(
						query, keypoints,
						info.getImage(train_idx), info.getKeyPoints(train_idx),
						convertArray<list<DMatch>, vector<DMatch> >(it->second),
						image_out);
					imshow("matching", image_out);
					waitKey();
				}
			}

			if (show_matching_image)
				destroyWindow("matching");

			Image::Candidate::Assessments assessments;
			for (map<string, double>::const_iterator it = scores.begin(); it!=scores.end(); it++) {
				string name = it->first;
				Image::Candidate::Assessment assessment;
				assessment.name = name;
				assessment.score = scores[name]/count_by_group[name];//Average
				assessments.push_back(assessment);
			}
			assessments.sort();
			return assessments;
		}
	}
}
