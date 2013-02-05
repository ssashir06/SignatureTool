#pragma once
#include <string>
#include <opencv2/opencv.hpp>

static const std::string path(".\\");
static const std::string file_info[][3] = {
	{"2012_11_13\\IMG_0001.jpg", "���f�B�A���T", "�R�c"},
	{"2012_11_13\\IMG_0002.jpg", "�j�C�V", "�R�c"},
	{"2012_11_13\\IMG_0003.jpg", "�f�B�l�X", "�R�c"},
	{"2012_11_13\\IMG_0004.jpg", "�b�c", "�R�c"},
	{"2012_11_13\\IMG_0005.jpg", ".", "�R�c"},
	{"2012_11_13\\IMG_0006.jpg", "�}��", "�R�c"},
	{"2012_11_13\\IMG_0007.jpg", "����", "�R�c"},
	{"2012_11_13\\IMG_0008.jpg", "�s��", "�R�c"},
	{"2012_11_13\\IMG_0009.jpg", ".", "�R�c"},
	{"2012_11_13\\IMG_0010.jpg", "�O����", "�R�c"},
	{"2012_11_13\\IMG_0011.jpg", "�R�C�f", "�R�c"},
	{"2012_11_13\\IMG_0012.jpg", "�{��", "�R�c"},
	{"2012_11_13\\IMG_0013.jpg", "���f�B�A���T", "�R�c"},
	{"2012_11_13\\IMG_0014.jpg", "�s��", "�R�c"},
	{"2012_11_13\\IMG_0015.jpg", ".", "�R�c"},
	{"2012_11_13\\IMG_0016.jpg", "�}��", "�낭����"},
	{"2012_11_13\\IMG_0017.jpg", "�b�c", "�낭����"},
	{"2012_11_13\\IMG_0018.jpg", "����", "����"},
	{"2012_11_13\\IMG_0019.jpg", "�R�C�f", "�A�T�m"},
	{"2012_11_13\\IMG_0020.jpg", "�R��", "�A�T�m"},
	{"2012_11_13\\IMG_0021.jpg", "�b�c", "����"},
	{"2012_11_13\\IMG_0022.jpg", "�낭����", "�R�c"},
	{"2012_11_13\\IMG_0023.jpg", "�R��", "����"},
	{"2012_11_13\\IMG_0024.jpg", "�b�c", "�R�c"},
	{"2012_11_13\\IMG_0025.jpg", "�}��", "�R�c"},
//#ifndef _DEBUG
#if 1
	{"2012_11_13\\IMG_0026.jpg", "�j�C�V", "�R�c"},
	{"2012_11_13\\IMG_0027.jpg", "�f�B�l�X", "�R�c"},
	{"2012_11_13\\IMG_0028.jpg", "����", "�R�c"},
	{"2012_11_13\\IMG_0029.jpg", "�{��", "�R�c"},
	{"2012_11_13\\IMG_0030.jpg", "���f�B�A���T", "�R�c"},
	{"2012_11_13\\IMG_0031.jpg", "�낭����", "�R�c"},
	{"2012_11_13\\IMG_0032.jpg", "�O����", "�R�c"},
	{"2012_11_13\\IMG_0033.jpg", "�}��", "�R�c"},
	{"2012_11_13\\IMG_0034.jpg", "�s��", "�R�c"},
	{"2012_11_13\\IMG_0035.jpg", "�b�c", "����"},
	{"2012_11_13\\IMG_0036.jpg", "�R�C�f", "����"},
	{"2012_11_13\\IMG_0037.jpg", "�R�C�f", "."},
	{"2012_11_13\\IMG_0038.jpg", "�R�c��", "����"},
	{"2012_11_13\\IMG_0039.jpg", "�}��", "����"},
	{"2012_11_13\\IMG_0040.jpg", "���f�B�A���T", "����"},
	{"2012_11_13\\IMG_0041.jpg", "����", "�R�c"},
	{"2012_11_13\\IMG_0042.jpg", "�R��", "����"},
	{"2012_11_13\\IMG_0043.jpg", "�j�C�V", "����"},
	{"2012_11_13\\IMG_0044.jpg", "�O����", "����"},
	{"2012_11_13\\IMG_0045.jpg", "�O����", "����"},
	{"2012_11_13\\IMG_0046.jpg", "����", "����"},
	{"2012_11_13\\IMG_0047.jpg", "�͎R", "����"},
	{"2012_11_13\\IMG_0048.jpg", "�낭����", "����"},
	{"2012_11_13\\IMG_0049.jpg", "�{��", "����"},
	{"2012_11_13\\IMG_0050.jpg", "�R��", "�R�c"},
	{"2012_11_13\\IMG_0051.jpg", "�R�C�f", "�R�c"},
	{"2012_11_13\\IMG_0052.jpg", "�}��", "�R�c"},
	{"2012_11_13\\IMG_0053.jpg", "�낭����", "�R�c"},
	{"2012_11_13\\IMG_0054.jpg", "�j�C�V", "�R�c"},
	{"2012_11_13\\IMG_0055.jpg", "�O����", "�R�c"},
	{"2012_11_13\\IMG_0056.jpg", "�O����", "�R�c"},
#endif
};
#if 0
static const cv::Rect trim_area_train[] = {
	cv::Rect(2000, 0, 475, 190),	//�W�׎�
	cv::Rect(2000, 140, 475, 190),	//���i��
};
static const cv::Rect trim_area_query[] = {
	cv::Rect(1800, 0, 675, 190),	//�W�׎�
	cv::Rect(1800, 140, 675, 190),	//���i��
};
#else
static const cv::Rect trim_area_train[] = {
	cv::Rect(2000, 30, 475, 160),	//�W�׎�
	cv::Rect(2000, 170, 475, 160),	//���i��
};
static const cv::Rect trim_area_query[] = {
	cv::Rect(2000, 30, 475, 160),	//�W�׎�
	cv::Rect(2000, 170, 475, 160),	//���i��
};
#endif