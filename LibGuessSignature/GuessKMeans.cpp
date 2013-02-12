#include <memory>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include "GuessKMeans.h"
#include "SaveLoadCV.h"

using namespace std;
using namespace cv;
using namespace CVUtil::SaveLoadCV;

namespace Signature{
	namespace Guess {
#pragma region KMeansBase abstract class
		KMeansBase::KMeansBase(void)
			: Base(), k(k_default), machines(), bowde(machines.getExtractor(), machines.getMatcher())
		{
		}

		KMeansBase::KMeansBase(unsigned int k, const Image::MatchingMachines& machines)
			: Base(), k(k), machines(machines), bowde(machines.getExtractor(), machines.getMatcher())
		{
		}

		KMeansBase::KMeansBase(const KMeansBase& src)
			: Base(src), k(src.k), histgrams_by_name(src.histgrams_by_name.begin(), src.histgrams_by_name.end()), bowde(src.copyBOWDescriptrExtractor()), machines(src.machines)
		{
		}

		KMeansBase& KMeansBase::operator=(const KMeansBase& src)
		{
			Base::operator=(src);
			k = src.k;
			histgrams_by_name = vector<pair<string, Mat> >(src.histgrams_by_name.begin(), src.histgrams_by_name.end());
			bowde = src.copyBOWDescriptrExtractor();
			return *this;
		}

		KMeansBase::~KMeansBase(void)
		{
		}

		void KMeansBase::train(const list<Image::Conclusive >& trains)
		{
			cout << "make vocabulary" << endl;
			BOWKMeansTrainer trainer(k);
			map<string, list<Image::Conclusive > > trains_by_name;
			for (const auto& train : trains) {
				trains_by_name[train.name].push_back(train);
				trainer.add(train.getDescriptor());
			}
			bowde = BOWImgDescriptorExtractor(machines.getExtractor(), machines.getMatcher());
			bowde.setVocabulary(trainer.cluster());

			cout << "make histgrams" << endl;
			histgrams_by_name.clear();
			for (const auto& trains_group : trains_by_name)
			{
				const auto& name = trains_group.first;
				cout << "histgram of " << name << endl;
				list<Image::Descriptor> histgrams;
				for (const auto& train : trains_group.second)
				{
					histgrams.push_back(getDescriptor(train));
				}
				if (histgrams.empty()) continue;
				Image::Descriptor hist_mat = Mat(0, histgrams.front().cols, histgrams.front().type());
				for (const auto& histgram : histgrams)
					hist_mat.push_back(histgram);
				histgrams_by_name.push_back(make_pair(name, hist_mat));
			}
		}

		Image::Descriptor KMeansBase::getDescriptor(const Image::Base& image) const
		{
			Image::Descriptor descriptor;
			auto bowde_copy = copyBOWDescriptrExtractor();
			bowde_copy.compute(image.getImage(), *image.getKeyPoints(), descriptor);
			return descriptor;
		}

		BOWImgDescriptorExtractor KMeansBase::copyBOWDescriptrExtractor() const
		{
			//FIXME: thread unsafe
			//auto bow_copy = BOWImgDescriptorExtractor(machines.getExtractor(), machines.getMatcher());
			auto bow_copy = BOWImgDescriptorExtractor(new SurfDescriptorExtractor(), new FlannBasedMatcher());//ok
			Image::Descriptor descriptor = bowde.getVocabulary();
			if (!descriptor.empty()) bow_copy.setVocabulary(descriptor);
			return bow_copy;
		}
#pragma endregion
	}
}
