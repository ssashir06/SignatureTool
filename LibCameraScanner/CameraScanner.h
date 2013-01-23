#include <string>
#include <vector>
#include <list>
#include <opencv2/opencv.hpp>

namespace CameraScanner
{
	typedef cv::Point2f Edge;
	typedef std::vector<Edge> Edges;
	struct ScanSpec
	{
		static const double mm_per_inch;//25.4mm/inch
		double width_mm, height_mm;
		double pixel_per_mm;
		ScanSpec();
		ScanSpec(double width, double height, double ppi);
	};

	class Reshape
	{
	protected:
		cv::Mat img_src, img_target, img_bin;
		std::vector<cv::Vec2f> side_lines;
		Edge paper_center;
		cv::Size small_size;
		Edges edges, edges_learge;
		ScanSpec spec;

	public:
		Reshape();
		Reshape(const ScanSpec& spec, const cv::Size small_size = cv::Size(640, 480));
		Reshape(const Reshape& src);
		Reshape& operator=(const Reshape& src);
		virtual ~Reshape();

		void prepare(const cv::Mat& image);
		cv::Mat scan();
		cv::Mat drawSideLines() const;
		cv::Mat drawSideLines(cv::Mat& img) const;

	protected:
		void reset();
		void setImageSource(const cv::Mat& image);
		void makeBinaryColor();
		void guessSideLines();
		void getPaperIntersectionEdges();
		void fitToPaperCorner();
		cv::Mat trimPaper() const;
	};

	cv::Mat smallerImage(const cv::Mat& img, int width = 640, int height = 480, bool fast = false);
	void trimBorder(cv::Mat& img, int border_width = 5, int border_height = 5);
	void resizeEdges(Edges& edges, double zoom = 1.0);
	void drawEdges(cv::Mat& image, const Edges& edges, const cv::Scalar& color_lines, const cv::Scalar& color_intersections);
	void showDebugImage(const cv::Mat& img, const std::string& title = std::string("image"));
}
