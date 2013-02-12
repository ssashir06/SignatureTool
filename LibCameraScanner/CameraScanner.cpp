#include <iostream>
#include <vector>
#include <list>
#include <iostream>
#include <sstream>
#include <opencv2/opencv.hpp>
#include "CameraScanner.h"
#include "Line.h"
using namespace std;
using namespace cv;

namespace CameraScanner
{
#pragma region local functions
#pragma region compare functions for sorting algorithms
	static bool compareEdgePair(const pair<double, Edge>& v1, const pair<double, Edge>& v2)
	{
		return v1.first < v2.first;
	}

	static bool compareIntPair(const pair<double, int>& v1, const pair<double, int>& v2)
	{
		return v1.first < v2.first;
	}
#pragma endregion
	static vector<pair<Line::P, Line::P> > convertLineArray(const vector<Vec2f>& lines, double length)
	{
		vector<pair<Line::P, Line::P> > lp(lines.size());
		for (int i=0; i<lines.size(); i++) {
			double rho = lines[i][0], theta = lines[i][1];
			Line::P s(rho * cos(theta), rho * sin(theta));
			Line::P t(length * -sin(theta), length * cos(theta));
			lp[i] = pair<Line::P, Line::P>(s + t, s - t);
		}
		return lp;
	}

	static void drawLines(Mat& img, const vector<pair<Line::P, Line::P> >&lines, const Scalar& color, int thickness)
	{
		for (int i=0; i<lines.size(); i++)
			line(img, 
				Point(saturate_cast<int>(lines[i].first.real()), saturate_cast<int>(lines[i].first.imag())), 
				Point(saturate_cast<int>(lines[i].second.real()), saturate_cast<int>(lines[i].second.imag())), 
				color, thickness, CV_AA);
	}

	static void moveLines(vector<Vec2f>& lines, const Edge& v)
	{
		for (int i=0; i<lines.size(); i++) {
			Vec2f& line = lines[i];
			line[0] -= v.x * cos(line[1]) + v.y * sin(line[1]);
		}
	}
#pragma endregion

#pragma region tools
	Mat smallerImage(const Mat& img, int width, int height, bool fast)
	{
		if (width < img.cols || height < img.rows) {
			int n_width = min((double)img.cols * height / img.rows, (double)width);
			int n_height = min((double)img.rows * width / img.cols, (double)height);
			Mat img_small;
			resize(img, img_small, Size(n_width, n_height), 0, 0, fast ? INTER_NEAREST : INTER_CUBIC);
			return img_small;
		} else {
			return img;
		}
	}

	void trimBorder(Mat& img, int border_width, int border_height)
	{
		if (img.empty()) return;
		int n_width = img.cols - 2 * border_width;
		int n_height = img.rows - 2 * border_height;
		if (n_width <= 0 || n_height <= 0) throw L"image is too small";

		img = Mat(img, Rect(border_width, border_height, n_width, n_height));
	}

	void resizeEdges(Edges& edges, double zoom)
	{
		for (Edges::iterator it=edges.begin(); it!=edges.end(); it++)
			(*it) *= zoom;
	}


	void drawEdges(Mat& image, const Edges& edges, const cv::Scalar& color_lines, const cv::Scalar& color_intersections)
	{
		for (int i=0; i<edges.size(); i++) {
			circle(image, edges[i], 3, color_intersections, 1, CV_AA);
			for (int j=i+1; j<edges.size(); j++)
				line(image, edges[i], edges[j], color_lines, 1, CV_AA);
		}
	}

	void showDebugImage(const Mat& img, const string& title)
	{
		namedWindow(title);
		imshow(title, img);
		waitKey();
		destroyWindow(title);
	}
#pragma endregion

#pragma region ScanSpec struct
	const double ScanSpec::mm_per_inch = 25.4;
	ScanSpec::ScanSpec()
	{
	}

	ScanSpec::ScanSpec(double width, double height, double ppi)
	{
		this->width_mm = width;
		this->height_mm = height;
		this->pixel_per_mm = ppi / mm_per_inch;
	}
#pragma endregion

#pragma region Reshape class
	Reshape::Reshape()
		: small_size(Size(640, 480))
	{
	}

	Reshape::Reshape(const ScanSpec& spec, const Size small_size)
		: spec(spec), small_size(small_size)
	{
	}

	Reshape::Reshape(const Reshape& src)
	{
		img_src = src.img_src;
		img_target = src.img_target;
		img_bin = src.img_bin;
		side_lines = src.side_lines;
		side_lines_candidate = src.side_lines_candidate;
		paper_center = src.paper_center;
		edges = src.edges;
		edges_learge = src.edges_learge;
		small_size = src.small_size;
		spec = src.spec;
	}

	Reshape& Reshape::operator=(const Reshape& src)
	{
		img_src = src.img_src;
		img_target = src.img_target;
		img_bin = src.img_bin;
		side_lines = src.side_lines;
		side_lines_candidate = src.side_lines_candidate;
		paper_center = src.paper_center;
		edges = src.edges;
		edges_learge = src.edges_learge;
		small_size = src.small_size;
		spec = src.spec;
		return *this;
	}

