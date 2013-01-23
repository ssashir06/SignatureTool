#include <algorithm>
#include <iostream>
#include <string>
#include <list>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "../LibCameraScanner/CameraScanner.h"

using namespace std;
using namespace cv;
using namespace CameraScanner;

Mat takePicture(const ScanSpec& spec, Reshape& scanner, int width, int height)
{
	VideoCapture cap(0);
	if (!cap.isOpened()) throw "Camera is not connected";

	cap.set(CV_CAP_PROP_FRAME_WIDTH, width);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, height);

	const char const *window = "Capturing";
	namedWindow(window);
	Mat img_tmp, img_cap, img_preview;
	int frame = 0;
	Edges edges;
	do {
		cap >> img_tmp;
		img_cap = img_tmp.clone();

		trimBorder(img_cap);

		if (frame == 0) {
			try {
				scanner.prepare(img_cap);
			} catch (...) {
				// do nothing
			}
			img_preview = scanner.drawSideLines();
		}
		drawEdges(img_preview, edges, CV_RGB(255, 0, 0), CV_RGB(0, 255, 0));
		imshow(window, img_preview);
		frame = (frame + 1) % 10;
	} while (waitKey(10) == -1);
	destroyWindow(window);
	return img_cap.clone();
}

int main()
{
	ScanSpec spec_a4(210, 297, 300);
	Reshape scanner(spec_a4);
#if 1
	Mat img_src = takePicture(spec_a4, scanner, 1600, 1200);
	imwrite("taken.png", img_src);
#else
	Mat img_src = imread("taken.png");
	if (img_src.empty()) exit(1);
#endif

	scanner.prepare(img_src);
	//Mat img_preview = img_src.clone();
	//drawEdges(img_preview, edges, CV_RGB(255, 0, 0));
	//showDebugImage(smallerImage(img_preview, 640, 480));

	Mat a4_paper = scanner.scan();
	showDebugImage(smallerImage(a4_paper, 640, 480), "A4");
	imwrite("a4.png", a4_paper);
}
