#include <iostream>
#include <list>
#include <map>
#include <opencv2/opencv.hpp>
#include "GuessEvalEasy.h"
#include "SaveLoadCV.h"
using namespace CVUtil::ReadWrite;
using namespace std;
using namespace cv;

namespace Signature
{
	namespace Guess
	{
		EvalEasy::EvalEasy()
			: Base(), matching_count_weight(matching_count_weight_default), machines(), train_images()
		{
		}

		EvalEasy::EvalEasy(const EvalEasy& src)
			: Base(src), matching_count_weight(src.matching_count_weight), machines(src.machines), train_images(src.train_images)
		{
		}

		EvalEasy& EvalEasy::operator=(const EvalEasy& src)
		{
			Base::operator=(src);
			matching_count_weight = src.matching_count_weight;
			machines = src.machines;
			train_images = src.train_images;
			return *this;
		}

		EvalEasy::~EvalEasy()
		{
		}

		void EvalEasy::train(const list<Image::Conclusive>& trains)
		{
			train_images = convertArray<list<Image::Conclusive>, vector<Image::Conclusive> >(trains);

			vector<Image::Descriptor> descriptors;
			for (auto& image : train_images)
				descriptors.push_back(image.getDescriptor());

			machines.getMatcher()->clear();//TODO: make it optional
			machines.getMatcher()->add(descriptors);
		}

		Image::Candidate::Assessments EvalEasy::match(const Image::Candidate& query) const
		{
			const bool show_matching_image = false;
			Image::Candidate query_copy = query;

			vector<DMatch> matches;
			machines.getMatcher()->match(query_copy.getDescriptor(), matches);

			map<int, list<DMatch> > matches_grouped;
			for (const auto& match : matches)
				matches_grouped[match.imgIdx].push_back(match);

			if (show_matching_image)
				namedWindow("matching");

			map<string, pair<double, int> > scores;
			for (const auto& matched : matches_grouped)
			{
				if (matched.second.empty()) continue;

				const auto& image_mached = train_images[matched.first];

				double distance_sum = 0, score = 0;
				size_t match_count = matched.second.size();

				for (const auto& dmatch : matched.second)
					distance_sum += dmatch.distance;

				score += distance_sum / match_count ;
				score += matching_count_weight * (double)(max(image_mached.getKeyPoints()->size(), query_copy.getKeyPoints()->size()) - match_count)
					/ matched.second.size();
				scores[image_mached.name].first += score;
				scores[image_mached.name].second++;

				cout << 
					"matching score: " << score << 
					"\tscore: " << scores[image_mached.name].first << "/" << scores[image_mached.name].second <<
					"\ttrain name: " << image_mached.name << 
					endl;
				if (show_matching_image) {
					cv::Mat image_out;
					drawMatches(
						query_copy.getImage(), *query_copy.getKeyPoints(),
						image_mached.getImage(), *image_mached.getKeyPoints(),
						convertArray<list<DMatch>, vector<DMatch> >(matched.second),
						image_out);
					imshow("matching", image_out);
					waitKey();
				}
			}

			if (show_matching_image)
				destroyWindow("matching");

			Image::Candidate::Assessments assessments;
			for (const auto& score : scores)
			{
				string name = score.first;
				Image::Candidate::Assessment assessment;
				assessment.name = name;
				assessment.score = score.second.first/score.second.second;//Average
				assessments.push_back(assessment);
			}
			assessments.sort();
			return assessments;
		}

		void EvalEasy::saveModel(const string& file_name) const
		{
			FileStorage fs(file_name + ".xml", FileStorage::WRITE);
			fs << "MatchingCountWeight" << matching_count_weight;
			fs << "Machines" << (ICVSaveLoad&)machines;
			fs << "TrainImages" << convertArray<vector<Image::Conclusive>, list<Image::Conclusive::Capsule<Image::Conclusive> > >(train_images);
			fs.release();
		}

		void EvalEasy::loadModel(const string& file_name)
		{
			list<Image::Conclusive::Capsule<Image::Conclusive> > train_images_c;

			FileStorage fs(file_name + ".xml", FileStorage::READ);
			fs["MatchingCountWeight"] >> matching_count_weight;
			fs["Machines"] >> (ICVSaveLoad&)machines;
			fs["TrainImages"] >> train_images_c;
			fs.release();

			train_images = convertArray<list<Image::Conclusive::Capsule<Image::Conclusive> >, vector<Image::Conclusive> >(train_images_c);

			vector<Image::Descriptor> train_descriptors(train_images.size());
			int i=0;
			for (const auto& train : train_images) train_descriptors[i++] = train.getDescriptor();
			machines.getMatcher()->add(train_descriptors);
		}
	}
}
