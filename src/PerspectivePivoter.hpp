//
// Created by Altan on 5/27/2023.
//

#ifndef PERSPECTIVEPIVOTER_HPP
#define PERSPECTIVEPIVOTER_HPP

// Include the single-file, header-only middleware libcluon to create
// high-performance microservices
#include "cluon-complete-v0.0.127.hpp"

// Include the GUI and image processing header files from OpenCV
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class PerspectivePivoter {
  public:
    cv::Mat calculateMatrix();
    cv::Mat changeView(cv::Mat image);
    std::vector<cv::Point2f> convertPoints( std::vector<cv::Point2f> coordinates);
};


#endif // PERSPECTIVEPIVOTER_HPP
