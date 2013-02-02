#include <list>
#include <tuple>
#include <iostream>
#include <sstream>
#include <cfloat>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <svm.h>
#include "GuessSvm.h"
using namespace std;
using namespace cv;

namespace Signature{
	namespace Guess {
#pragma region SvmBase
		SvmBase::SvmBase(void)
			: KMeansBase(), param(buildDefaultParam())
		{
		}

		SvmBase::SvmBase(unsigned int k, const LibSVM::Parameter& param)
			: KMeansBase(k), param(param)
		{
		}

		SvmBase::SvmBase(const SvmBase& src)
			: KMeansBase(src), param(src.param)
		{
		}

		SvmBase& SvmBase::operator=(const SvmBase& src)
		{
			KMeansBase::operator=(src);
			param = src.param;
			return *this;
		}

		SvmBase::~SvmBase(void)
		{
		}

		void SvmBase::setBestParam(const list<shared_ptr<Image::Base> >& images, unsigned int grid)
		{
			auto param_scores = crossValidation(images, grid);
			param_scores.sort([](const SvmBase::ParamScore& v1, const SvmBase::ParamScore& v2){ return get<0>(v1) < get<0>(v2); });
			param = get<1>(param_scores.back());
		}

		LibSVM::Parameter SvmBase::getParam() const
		{
			return param;
		}

		list<SvmBase::ParamScore> SvmBase::crossValidation(const list<shared_ptr<Image::Base> >& images, unsigned int grid)
		{
			map<string, list<shared_ptr<Image::Base> > > images_by_name;
			vector<list<shared_ptr<Image::Base> > > parted_image_list(grid);
			for (const auto& image : images) images_by_name[image->getName()].push_back(image);
			for (const auto& image : images_by_name)
			{
				unsigned int grid_local = std::min(grid, image.second.size());
				unsigned int part_count = image.second.size() / grid_local;
				unsigned int part_mod = image.second.size() % grid_local;

				auto it = image.second.begin();
				for (unsigned int i=0; i<grid_local; i++)
				{
					for (unsigned int j=0; j<part_count; j++)
						parted_image_list[i].push_back(*(it++));
				}
				for (unsigned int i=0; i<part_mod; i++)
				{
					parted_image_list[i].push_back(*(it++));
				}
			}

			LibSVM::v_range r_c(15, -5), r_gamma(3, -15);
			map<pair<int, int>, pair<double, unsigned int> > try_log;
			map<pair<int, int>, LibSVM::Parameter> params;

			for (int p_c = (int)r_c.second; p_c <= r_c.first; p_c++)
			{
				for (int p_gamma = (int)r_gamma.second; p_gamma <= r_gamma.first; p_gamma++)
				{
					auto score_key = make_pair(p_c, p_gamma);
					params[score_key] = buildDefaultParam();
					params[score_key]->C = pow(p_c, 2);
					params[score_key]->gamma = pow(p_gamma, 2);
				}
			}
			for (unsigned int i=0; i<grid; i++)
			{
				const list<shared_ptr<Image::Base> >& test_data = parted_image_list[i];
				list<shared_ptr<Image::Base> > train_data;
				map<string, bool> names_on_train_data;
				for (unsigned int j=0; j<grid; j++)
				{
					if (i==j) continue;
					train_data.insert(train_data.end(), parted_image_list[j].begin(), parted_image_list[j].end());

					for (const auto& image : parted_image_list[j])
						names_on_train_data[image->getName()] = true;
				}
				if (test_data.empty() || train_data.empty()) continue;
				KMeansBase::train(train_data);

				for (const auto& param : params)
				{
					this->param = param.second;
					train();

					cout << "testing pram is (" << param.first.first << ", " << param.first.second << ")" << endl;
					for (const auto& data : test_data)
					{
						if (names_on_train_data.find(data->getName()) == names_on_train_data.end()) continue;

						Result results = match(data->signature);
						double score = 0;
						for (const auto& r : results)
							score += (r.name == data->getName()) ? r.score : -r.score * 0.5;

						if (try_log.find(param.first) == try_log.end()) {
							try_log[param.first] = make_pair(score, 1);
						} else {
							try_log[param.first].first += score;
							try_log[param.first].second++;
						}

						cout << data->getName() << "\tscore: " << score << endl;
					}
				}
			}

			list<ParamScore> try_log_avg;
			for (const auto& log : try_log)
				try_log_avg.push_back(make_pair(log.second.first/log.second.second, params[log.first]));
			try_log_avg.sort([](const ParamScore& v1, const ParamScore& v2){ return v1.first < v2.first; });

			unsigned int i=0;
			for (auto it = try_log_avg.begin(); it != try_log_avg.end() && i < 10; it++, i++)
				cout << 
					"No. " << i+1 << "\t" << "score:" << it->first << "\t" <<
					"C:" << it->second->C << "\t" << "gamma:" << it->second->gamma << endl;

			return try_log_avg;
		}

		LibSVM::Parameter SvmBase::buildDefaultParam()
		{
			LibSVM::Parameter param;
			param->svm_type = C_SVC;
			param->kernel_type = RBF;
			param->degree = 3;
			param->gamma = 8;//
			param->coef0 = 0;
			param->nu = 0.5;
			param->cache_size = 100;
			param->C = 8;//
			param->eps = 1e-5;
			param->p = 0.1;
			param->shrinking = 1;
			param->probability = 0;
			param->nr_weight = 0;
			param->weight_label = NULL;
			param->weight = NULL;
			return param;
		}
#pragma endregion
#pragma region Handle cv::Mat
		vector<LibSVM::NodeArray::Classified> buildClassfiedData(const Mat& histgram, double label)
		{
			auto data = vector<LibSVM::NodeArray::Classified>(histgram.rows);

			for (int r=0; r<histgram.rows; r++)
			{
				LibSVM::NodeArray nodes = buildNodeArray(Mat(histgram, Rect(0, r, histgram.cols, 1)));
				data[r] = LibSVM::NodeArray::Classified(label, nodes);
			}
			return data;
		}

		LibSVM::NodeArray buildNodeArray(const Mat& descriptor)
		{
			if (descriptor.rows != 1) throw;
			auto nodes = make_shared<vector<LibSVM::Node> >(descriptor.cols);
			for (int c=0; c<descriptor.cols; c++)
			{
				double v;
				switch (descriptor.type())
				{
				case CV_32F:
					v = descriptor.at<float>(Point(c, 0));
					break;
				case CV_64F:
					v = descriptor.at<double>(Point(c, 0));
					break;
				default:
					throw;
				}
				(*nodes)[c].index = c;
				(*nodes)[c].value = v;
			}
			return LibSVM::NodeArray(nodes);
		}

		LibSVM::ScalingSetting buildScalingSetting(const map<string, Mat>& histgrams_by_name)
		{
			LibSVM::ScalingSetting scaling;
			for (const auto& hist_positive: histgrams_by_name)
			{
				list<vector<LibSVM::NodeArray::Classified> > data_set;
				for (const auto& hist : histgrams_by_name)
					data_set.push_back(buildClassfiedData(hist.second, 0));
				auto data_set_merged = LibSVM::mergeClassified(data_set);
				scaling = LibSVM::buildSettingOfScaling(data_set_merged);
			}
			return scaling;
		}
#pragma endregion
	}
}