#include <iostream>
#include <map>
#include <opencv2/opencv.hpp>
#include "EvalEasy.h"
using namespace std;
using namespace cv;

namespace Signature
{
	namespace Guess
	{
		EvalEasy::EvalEasy() : Base()
		{
			matching_count_weight = matching_count_weight_default;
		}

		EvalEasy::EvalEasy(const EvalEasy& src) : Base(src)
		{
			matching_count_weight = src.matching_count_weight;
		}

		EvalEasy& EvalEasy::operator=(const EvalEasy& src)
		{
			Base::operator=(src);
			matching_count_weight = src.matching_count_weight;
			return *this;
		}

		EvalEasy::~EvalEasy()
		{
		}

		void EvalEasy::buildInfo(const std::list<Image::Conclusive>& images)
		{
			Base::buildInfo(images);
			matcher = new FlannBasedMatcher();
			matcher->add(info.getDescriptors());
		}

		void EvalEasy::buildInfo(const std::list<Image::Candidate>& images)
		{
			Base::buildInfo(images);
			matcher = new FlannBasedMatcher();
			matcher->add(info.getDescriptors());
		}

		Result EvalEasy::match(const Image::Info& query, int idx)
		{
			const bool show_matching_image = false;
			Mat img;
			vector<DMatch> matches;
			matcher->match(query.getDescriptor(idx), matches);

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
				score += matching_count_weight * (double)(max(info.getKeyPoints(train_idx).size(), query.getKeyPoints(idx).size()) - match_count)
					/ it->second.size();
				scores[name] += score;
				count_by_group[name]++;

				cout << 
					"matching score: " << score << 
					"\tscore sum:" << scores[name] << 
					"\ttrain name: " << name << 
					endl;
				if (show_matching_image) {
					drawMatches(
						query.getImage(idx), query.getKeyPoints(idx),
						info.getImage(train_idx), info.getKeyPoints(train_idx),
						convertArray<list<DMatch>, vector<DMatch> >(it->second), img);
					imshow("matching", img);
					waitKey();
				}
			}

			if (show_matching_image)
				destroyWindow("matching");

			Result assessments;
			for (map<string, double>::const_iterator it = scores.begin(); it!=scores.end(); it++) {
				string name = it->first;
				Image::Candidate::Assessment assessment;
				assessment.name = name;
				assessment.score = scores[name]/count_by_group[name];//Average
				assessments.push_back(assessment);
			}
			return assessments;
		}
	}
}
