#include <list>
#include <tuple>
#include <iostream>
#include <sstream>
#include <fstream>
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

		void SvmBase::setBestParam(const list<Image::Conclusive>& images, unsigned int grid)
		{
			auto param_scores = crossValidation(images, grid);
			param_scores.sort([](const SvmBase::ParamScore& v1, const SvmBase::ParamScore& v2){ return get<0>(v1) < get<0>(v2); });
			param = get<1>(param_scores.front());
		}

		LibSVM::Parameter SvmBase::getParam() const
		{
			return param;
		}

		list<SvmBase::ParamScore> SvmBase::crossValidation(const list<Image::Conclusive>& images, unsigned int grid)
		{
			map<string, list<Image::Conclusive> > images_by_name;
			vector<list<Image::Conclusive> > parted_image_list(grid);
			for (const auto& image : images) images_by_name[image.name].push_back(image);
			for (auto& images : images_by_name)
			{
				list<pair<size_t, size_t> > image_sizes;
				for (size_t i=0; i<parted_image_list.size(); i++) image_sizes.push_back(make_pair(i, parted_image_list[i].size()));
				const auto& min_partation = min_element(image_sizes.begin(), image_sizes.end(),
					[](const pair<size_t, size_t>& v1, const pair<size_t, size_t>& v2) { return v1.second < v2.second; });

				unsigned int i = (min_partation == image_sizes.end()) ? 0 : min_partation->first;
				for (auto& image : images.second)
				{
					image.getDescriptor();//update descriptor/keypoint
					parted_image_list[(i++)%parted_image_list.size()].push_back(image);
				}
			}

			vector<pair<shared_ptr<SvmBase>, map<string, list<Image::Candidate> > > > svms(parted_image_list.size());
			for (unsigned int i=0; i<parted_image_list.size(); i++)
			{
				auto& test_data = svms[i].second;
				for (const auto& image : parted_image_list[i])
					test_data[image.name].push_back(image);

				list<Image::Conclusive> train_data;
				for (unsigned int j=0; j<parted_image_list.size(); j++)
					if (i!=j)
						train_data.insert(train_data.end(), parted_image_list[j].begin(), parted_image_list[j].end());

				if (test_data.empty() || train_data.empty()) continue;
				svms[i].first = shared_ptr<SvmBase>(clone());
				svms[i].first->KMeansBase::train(train_data);
			}

			double gamma_center = std::log(1/(double)k)/std::log(2);
#ifdef _DEBUG
			LibSVM::v_range r_c(15, -4), r_gamma(gamma_center + 1, gamma_center - 1);
			double step_c = 3, step_gamma = 1;
#else
			LibSVM::v_range r_c(20, -4), r_gamma(gamma_center + 2, gamma_center - 2);
			double step_c = 1, step_gamma = 0.5;
#endif
			typedef pair<double, double> TargetKey;
			list<TargetKey> targets;
			map<TargetKey, tuple<double, unsigned int, LibSVM::Parameter> > try_log;

			double step_size = 9;
			for (double p_c = r_c.second; p_c <= r_c.first; p_c+=step_c*step_size)
				for (double p_gamma = r_gamma.second; p_gamma <= r_gamma.first; p_gamma+=step_gamma*step_size)
					targets.push_back(make_pair(p_c, p_gamma));

			while (!targets.empty())
			{
				vector<pair<TargetKey, LibSVM::Parameter> > params_array;
				for (const auto& target : targets) {
					auto param = buildDefaultParam();
					param->C = std::pow(2, target.first);
					param->gamma = std::pow(2, target.second);
					param->cache_size = std::max((double)100, std::min((double)500, (double)5 * images.size()));
					params_array.push_back(make_pair(target, param));
				}

				for (unsigned int i=0; i<svms.size(); i++)
				{
#pragma omp parallel for
					for (int j = 0; j<(int)params_array.size(); j++)
					{
						const auto& param_key = params_array[j].first;
						shared_ptr<SvmBase> svm;

						OmpStream(cout) << "testing pram: (" << param_key.first << ", " << param_key.second << ")" << endl;

#pragma omp critical
						{
							svm = shared_ptr<SvmBase>(svms[i].first->clone());
							svm->param = params_array[j].second;
						}
						svm->train();

						for (const auto& data_group : svms[i].second)
						{
							double score = 0;
							for (const auto& candidate : data_group.second)
							{
								Image::Candidate::Assessments results = svm->match(candidate);
								for (const auto& r : results)
									score += (r.name == data_group.first) ? r.score : r.score * -0.8; //TODO
							}
							score /= data_group.second.size();

							OmpStream(cout) << data_group.first << "\tscore: " << score << endl;

#pragma omp critical
							{
								if (try_log.find(param_key) == try_log.end()) {
									try_log[param_key] = make_tuple(score, 1, svm->param);
								} else {
									auto& log_data = try_log[param_key];
									get<0>(log_data) += score;
									get<1>(log_data)++;
								}
							}
						}
					}
				}


				list<pair<TargetKey, double> > scores_tried;
				double avg_sum = 0;
				for (const auto& target : targets) {
					double avg = get<0>(try_log[target])/get<1>(try_log[target]);
					scores_tried.push_back(make_pair(target, avg));
					avg_sum += avg;
				}
				step_size*=1/3.0;
				if (step_size<1) break;

				targets.clear();
				for (const auto& score : scores_tried)
				{
					if (score.second <= avg_sum/scores_tried.size())
					{
						for (int sc = -1; sc <= 1; sc+= 1)
							for (int sg = -1; sg <= 1; sg+=1)
								//if (sc != 0 || sg != 0) //”ª•û
								if (sc != 0 && sg != 0) //Žl•û
									targets.push_back(make_pair(sc * step_c * step_size + score.first.first, sg * step_gamma * step_size + score.first.second));
					}
				}
			}

			ofstream ofs("cross_validation_grids.dat", ios::out | ios::app);

			list<ParamScore> try_log_avg;
			for (const auto& log : try_log)
			{
				try_log_avg.push_back(make_pair(get<0>(log.second)/get<1>(log.second), get<2>(log.second)));
				if (ofs.is_open())
					ofs << 
						k << "\t" <<
						get<0>(log.second) << "\t" << get<1>(log.second) << "\t" <<
						get<2>(log.second)->C << "\t" << get<2>(log.second)->gamma << endl;
			}
			try_log_avg.sort([](const ParamScore& v1, const ParamScore& v2){ return v1.first < v2.first; });
			
			ofs.close();

			unsigned int i=0;
			for (auto it = try_log_avg.begin(); it != try_log_avg.end() && i < 10; it++, i++)
				cout << 
					"No. " << i+1 << "\t" << "score:" << it->first << "\t" <<
					"C:" << it->second->C << "\t" << "gamma:" << it->second->gamma << endl;

			return try_log_avg;
		}
#pragma endregion
#pragma region Handle LibSVM
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
	
		vector<LibSVM::NodeArray::Classified> buildClassfiedData(const Image::Descriptor& histgram, double label)
		{
			auto data = vector<LibSVM::NodeArray::Classified>(histgram.rows);

			for (int r=0; r<histgram.rows; r++)
			{
				LibSVM::NodeArray nodes = buildNodeArray(Mat(histgram, Rect(0, r, histgram.cols, 1)));
				data[r] = LibSVM::NodeArray::Classified(label, nodes);
			}
			return data;
		}

		LibSVM::NodeArray buildNodeArray(const Image::Descriptor& descriptor)
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

		LibSVM::ScalingSetting buildScalingSetting(const vector<pair<string, Image::Descriptor> >& histgrams_by_name)
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