#include <list>
#include <tuple>
#include <iostream>
#include <cfloat>
#include "GuessSvmOneVsAll.h"
using namespace std;
using namespace cv;

namespace Signature{
	namespace Guess {
#pragma region SvmOneVsAll
		SvmOneVsAll::SvmOneVsAll(void)
			: KMeansBase()
		{
		}

		SvmOneVsAll::SvmOneVsAll(unsigned int k, const CvSVMParams& svm_params)
			: KMeansBase(k), params(svm_params)
		{
		}

		SvmOneVsAll::SvmOneVsAll(const SvmOneVsAll& src)
			: KMeansBase(src)
		{
		}

		SvmOneVsAll& SvmOneVsAll::operator=(const SvmOneVsAll& src)
		{
			KMeansBase::operator=(src);
			return *this;
		}

		SvmOneVsAll::~SvmOneVsAll(void)
		{
		}

		void SvmOneVsAll::setImages(const list<shared_ptr<Image::Base> >& images)
		{
			KMeansBase::setImages(images);
			int label_type = CV_32FC1;//CV_32SC1 (only in the classification problem) or CV_32FC1 format

			svm_models_by_name.clear();
			for (const auto& hist_positive : histgrams_by_name)
			{
				Mat histgrams(0, hist_positive.second.cols, hist_positive.second.type());
				Mat labels = Mat::ones(hist_positive.second.rows, 1, label_type);
				histgrams.push_back(hist_positive.second);

				int rest_count = 0;
				for (const auto& hist_rest : histgrams_by_name)
				{
					if (hist_positive.first == hist_rest.first) continue;
					histgrams.push_back(hist_rest.second);
					Mat z = Mat::zeros(hist_rest.second.rows, 1, label_type);
					labels.push_back(z);
					rest_count++;
				}

				if (rest_count == 0) continue;

				Mat histgrams_for_svm;
				if (histgrams.type() != CV_32FC1)
					histgrams.convertTo(histgrams_for_svm, CV_32FC1);//Only CV_32FC1 is accepted by CvSVM
				else
					histgrams_for_svm = histgrams;

				shared_ptr<CvSVM> svm_model(new CvSVM());
				cv::Mat var_idx, sample_idx;
				svm_model->train(histgrams_for_svm, labels, var_idx, sample_idx, params);
				svm_models_by_name[hist_positive.first] = make_tuple(svm_model, histgrams_for_svm, labels, var_idx, sample_idx);
			}
		}

		Result SvmOneVsAll::match(const Mat& query) const
		{
			BOWImgDescriptorExtractor bowde = makeBOWImageDescriptorExtractor();
			Image::Info::KeyPoints keypoints;
			Image::Info::Descriptor descriptor;
			machines.getDetector()->detect(query, keypoints);
			bowde.compute(query, keypoints, descriptor);

			cout << "matching query of image" << endl;
			Result assessments;
			for (const auto& svm_model_group : svm_models_by_name)
			{
				const auto& svm_model = get<0>(svm_model_group.second);
				float score_f = svm_model->predict(descriptor);
				Image::Candidate::Assessment assessment;
				assessment.name = svm_model_group.first;
				assessment.score = score_f;
				assessments.push_back(assessment);
			}
			return assessments;
		}
#pragma endregion
	}
}