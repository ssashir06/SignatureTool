#include <list>
#include <string>
#include <map>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include "../LibGuessSignature/EvalEasy.h"
#include "files.h"
using namespace std;
using namespace cv;
using namespace Signature;

list<Image::Conclusive> train;
list<Image::Candidate> query;

void loadImages()
{
	map<string, list<pair<int, Mat> > > all_images;
	int number_of_images = sizeof(file_info)/sizeof(*file_info);
	for (int i=0; i<number_of_images; i++) {
		string file_name = path + file_info[i][0];
		Mat img_loaded = imread(file_name, 0);//ƒ‚ƒmƒNƒ
		threshold(img_loaded, img_loaded, 190, 255, THRESH_BINARY);//“ñ’l‰»
		for (int j=0; j<2; j++) {
			string name = file_info[i][j+1];
			all_images[name].push_back(pair<int, Mat>(j, img_loaded));
		}
	}
	for (map<string, list<pair<int, Mat> > >::const_iterator it=all_images.begin(); it!=all_images.end(); it++) {
		string name = it->first;
		int count = it->second.size(), i = 0;
		if (count < 3) continue;
		for (list<pair<int, Mat> >::const_iterator it_img=it->second.begin(); it_img!=it->second.end(); it_img++, i++) {
			if (i < count / 2) {
				Mat img_trim = Mat(it_img->second, trim_area_train[it_img->first]);
				train.push_back(Image::Conclusive(train.size(), img_trim.clone(), name));
			} else {
				Mat img_trim = Mat(it_img->second, trim_area_query[it_img->first]);
				query.push_back(Image::Candidate(query.size(), img_trim.clone()));
			}
		}
	}
}

template<class Evaluator>
void eval()
{
	Ptr<FeatureDetector> detector = new SiftFeatureDetector();
	Ptr<DescriptorExtractor> extractor = new SiftDescriptorExtractor();

	Evaluator eval_train, eval_query;

	eval_train.setMatchingMachine(detector, extractor);
	eval_train.buildInfo(train);

	eval_query.setMatchingMachine(detector, extractor);
	eval_query.buildInfo(query);

	namedWindow("match");
	Mat img;
	const Image::Info& info_query = eval_query.getInfo();
	for (int i=0; i<info_query.size(); i++) {
		drawKeypoints(info_query.getImage(i), info_query.getKeyPoints(i), img);
		imshow("match", img);

		Guess::Result result = eval_train.match(info_query, i);

		if (result.empty()) continue;
		result.sort();
		for (Guess::Result::const_iterator it = result.begin(); it!=result.end(); it++) {
			cout << "Name: " << it->name << " (" << it->score << ")" << endl;
		}
		cout << "------------" << endl;
		waitKey();
	}
}

int main()
{
	loadImages();
	eval<Guess::EvalEasy>();
	return 0;
}
