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

list<Image::Conclusive > train_images;
map<string, list<Image::Candidate> > query_image_by_name;

void loadImages()
{
	map<string, list<string> > file_by_name;
	int number_of_images = sizeof(file_info)/sizeof(*file_info);
	for (int i=0; i<number_of_images; i++) {
		string name(file_info[i][1]);
		string file_name = string(path) + string(file_info[i][0]);
		file_by_name[name].push_back(file_name);
	}
	for (const auto& image_group : file_by_name) {
		string name = image_group.first;
		if (image_group.second.size() < 5) continue;
		unsigned int i=0;
		for (const auto& file_name : image_group.second)
		{
			Mat img_loaded;
			if (false) {
				img_loaded = imread(file_name, 0);//ƒ‚ƒmƒNƒ
				threshold(img_loaded, img_loaded, 190, 255, THRESH_BINARY);//“ñ’l‰»
			} else {
				img_loaded = imread(file_name);
			}

			if (i++ > image_group.second.size() * 0.2) {
				train_images.push_back(Image::Conclusive(img_loaded, name, file_name));
			} else {
				query_image_by_name[name].push_back(Image::Candidate(img_loaded, file_name));
			}
		}
	}
}

int main()
{
	loadImages();

	ofstream ofs("experiment_data.dat", ios::out | ios::app);
	if (!ofs.is_open()) exit(1);

	for (unsigned int kb=6; kb<=12; kb+=2)
	{
		unsigned int k = (unsigned int)pow(2, kb);

		cout << "K:" << k << endl;

		Guess::SvmOneVsAll trainer(k);

		trainer.train(train_images);

		for (const auto& query_group : query_image_by_name)
		{
			unsigned int match_count = 0, try_count = 0;
			string name = query_group.first;

			for (const auto& query : query_group.second)
			{
				Image::Candidate::Assessments result = trainer.match(query);
				result.sort();

				if (result.front().name == name) match_count++;
				try_count++;
			}

			ofs <<
				k << "\t" <<
				name << "\t" <<
				match_count << "\t" << try_count << endl;
		}
	}

	ofs.close();

	return 0;
}
