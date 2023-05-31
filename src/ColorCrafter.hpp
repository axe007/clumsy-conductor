//
// Scribbled on by Group 16, Class of 2023
//

#ifndef COLORCRAFTER
#define COLORCRAFTER

#include <utility>
#include <opencv2/core/core.hpp>

// Import the header-only middleware library, libcluon, to build high-performance microservices.
#include "cluon-complete-v0.0.127.hpp"

// Incorporate the GUI and image processing headers from OpenCV.
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class ColorCrafter {
    public: 
        std::pair<cv::Mat, cv::Mat> processColors(cv::Mat image);

};

#endif //COLORCRAFTER
