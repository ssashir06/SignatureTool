#pragma once
#include <map>
#include <tuple>
#include <opencv2/opencv.hpp>

namespace CVUtil
{
	namespace SaveLoadCV
	{
#pragma region std::map 
		template<typename Tk, typename Tv>
		cv::FileStorage& operator<<(cv::FileStorage& fs, const std::map<Tk, Tv>& map)
		{
			if(!fs.isOpened()) return fs;

			fs << "[";
			for (const auto& item : map)
			{
				fs << "{:";
				fs << "key" << item.first;
				fs << "value" << item.second;
				fs << "}";
			}
			fs << "]";
			return fs;
		}
		template<typename Tk, typename Tv>
		cv::FileNodeIterator& operator>>(cv::FileNodeIterator& it, std::map<Tk, Tv>& map)
		{
			Tk key;
			Tv value;
			(*it)["key"] >> key;
			(*it)["value"] >> value;
			map[key] = value;
			return it;
		}
		template<typename Tk, typename Tv>
		void operator>>(const cv::FileNode& fn, std::map<Tk, Tv>& map)
		{
			map.clear();
			for (auto it = fn.begin(); it != fn.end(); it++)
				it >> map;
		}
#pragma endregion
#pragma region std::tuple
		//TODO
#pragma endregion
#pragma region std::pair
		//TODO
#pragma endregion
	}
}
