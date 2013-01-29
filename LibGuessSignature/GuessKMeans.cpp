#include <memory>
#include <vector>
#include <list>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <opencv2/opencv.hpp>
#include "GuessKMeans.h"

using namespace std;
using namespace cv;

namespace Signature{
	namespace Guess {
#pragma region KMeansBase abstract class
		KMeansBase::KMeansBase(void)
			: Base(), k(k_default)
		{
		}

		KMeansBase::KMeansBase(unsigned int k)
			: Base(), k(k)
		{
		}

		KMeansBase::KMeansBase(const KMeansBase& src)
			: Base(src), k(src.k), histgrams_by_name(src.histgrams_by_name.begin(), src.histgrams_by_name.end())
		{
		}

		KMeansBase& KMeansBase::operator=(const KMeansBase& src)
		{
			Base::operator=(src);
			k = src.k;
			histgrams_by_name = map<string, Mat>(src.histgrams_by_name.begin(), src.histgrams_by_name.end());
			return *this;
		}

		KMeansBase::~KMeansBase(void)
		{
		}

		void KMeansBase::setImages(const list<shared_ptr<Image::Base> >& trains)
		{
			ofstream ofs_log("log_kmeans.txt");

			cout << "make vocabulary" << endl;
			BOWKMeansTrainer trainer(k);
			map<string, list<pair<Mat, Image::Info::KeyPoints> > > trains_by_name;
			for (const auto& train : trains) {
				Image::Info::KeyPoints keypoints;
				machines.getDetector()->detect(train->signature, keypoints);
				trains_by_name[train->getName()].push_back(pair<Mat, Image::Info::KeyPoints>(train->signature, keypoints));

				Image::Info::Descriptor descriptor;
				machines.getExtractor()->compute(train->signature, keypoints, descriptor);
				trainer.add(descriptor);

				ofs_log << "descriptor added of " << train->getName() << endl;
				ofs_log << descriptor << endl;
			}
			vocabularies = trainer.cluster();
			BOWImgDescriptorExtractor bowde = makeBOWImageDescriptorExtractor();

			ofs_log << "vocabularies" << endl;
			ofs_log << vocabularies << endl;

			cout << "make histgrams" << endl;
			histgrams_by_name.clear();
			for (const auto& trains_group : trains_by_name)
			{
				const auto& name = trains_group.first;
				cout << "histgram of " << name << endl;
				list<Image::Info::Descriptor> histgrams;
				for (const auto& train : trains_group.second)
				{
					Image::Info::KeyPoints keypoints = train.second;
					Image::Info::Descriptor descriptor;
					bowde.compute(train.first, keypoints, descriptor);

					histgrams.push_back(descriptor);
				}
				if (histgrams.empty()) continue;
				histgrams_by_name[name] = Mat(0, histgrams.front().cols, histgrams.front().type());
				for (const auto& histgram : histgrams)
				{
					histgrams_by_name[name].push_back(histgram);
				}

				ofs_log << "histgrams of " << name << endl;
				ofs_log << histgrams_by_name[name] << endl;
			}

			ofs_log.close();
		}

		BOWImgDescriptorExtractor KMeansBase::makeBOWImageDescriptorExtractor() const
		{
			BOWImgDescriptorExtractor bowde(machines.getExtractor(), machines.getMatcher());
			bowde.setVocabulary(vocabularies);
			return bowde;
		}
#pragma endregion
	}
}
