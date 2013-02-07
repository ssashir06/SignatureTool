#pragma once
#include <string>
#include <opencv2/opencv.hpp>

static const std::string path(".\\");
static const std::string file_info[][3] = {
	{"2012_11_13\\IMG_0001.jpg", "ラディアンサ", "山田"},
	{"2012_11_13\\IMG_0002.jpg", "ニイシ", "山田"},
	{"2012_11_13\\IMG_0003.jpg", "ディネス", "山田"},
	{"2012_11_13\\IMG_0004.jpg", "甲田", "山田"},
	{"2012_11_13\\IMG_0005.jpg", ".", "山田"},
	{"2012_11_13\\IMG_0006.jpg", "マリ", "山田"},
	{"2012_11_13\\IMG_0007.jpg", "白井", "山田"},
	{"2012_11_13\\IMG_0008.jpg", "市川", "山田"},
	{"2012_11_13\\IMG_0009.jpg", ".", "山田"},
	{"2012_11_13\\IMG_0010.jpg", "グルン", "山田"},
	{"2012_11_13\\IMG_0011.jpg", "コイデ", "山田"},
	{"2012_11_13\\IMG_0012.jpg", "宮尾", "山田"},
	{"2012_11_13\\IMG_0013.jpg", "ラディアンサ", "山田"},
	{"2012_11_13\\IMG_0014.jpg", "市川", "山田"},
	{"2012_11_13\\IMG_0015.jpg", ".", "山田"},
	{"2012_11_13\\IMG_0016.jpg", "マリ", "ろくかわ"},
	{"2012_11_13\\IMG_0017.jpg", "甲田", "ろくかわ"},
	{"2012_11_13\\IMG_0018.jpg", "白井", "小林"},
	{"2012_11_13\\IMG_0019.jpg", "コイデ", "アサノ"},
	{"2012_11_13\\IMG_0020.jpg", "山崎", "アサノ"},
	{"2012_11_13\\IMG_0021.jpg", "甲田", "小林"},
	{"2012_11_13\\IMG_0022.jpg", "ろくかわ", "山田"},
	{"2012_11_13\\IMG_0023.jpg", "山崎", "小平"},
	{"2012_11_13\\IMG_0024.jpg", "甲田", "山田"},
	{"2012_11_13\\IMG_0025.jpg", "マリ", "山田"},
//#ifndef _DEBUG
#if 1
	{"2012_11_13\\IMG_0026.jpg", "ニイシ", "山田"},
	{"2012_11_13\\IMG_0027.jpg", "ディネス", "山田"},
	{"2012_11_13\\IMG_0028.jpg", "白井", "山田"},
	{"2012_11_13\\IMG_0029.jpg", "宮尾", "山田"},
	{"2012_11_13\\IMG_0030.jpg", "ラディアンサ", "山田"},
	{"2012_11_13\\IMG_0031.jpg", "ろくかわ", "山田"},
	{"2012_11_13\\IMG_0032.jpg", "グルン", "山田"},
	{"2012_11_13\\IMG_0033.jpg", "マリ", "山田"},
	{"2012_11_13\\IMG_0034.jpg", "市川", "山田"},
	{"2012_11_13\\IMG_0035.jpg", "甲田", "小平"},
	{"2012_11_13\\IMG_0036.jpg", "コイデ", "小平"},
	{"2012_11_13\\IMG_0037.jpg", "コイデ", "."},
	{"2012_11_13\\IMG_0038.jpg", "山田光", "小平"},
	{"2012_11_13\\IMG_0039.jpg", "マリ", "小平"},
	{"2012_11_13\\IMG_0040.jpg", "ラディアンサ", "小平"},
	{"2012_11_13\\IMG_0041.jpg", "白井", "山田"},
	{"2012_11_13\\IMG_0042.jpg", "山崎", "小平"},
	{"2012_11_13\\IMG_0043.jpg", "ニイシ", "小平"},
	{"2012_11_13\\IMG_0044.jpg", "グルン", "小平"},
	{"2012_11_13\\IMG_0045.jpg", "グルン", "小平"},
	{"2012_11_13\\IMG_0046.jpg", "白井", "小平"},
	{"2012_11_13\\IMG_0047.jpg", "力山", "小平"},
	{"2012_11_13\\IMG_0048.jpg", "ろくかわ", "小平"},
	{"2012_11_13\\IMG_0049.jpg", "宮尾", "小林"},
	{"2012_11_13\\IMG_0050.jpg", "山崎", "山田"},
	{"2012_11_13\\IMG_0051.jpg", "コイデ", "山田"},
	{"2012_11_13\\IMG_0052.jpg", "マリ", "山田"},
	{"2012_11_13\\IMG_0053.jpg", "ろくかわ", "山田"},
	{"2012_11_13\\IMG_0054.jpg", "ニイシ", "山田"},
	{"2012_11_13\\IMG_0055.jpg", "グルン", "山田"},
	{"2012_11_13\\IMG_0056.jpg", "グルン", "山田"},
#endif
};

#if 0
static const cv::Rect trim_area[] = {
	cv::Rect(1800, 0, 675, 190),	//集荷者
	cv::Rect(1800, 140, 675, 190),	//検品者
};
#elif 1
static const cv::Rect trim_area[] = {
	cv::Rect(1800, 30, 675, 160),	//集荷者
	cv::Rect(1800, 170, 675, 160),	//検品者
};
#elif 1
static const cv::Rect trim_area[] = {
	cv::Rect(2000, 0, 475, 190),	//集荷者
	cv::Rect(2000, 140, 475, 190),	//検品者
};
#else
static const cv::Rect trim_area[] = {
	cv::Rect(2000, 30, 475, 160),	//集荷者
	cv::Rect(2000, 170, 475, 160),	//検品者
};
#endif
