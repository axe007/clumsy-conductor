//
// Scribbled on by Group 16, Class of 2023
//

#include "ShapeSniffer.hpp"
#include "PerspectivePivoter.hpp"

std::pair<std::vector<cv::Point2f>, std::vector<cv::Point2f>>
ShapeSniffer::findContours(std::pair<cv::Mat, cv::Mat> image, cv::Mat original) {
    PerspectivePivoter perspectivePivoter;
    const float CLUSTER_RANGE = 100.0;

    cv::Mat birdsEyeView = perspectivePivoter.changeView(original);

    // Yellow contours
    std::vector<std::vector<cv::Point>> contoursYellow;
    std::vector<cv::Vec4i> hierarchyYellow;
    cv::findContours(image.first, contoursYellow, hierarchyYellow, CV_RETR_TREE,
                     CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    // Get the moments
    std::vector<cv::Moments> muYellow(contoursYellow.size());
    for (unsigned int i = 0; i < contoursYellow.size(); i++) {
        muYellow[i] = moments(contoursYellow[i], false);
    }

    // Get center of mass
    std::vector<cv::Point2f> mcYellow(contoursYellow.size());
    std::vector<cv::Point2f> mcTransformedYellow(contoursYellow.size());
    for (unsigned int i = 0; i < contoursYellow.size(); i++) {
        mcYellow[i] = cv::Point2f(muYellow[i].m10 / muYellow[i].m00,
                                muYellow[i].m01 / muYellow[i].m00);
        mcYellow[i].y = mcYellow[i].y + 240;
        mcTransformedYellow = perspectivePivoter.convertPoints(mcYellow);
    }


    // Check for clusters
    if (mcTransformedYellow.size() >= 2) {
        for (unsigned int i = 0; i < (mcTransformedYellow.size() - 1); i++) {
            float y_value = mcTransformedYellow[i].y - mcTransformedYellow[i + 1].y;

            if (y_value < CLUSTER_RANGE) {
                float x_value = mcTransformedYellow[i].x - mcTransformedYellow[i + 1].x;

                if (x_value < CLUSTER_RANGE) {
                    // if close, remove then one from the top of the frame
                    mcTransformedYellow.erase(mcTransformedYellow.begin() + i);
                    i--;
                }
            }
        }
    }

    // Draw yellow contour shapes
    for (unsigned int i = 0; i < contoursYellow.size(); i++) {
        cv::circle(birdsEyeView, mcTransformedYellow[i], 10, cv::Scalar(0, 0, 255), 1, CV_AA, 0);
    }

    // Blue contour
    std::vector<std::vector<cv::Point>> contoursBlue;
    std::vector<cv::Vec4i> hierarchyBlue;
    cv::findContours(image.second, contoursBlue, hierarchyBlue, CV_RETR_TREE,
                     CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    // Get the moments
    std::vector<cv::Moments> muBlue(contoursBlue.size());
    for (unsigned int i = 0; i < contoursBlue.size(); i++) {
        muBlue[i] = moments(contoursBlue[i], false);
    }

    //  Get center of mass
    std::vector<cv::Point2f> mcBlue(contoursBlue.size());
    std::vector<cv::Point2f> mcTransformedBlue(contoursBlue.size());
    for (unsigned int i = 0; i < contoursBlue.size(); i++) {
        mcBlue[i] = cv::Point2f(muBlue[i].m10 / muBlue[i].m00,
                                 muBlue[i].m01 / muBlue[i].m00);
        mcBlue[i].y = mcBlue[i].y + 240;
        mcTransformedBlue = perspectivePivoter.convertPoints(mcBlue);
    }

    // Check for clusters
    if (mcTransformedBlue.size() >= 2) {

        for (unsigned int i = 0; i < (mcTransformedBlue.size() - 1); i++) {
            float y_value = mcTransformedBlue[i].y - mcTransformedBlue[i + 1].y;

            if (y_value < CLUSTER_RANGE) {
                float x_value = mcTransformedBlue[i].x - mcTransformedBlue[i + 1].x;

                if (x_value < CLUSTER_RANGE) {
                    // if close, remove then one from the top of the frame
                    mcTransformedBlue.erase(mcTransformedBlue.begin() + i);
                    i--;
                }
            }
        }
    }

    // Draw blue contour shapes
    for (unsigned int i = 0; i < contoursBlue.size(); i++) {
        cv::circle(birdsEyeView, mcTransformedBlue[i], 10, cv::Scalar(0, 0, 255), 1, CV_AA, 0);
    }

    // Prep set of coordinates
    std::pair<std::vector<cv::Point2f>, std::vector<cv::Point2f>> coordinates =
            std::make_pair(mcTransformedYellow, mcTransformedBlue);

    return coordinates;
}