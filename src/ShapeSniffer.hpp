//
// Scribbled on by Group 16, Class of 2023
//

#ifndef SHAPESNIFFER
#define SHAPESNIFFER

// Import the header-only middleware library, libcluon, to build high-performance microservices.
#include "cluon-complete-v0.0.127.hpp"

// Incorporate the GUI and image processing headers from OpenCV.
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class ShapeSniffer {
    public:
        std::pair<std::vector<cv::Point2f>,
                std::vector<cv::Point2f>> findContours(std::pair<cv::Mat, cv::Mat> image, cv::Mat original);

    private:
        std::vector<cv::Point2f> processContours(const std::vector<std::vector<cv::Point>>& contours, const cv::Mat& original, const cv::Scalar& color);
};

#endif // SHAPESNIFFER
