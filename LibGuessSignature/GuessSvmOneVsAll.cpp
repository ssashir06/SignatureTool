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
			: SvmBase(src), scaling(src.scaling), model_filename_by_name(src.model_filename_by_name)
		{
		}

		SvmOneVsAll& SvmOneVsAll::operator=(const SvmOneVsAll& src)
		{
			KMeansBase::operator=(src);
			scaling = src.scaling;
			model_filename_by_name = src.model_filename_by_name;
			return *this;
		}

		SvmOneVsAll::~SvmOneVsAll(void)
		{
		}

		void SvmOneVsAll::train(const list<shared_ptr<Image::Base> >& images)
		{
			setBestParam(images, 2);
			KMeansBase::train(images);
			train();
		}

		void SvmOneVsAll::train(const string& file_name)
		{
			load(file_name);
			train();
		}

		void SvmOneVsAll::train()
		{
			scaling = buildScalingSetting(histgrams_by_name);

			model_filename_by_name.clear();
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
					cerr << "Parameter is wrong: " << param_error_message << endl;
					continue;
				}

				LibSVM::Model model(svm_train(&prob, param));

				stringstream file_name;
				file_name << "svm_model_" << hist_positive.first << ".dat";
				svm_save_model(file_name.str().c_str(), model);
				model_filename_by_name[hist_positive.first] = file_name.str();
			}
		}

		Result SvmOneVsAll::match(const Mat& query) const
		{
			BOWImgDescriptorExtractor bowde = makeBOWImageDescriptorExtractor();
			Image::Info::KeyPoints keypoints;
			Image::Info::Descriptor descriptor;
			machines.getDetector()->detect(query, keypoints);
			bowde.compute(query, keypoints, descriptor);
			LibSVM::NodeArray node_list = buildNodeArray(descriptor);
			LibSVM::scale(scaling, node_list);

			cout << "matching query of image" << endl;
			
			Result assessments;
			for (const auto& model_file_group : model_filename_by_name)
			{
				LibSVM::Model model(svm_load_model(model_file_group.second.c_str()));
				double svm_out = svm_predict(model, node_list.getPtr());

				Image::Candidate::Assessment assessment;
				assessment.name = model_file_group.first;
				assessment.score = svm_out;
				assessments.push_back(assessment);
			}
			return assessments;
		}

		//TODO: write load() and save() (overwriting)
#pragma endregion
	}
}
