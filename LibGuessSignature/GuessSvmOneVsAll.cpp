#include <list>
#include <tuple>
#include <iostream>
#include <sstream>
#include <cfloat>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <svm.h>
#include "GuessSvmOneVsAll.h"
using namespace std;
using namespace cv;

namespace Signature{
	namespace Guess {
#pragma region SvmOneVsAll
		SvmOneVsAll::SvmOneVsAll(void)
			: SvmBase()
		{
		}

		SvmOneVsAll::SvmOneVsAll(unsigned int k, const LibSVM::Parameter& param)
			: SvmBase(k, param)
		{
		}

		SvmOneVsAll::SvmOneVsAll(const SvmOneVsAll& src)
			: SvmBase(src), scaling(src.scaling), models_by_name(src.models_by_name)
		{
		}

		SvmOneVsAll& SvmOneVsAll::operator=(const SvmOneVsAll& src)
		{
			KMeansBase::operator=(src);
			scaling = src.scaling;
			models_by_name = src.models_by_name;
			return *this;
		}

		SvmOneVsAll::~SvmOneVsAll(void)
		{
		}

		SvmBase* SvmOneVsAll::clone() const
		{
			return new SvmOneVsAll(*this);
		}

		void SvmOneVsAll::train(const list<Image::Conclusive >& images)
		{
			setBestParam(images, 2);
			KMeansBase::train(images);
			train();
		}

		void SvmOneVsAll::train()
		{
			scaling = buildScalingSetting(histgrams_by_name);

			models_by_name.clear();
			for (const auto& hist_positive: histgrams_by_name)
			{
				list<vector<LibSVM::NodeArray::Classified> > data_set, data_set_positive, data_set_rest;
				for (const auto& hist : histgrams_by_name)
				{
					if (hist_positive.first == hist.first) {
						auto d = buildClassfiedData(hist.second, 1);
						data_set.push_back(d);
						data_set_positive.push_back(d);
					}
					else {
						auto d = buildClassfiedData(hist.second, 0);
						data_set.push_back(d);
						data_set_rest.push_back(d);
					}
				}

				auto data_set_merged = LibSVM::mergeClassified(data_set);
				LibSVM::scale(scaling, data_set_merged);

				LibSVM::Problem prob(data_set_merged);

				const char* param_error_message = svm_check_parameter(&prob, param);
				if (param_error_message) {
					OmpStream(cerr) << "Parameter is wrong: " << param_error_message << endl;
					continue;
				}

				LibSVM::Model model(svm_train(&prob, param));
				models_by_name[hist_positive.first] = make_tuple(model, prob);
			}
		}

		Image::Candidate::Assessments SvmOneVsAll::match(const Image::Candidate& query) const
		{
			LibSVM::NodeArray node_list = buildNodeArray(getDescriptor(query));
			LibSVM::scale(scaling, node_list);

			OmpStream(cout) << "matching query" << endl;
			
			Image::Candidate::Assessments assessments;
			for (const auto& model_group : models_by_name)
			{
				const auto& model = get<0>(model_group.second);
				double svm_out = svm_predict(model, node_list.getPtr());

				Image::Candidate::Assessment assessment;
				assessment.name = model_group.first;
				assessment.score = - svm_out;
				assessments.push_back(assessment);
			}
			return assessments;
		}
#pragma endregion
	}
}
