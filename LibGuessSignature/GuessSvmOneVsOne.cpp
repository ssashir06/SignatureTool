#include <list>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include "GuessSvmOneVsOne.h"
#include "SaveLoadCV.h"

using namespace cv;
using namespace std;
using namespace CVUtil::ReadWrite;

namespace Signature
{
	namespace Guess
	{
		SvmOneVsOne::SvmOneVsOne(void)
			: SvmBase()
		{
		}

		SvmOneVsOne::SvmOneVsOne(unsigned int k)
			: SvmBase(k)
		{
		}

		SvmOneVsOne::SvmOneVsOne(const SvmOneVsOne& src)
			: SvmBase(src), scaling(src.scaling), models_by_name(src.models_by_name), train_images(src.train_images)
		{
		}

		SvmOneVsOne& SvmOneVsOne::operator=(const SvmOneVsOne& src)
		{
			SvmBase::operator=(src);
			scaling = src.scaling;
			models_by_name = src.models_by_name;
			train_images = src.train_images;
			return *this;
		}

		SvmOneVsOne::~SvmOneVsOne(void)
		{
		}

		SvmBase* SvmOneVsOne::clone() const
		{
			return new SvmOneVsOne(*this);
		}

		void SvmOneVsOne::train(const list<Image::Conclusive >& images, bool adding)
		{
			if (adding) {
				for (const auto& image : images)
					train_images.push_back(image);
			} else {
				train_images = images;
			}

			setBestParam(train_images);
			KMeansBase::train(train_images);
			train();
		}

		void SvmOneVsOne::train()
		{
			scaling = buildScalingSetting(histgrams_by_name);

			models_by_name.clear();
			for (const auto& hist_positive : histgrams_by_name)
			{
				vector<LibSVM::NodeArray::Classified> positive_classified_data = buildClassfiedData(hist_positive.second, 1);

#pragma omp parallel for
				for (int rest_i = 0; rest_i<(int)histgrams_by_name.size(); rest_i++)
				{
					const auto& hist_rest = histgrams_by_name[rest_i];
					if (hist_positive.first == hist_rest.first) continue;

					list<vector<LibSVM::NodeArray::Classified> > data_set;
					data_set.push_back(positive_classified_data);
					data_set.push_back(buildClassfiedData(hist_rest.second, 0));

					auto data_set_merged = LibSVM::mergeClassified(data_set);
					LibSVM::scale(scaling, data_set_merged);

					LibSVM::Problem prob(data_set_merged);

					const char* param_error_message = svm_check_parameter(&prob, param);
					if (param_error_message) {
						cerr << "Parameter is wrong: " << param_error_message << endl;
						continue;
					}

					LibSVM::Model model(svm_train(&prob, param));
#pragma omp critical
					{
						models_by_name[hist_positive.first].push_back(make_tuple(hist_rest.first, model, prob));
					}
				}
			}
		}

		Image::Candidate::Assessments SvmOneVsOne::match(const Image::Candidate& query) const
		{
			LibSVM::NodeArray node_list = buildNodeArray(getDescriptor(query));
			LibSVM::scale(scaling, node_list);

			cout << "matching query of image" << endl;
			
			Image::Candidate::Assessments assessments;
			for (const auto& model_group : models_by_name)
			{
				if (model_group.second.empty()) continue;

				double score_sum = 0;
				unsigned int try_count = 0;
				for (const auto& rest_group : model_group.second)
				{
					const auto& model = get<1>(rest_group);
					double svm_out = svm_predict(model, node_list.getPtr());

					try_count++;
					score_sum += svm_out > 0 ? 1 : 0;
				}

				Image::Candidate::Assessment assessment;
				assessment.name = model_group.first;
				assessment.score = - score_sum / try_count;
				assessments.push_back(assessment);
			}
			assessments.sort();
			return assessments;
		}

		void SvmOneVsOne::saveModel(const string& file_name) const
		{
			//TODO
			throw exception("not implemented!");
		}

		void SvmOneVsOne::loadModel(const string& file_name)
		{
			//TODO
			throw exception("not implemented!");
		}

		pair<string, string> SvmOneVsOne::modelSuffix() const
		{
			//TODO
			throw exception("not implemented!");
		}

		void SvmOneVsOne::strip(bool remove_keypoints)
		{
			for (auto& image : train_images) image.strip(remove_keypoints);
		}
	}
}
