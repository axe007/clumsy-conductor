//
// Scribbled on by Group 16, Class of 2023
//

#ifndef PERSPECTIVEPIVOTER_HPP
#define PERSPECTIVEPIVOTER_HPP

// Import the header-only middleware library, libcluon, to build high-performance microservices.
#include "cluon-complete-v0.0.127.hpp"

// Incorporate the GUI and image processing headers from OpenCV.
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class PerspectivePivoter {
public:
    PerspectivePivoter();
    cv::Mat changeView(const cv::Mat &image);
    std::vector<cv::Point2f> convertPoints(const std::vector<cv::Point2f> &coordinates);

private:
    cv::Mat perspectiveMatrix;
};

#endif // PERSPECTIVEPIVOTER_HPP