	Reshape::~Reshape()
	{
	}

	void Reshape::prepare(const Mat& image)
	{
		reset();
		try {
			setImageSource(image);
			makeBinaryColor();
			guessSideLines();
			decideIntersectionEdges();
		} catch (const char* const msg) {
			stringstream ss_msg;
			ss_msg << L"Preparing scan was failed: " << msg;
			cerr << ss_msg << endl;
			throw ss_msg.str().c_str();
		}
	}

	Mat Reshape::scan()
	{
		try {
			fitToPaperCorner();
			return trimPaper();
		} catch (const char* const msg) {
			stringstream ss_msg;
			ss_msg << L"Scanning was failed: " << msg;
			cerr << ss_msg << endl;
			throw ss_msg.str().c_str();
		}
	}

	Mat Reshape::drawSideLines(Mat& img, bool with_candidates) const
	{
		if (img.empty()) throw L"image is not set?";

		if (with_candidates)
			drawLines(img, convertLineArray(side_lines_candidate, img_src.rows * 3), CV_RGB(128, 128, 128), 1);

		drawLines(img, convertLineArray(side_lines, img_src.rows * 3), CV_RGB(0, 0, 255), 1);
		drawEdges(img, edges, CV_RGB(255, 0, 0), CV_RGB(0, 255, 0));
		return img;
	}

	Mat Reshape::drawSideLines(bool with_candidates) const
	{
		return drawSideLines(img_target.clone(), with_candidates);
	}

	Edges Reshape::getPaperEdges() const
	{
		return edges;
	}

	Edge Reshape::getPaperCenter() const
	{
		return paper_center;
	}

	ScanSpec Reshape::getScanSpec() const
	{
		return spec;
	}

	void Reshape::reset()
	{
		side_lines.clear();
		edges.clear();
	}

	void Reshape::setImageSource(const Mat& image)
	{
		if (image.channels() != 3) throw L"invalid image";
		img_src = image;

		if (img_src.cols < small_size.width && img_src.rows < small_size.height) {
			img_target = img_src.clone();
		} else {
			resize(img_src, img_target, small_size);
		}
		paper_center = Edge(img_target.cols/2, img_target.rows/2);
	}

	void Reshape::makeBinaryColor()
	{
		Mat median;
		medianBlur(img_target, median, (int)(img_target.cols * 0.01717 * 1.5 / 2) * 2 + 1);
		cvtColor(median, median, CV_BGR2HSV);

		Mat mask = Mat::zeros(median.rows + 2, median.cols + 2, CV_8UC1);
		Scalar hsv_diff = CV_RGB(2, 20, 20);
		floodFill(median, mask, Point(median.cols/2, median.rows/2), CV_RGB(0, 0, 0), nullptr, hsv_diff, hsv_diff, FLOODFILL_MASK_ONLY | 255<<8 | 4);
		erode(mask, mask, Mat());

		Mat masked;
		vector<Mat> hsv;
		split(median, hsv);

		hsv[2].copyTo(masked, Mat(mask, Rect(1, 1, median.cols, median.rows)));

		Canny(masked, img_bin, 10, 50);
	}

	void Reshape::guessSideLines()
	{
		if (img_bin.channels() != 1 || img_bin.depth() != CV_8U) throw L"invalid image";

		// hough
		vector<Vec2f> hough_lines;
		HoughLines(img_bin, hough_lines, 1, CV_PI/180, 90, 0, 0);

//show preview
#if 0
		Mat preview;
		cvtColor(img_bin, preview, CV_GRAY2BGR);
		drawLines(preview, convertLineArray(hough_lines, 1000), CV_RGB(0, 255, 0), 1);
		resize(preview, preview, Size(640, 480));
		namedWindow("hough");
		imshow("hough", preview);
		waitKey();
		destroyWindow("hough");
#endif

		// clustering
		const int cluster_count = 4;
		Mat clusters_mat, lines_center_m, lines_mat = Mat_<float>(hough_lines.size(), 3);

		if (hough_lines.size() < cluster_count) throw L"too a few side lines";

		moveLines(hough_lines, paper_center);
		side_lines_candidate.resize(hough_lines.size());
		for (int i=0; i<hough_lines.size(); i++) {
			if (hough_lines[i][0] < 0) {
				hough_lines[i][0] *= -1;
				hough_lines[i][1] += CV_PI;
			}
			side_lines_candidate[i] = Vec2f(hough_lines[i][0], hough_lines[i][1]);
		}
		moveLines(side_lines_candidate, -paper_center);

		for (int i=0; i<hough_lines.size(); i++) {
			lines_mat.at<float>(i, 0) = hough_lines[i][0];
			lines_mat.at<float>(i, 1) = hough_lines[i][0] * cos(hough_lines[i][1]);
			lines_mat.at<float>(i, 2) = hough_lines[i][0] * sin(hough_lines[i][1]);
		}

		kmeans(lines_mat, cluster_count, clusters_mat, TermCriteria(CV_TERMCRIT_EPS|CV_TERMCRIT_ITER, 50, 1.0), 1, KMEANS_PP_CENTERS, lines_center_m);

		side_lines = vector<Vec2f>(cluster_count, Vec2f(0, 0));
		for (int i=0; i<cluster_count; i++) {
			float p = lines_center_m.at<float>(i, 0);
			float c = lines_center_m.at<float>(i, 1);
			float s = lines_center_m.at<float>(i, 2);
			side_lines[i] = Vec2f(p, atan2(s, c));
		}
		moveLines(side_lines, -paper_center);
	}

