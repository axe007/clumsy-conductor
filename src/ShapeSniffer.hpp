//
// Scribbled on by Group 16, Class of 2023
//

#ifndef SHAPESNIFFER
#define SHAPESNIFFER

// Include the single-file, header-only middleware libcluon to create
// high-performance microservices
#include "cluon-complete-v0.0.127.hpp"

// Include the GUI and image processing header files from OpenCV
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class ShapeSniffer {
    public:
        std::pair<std::vector<cv::Point2f>,
        std::vector<cv::Point2f>> findContours(std::pair<cv::Mat, cv::Mat> image, cv::Mat original);
};

#endif // SHAPESNIFFER
