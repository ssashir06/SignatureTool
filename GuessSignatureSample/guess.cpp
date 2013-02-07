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

list<Image::Conclusive > train_images;
map<string, list<Image::Candidate> > query_image_by_name;

void loadImages()
{
	map<string, list<pair<int, string> > > file_by_name;
	int number_of_images = sizeof(file_info)/sizeof(*file_info);
	for (int i=0; i<number_of_images; i++) {
		string file_name = path + file_info[i][0];
		for (int j=0; j<2; j++)
		{
			string name = file_info[i][j+1];
			file_by_name[name].push_back(make_pair(j, file_name));
		}
	}

	pair<string, Mat> last_loaded;
	for (const auto& image_group : file_by_name) {
		string name = image_group.first;
		unsigned int i=0;

		if (image_group.second.size() < 3) continue;
		if (name == ".") continue;

		for (const auto& file_name_group : image_group.second)
		{
			const auto type = file_name_group.first;
			const auto file_name = file_name_group.second;

			Mat img_loaded;
			if (last_loaded.first != file_name)
			{
				if (true)
				{
					img_loaded = imread(file_name, 0);//モノクロ
					//threshold(img_loaded, img_loaded, 190, 255, THRESH_BINARY);//二値化
				} else {
					img_loaded = imread(file_name);
				}

				last_loaded = make_pair(file_name, img_loaded);
			} else {
				img_loaded = last_loaded.second;
			}

			Mat img_trim = Mat(img_loaded, trim_area[type]).clone();
			if (i++ != 0) {
				train_images.push_back(Image::Conclusive(img_trim, name, file_name));
			} else {
				query_image_by_name[name].push_back(Image::Candidate(img_trim, file_name));
			}
		}
	}
}

int main()
{
#ifdef _DEBUG
	unsigned int k = 10;
#else
	unsigned int k = 100;
#endif
#if 0
	Guess::EvalEasy trainer;
#elif 1
	Guess::SvmOneVsAll trainer(k);
#elif 1
	Guess::SvmOneVsOne trainer(k);
#endif

	loadImages();
	trainer.train(train_images);

	const string window_name = "query_name";
	namedWindow(window_name);
	for (const auto& query_group : query_image_by_name)
	{
		string name = query_group.first;
		for (const auto& query : query_group.second)
		{
			Image::Candidate::Assessments result = trainer.match(query);
			result.sort();
			cout << "------------------" << endl;
			cout << "Query name: " << name << endl;
			for (const auto& assesment : result)
			{
				cout << assesment.name << "\t" << assesment.score << endl;
			}
			imshow(window_name, query.getImage());
			waitKey();
		}
	}
	destroyWindow(window_name);

	return 0;
}
