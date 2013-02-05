#include <memory>
#include <list>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include "../LibGuessSignature/GuessSvmOneVsAll.h"
#include "files.h"
using namespace std;
using namespace cv;
using namespace Signature;

list<shared_ptr<Image::Base> > train_images;
list<pair<string, Mat> > query_images;

void loadImages()
{
	map<string, list<pair<int, Mat> > > all_images;
	int number_of_images = sizeof(file_info)/sizeof(*file_info);
	for (int i=0; i<number_of_images; i++) {
		string file_name = path + file_info[i][0];
		Mat img_loaded;
		if (false) {
			img_loaded = imread(file_name, 0);//ƒ‚ƒmƒNƒ
			threshold(img_loaded, img_loaded, 190, 255, THRESH_BINARY);//“ñ’l‰»
		} else {
			img_loaded = imread(file_name);
		}
		string name = file_info[i][1];
		all_images[name].push_back(pair<int, Mat>(1, img_loaded));
	}
	for (const auto& image_group : all_images) {
		string name = image_group.first;
		int count = image_group.second.size(), i = 0;
		if (count < 5) continue;
		for (const auto& image : image_group.second) {
			if (i++ > count * 0.2) {
				train_images.push_back(shared_ptr<Image::Base>(new Image::Conclusive(train_images.size(), image.second, name)));
			} else {
				query_images.push_back(pair<string, Mat>(name, image.second));
			}
		}
	}
}

int main()
{
	loadImages();

	ofstream ofs("experiment_data.dat", ios::out | ios::app);
	if (!ofs.is_open()) exit(1);

	for (unsigned int kb=4; kb<10; kb++)
	{
		unsigned int k = pow(2, kb);

		cout << "K:" << k << endl;

		Guess::SvmOneVsAll trainer(k);

		trainer.setMatchingMachines(Image::MatchingMachines(
			Ptr<FeatureDetector>(new SurfFeatureDetector()),
			Ptr<DescriptorExtractor>(new SurfDescriptorExtractor()),
			Ptr<DescriptorMatcher>(new FlannBasedMatcher())));
		trainer.train(train_images);

		map<string, pair<unsigned int, unsigned int> > match_scores;
		for (const auto& query : query_images)
		{
			Guess::Result result = trainer.match(query.second);
			result.sort();

			auto& match_score = match_scores[query.first];
			if (result.front().name == query.first) match_score.first++;
			match_score.second++;
		}

		for (const auto& match_score : match_scores)
		{
			ofs <<
				k << "\t" <<
				match_score.first << "\t" <<
				match_score.second.first << "\t" <<match_score.second.second << endl;
		}
	}

	ofs.close();

	return 0;
}
