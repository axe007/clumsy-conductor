//
// Scribbled on by Group 16, Class of 2023
//

#ifndef ANGLECALCULATOR
#define ANGLECALCULATOR

// Import the header-only middleware library, libcluon, to build high-performance microservices.
#include "cluon-complete-v0.0.127.hpp"

// Incorporate the GUI and image processing headers from OpenCV.
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class AngleCalculator {
    public:
        float getAngleBetweenCones(std::vector<cv::Point2f> points);
        float getAngleOfRoad(std::vector<cv::Point2f> yellow, std::vector<cv::Point2f> blue);
        float distanceBetweenPoints(float xInPoint1, float yInPoint1, float xInPoint2, float yInPoint2);
};

#endif // ANGLECALCULATOR