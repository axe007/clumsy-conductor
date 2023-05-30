//
// Scribbled on by Group 16, Class of 2023
//

#ifndef NOISENEUTRALIZER
#define NOISENEUTRALIZER

// Import the header-only middleware library, libcluon, to build high-performance microservices.
#include "cluon-complete-v0.0.127.hpp"

// Incorporate the GUI and image processing headers from OpenCV.
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class NoiseNeutralizer {
    public:
        std::pair<cv::Mat,cv::Mat> reduceNoise(std::pair<cv::Mat, cv::Mat> incomingImg);
};

#endif // NOISENEUTRALIZER