	void Reshape::decideIntersectionEdges()
	{
		double length = max(img_target.cols, img_target.rows) * 2;
		vector<pair<Line::P, Line::P> > lines_p_center = convertLineArray(side_lines, length);

		list<pair<double, Edge> > edges_w_distance;

		for (int i=0; i<lines_p_center.size(); i++) for (int j=i+1; j<lines_p_center.size(); j++) {
			Line::P &a1 = lines_p_center[i].first, &a2 = lines_p_center[i].second;
			Line::P &b1 = lines_p_center[j].first, &b2 = lines_p_center[j].second;
			if (!Line::is_intersected_ls(a1, a2, b1, b2)) continue;
			Line::P c = Line::intersection_ls(a1, a2, b1, b2);
			Edge p = Point2d(c.real(), c.imag());
			double d = sqrt((paper_center - p).ddot(paper_center - p));
			edges_w_distance.push_back(pair<double, Edge>(d, p));
		}

		// sort by the distance from the center of the paper and cut off over 4 points
		edges_w_distance.sort(compareEdgePair);
		edges.clear();
		for (list<pair<double, Edge> >::const_iterator it=edges_w_distance.begin(); it!=edges_w_distance.end() && edges.size() < 4; it++) {
			edges.push_back(it->second);
		}

		edges_learge = edges;
		resizeEdges(edges_learge, (double)img_src.rows / img_target.rows);
	}

	void Reshape::fitToPaperCorner()
	{
		if (edges_learge.empty()) return;

		Mat m8u, m_hsv;
		cvtColor(img_src, m_hsv, CV_BGR2HSV);
		vector<Mat> hsv;
		split(m_hsv, hsv);
		//threshold(hsv[2], m8u, 128, 255, THRESH_BINARY);
		m8u = hsv[2];
		cornerSubPix(m8u, edges_learge, Size(10, 10)*(int)((double)img_src.cols/640), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
	}

	Mat Reshape::trimPaper() const
	{
		if (edges_learge.size() != 4) throw L"the edge size is wrong";

		// sort by angle
		vector<pair<double, Edge> > edges_w_angle(4);
		Edge center(0, 0);
		for (int i=0; i<4; i++) center += edges_learge[i];
		center.x /= 4, center.y /= 4;
		for (int i=0; i<4; i++) edges_w_angle[i] = 
			pair<double, Edge>(atan2(edges_learge[i].y-center.y, edges_learge[i].x-center.x), edges_learge[i]);
		sort(edges_w_angle.begin(), edges_w_angle.end(), compareEdgePair);

		// sort by side length
		vector<pair<double, int> > edge_ids_w_length(4);
		for (int i=0; i<4; i++) {
			Edge &a = edges_w_angle[i].second, &b = edges_w_angle[(i+1)%4].second;
			edge_ids_w_length[i] = pair<double, int>(sqrt((a-b).ddot(a-b)), i);
		}
		sort(edge_ids_w_length.begin(), edge_ids_w_length.end(), compareIntPair);

		// changed ordering by the length
		Edges src_edges(4);
		int start_edge = (spec.width_mm < spec.height_mm) ? 
			edge_ids_w_length.front().second :
			edge_ids_w_length.back().second;
		for (int i=0; i<4; i++) {
			int id = (i+start_edge)%4;
			src_edges[i] = edges_w_angle[id].second;
		}

		// scanned edge settings
		Edges dst_edges(4, Edge(0, 0));
		int dst_width = saturate_cast<int>(spec.width_mm * spec.pixel_per_mm);
		int dst_height = saturate_cast<int>(spec.height_mm * spec.pixel_per_mm);
#if 0
		dst_edges[1].x = dst_edges[2].x = dst_width;
		dst_edges[2].y = dst_edges[3].y = dst_height;
#else
		dst_edges[3].x = dst_edges[0].x = dst_width;
		dst_edges[0].y = dst_edges[1].y = dst_height;
#endif

		// homograpy
		Mat h = findHomography(src_edges, dst_edges);
		Mat out;
		warpPerspective(img_src, out, h, Size(dst_width, dst_height), INTER_CUBIC);

		return out;
	}
#pragma endregion
}
