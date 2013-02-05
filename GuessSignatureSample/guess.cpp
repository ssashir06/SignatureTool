#include <memory>
#include <list>
#include <string>
#include <map>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include "../LibGuessSignature/GuessEvalEasy.h"
#include "../LibGuessSignature/GuessSvmOneVsAll.h"
#include "../LibGuessSignature/GuessSvmOneVsOne.h"
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
		Mat img_loaded = imread(file_name, 0);//ƒ‚ƒmƒNƒ
		threshold(img_loaded, img_loaded, 190, 255, THRESH_BINARY);//“ñ’l‰»
		for (int j=0; j<2; j++) {
			string name = file_info[i][j+1];
			if (name == ".") continue;
			all_images[name].push_back(pair<int, Mat>(j, img_loaded));
		}
	}
	for (map<string, list<pair<int, Mat> > >::const_iterator it=all_images.begin(); it!=all_images.end(); it++) {
		string name = it->first;
		int count = it->second.size(), i = 0;
		if (count < 3) continue;
		for (list<pair<int, Mat> >::const_iterator it_img=it->second.begin(); it_img!=it->second.end(); it_img++, i++) {
			if (i < count - 1) {
				Mat img_trim = Mat(it_img->second, trim_area_train[it_img->first]);
				train_images.push_back(shared_ptr<Image::Base>(new Image::Conclusive(train_images.size(), img_trim.clone(), name)));
			} else {
				Mat img_trim = Mat(it_img->second, trim_area_query[it_img->first]);
				query_images.push_back(pair<string, Mat>(name, img_trim.clone()));
			}
		}
	}
}

int main()
{
#ifdef _DEBUG
	unsigned int k = 10;
#else
	unsigned int k = 25;
#endif
#if 0
	Guess::EvalEasy trainer;
#elif 1
	Guess::SvmOneVsAll trainer(k);
#elif 1
	Guess::SvmOneVsOne trainer(k);
#endif

	loadImages();
	if (true) {
		trainer.setMatchingMachines(Image::MatchingMachines(
			Ptr<FeatureDetector>(new SurfFeatureDetector()),
			Ptr<DescriptorExtractor>(new SurfDescriptorExtractor()),
			Ptr<DescriptorMatcher>(new FlannBasedMatcher())));
		trainer.train(train_images);
	} else {
		trainer.train("kmeans.xml");
	}

	const string window_name = "query_name";
	namedWindow(window_name);
	for (const auto& query : query_images)
	{
		//trainer.log_output << "Query: " << query.first << endl;
		Guess::Result result = trainer.match(query.second);
		result.sort();
		cout << "------------------" << endl;
		cout << "Query name: " << query.first << endl;
		for (const auto& assesment : result)
		{
			cout << assesment.name << "\t" << assesment.score << endl;
		}
		imshow(window_name, query.second);
		waitKey();
	}
	destroyWindow(window_name);

	return 0;
}
