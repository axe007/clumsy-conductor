//
// Scribbled on by Group 16, Class of 2023
//

#include "ShapeSniffer.hpp"
#include "PerspectivePivoter.hpp"

std::vector<cv::Point2f> ShapeSniffer::processContours(const std::vector<std::vector<cv::Point>>& contours, const cv::Mat& original, const cv::Scalar& color) {
    PerspectivePivoter perspectivePivoter;
    const float CLUSTER_RANGE = 100.0;

    std::vector<cv::Moments> mu(contours.size());
    for (unsigned int i = 0; i < contours.size(); i++) {
        mu[i] = moments(contours[i], false);
    }

    std::vector<cv::Point2f> mc(contours.size());
    std::vector<cv::Point2f> mcTransformed(contours.size());
    for (unsigned int i = 0; i < contours.size(); i++) {
        mc[i] = cv::Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
        mc[i].y = mc[i].y + 240;
        mcTransformed = perspectivePivoter.convertPoints(mc);
    }

    if (mcTransformed.size() >= 2) {
        for (unsigned int i = 0; i < (mcTransformed.size() - 1); i++) {
            float y_value = mcTransformed[i].y - mcTransformed[i + 1].y;
            if (y_value < CLUSTER_RANGE) {
                float x_value = mcTransformed[i].x - mcTransformed[i + 1].x;
                if (x_value < CLUSTER_RANGE) {
                    mcTransformed.erase(mcTransformed.begin() + i);
                    i--;
                }
            }
        }
    }

    cv::Mat birdsEyeView = perspectivePivoter.changeView(original);
    for (unsigned int i = 0; i < contours.size(); i++) {
        cv::circle(birdsEyeView, mcTransformed[i], 10, color, 1, CV_AA, 0);
    }

    return mcTransformed;
}

std::pair<std::vector<cv::Point2f>, std::vector<cv::Point2f>> ShapeSniffer::findContours(std::pair<cv::Mat, cv::Mat> image, cv::Mat original) {
    std::vector<std::vector<cv::Point>> contoursYellow;
    std::vector<cv::Vec4i> hierarchyYellow;
    cv::findContours(image.first, contoursYellow, hierarchyYellow, CV_RETR_TREE,
                     CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    std::vector<cv::Point2f> mcTransformedYellow = processContours(contoursYellow, original, cv::Scalar(0, 0, 255));

    std::vector<std::vector<cv::Point>> contoursBlue;
    std::vector<cv::Vec4i> hierarchyBlue;
    cv::findContours(image.second, contoursBlue, hierarchyBlue, CV_RETR_TREE,
                     CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    std::vector<cv::Point2f> mcTransformedBlue = processContours(contoursBlue, original, cv::Scalar(0, 0, 255));

    std::pair<std::vector<cv::Point2f>, std::vector<cv::Point2f>> coordinates =
            std::make_pair(mcTransformedYellow, mcTransformedBlue);

    return coordinates;
}
