//
// Scribbled on by Group 16, Class of 2023
//

#include "NoiseNeutralizer.hpp"

std::pair<cv::Mat, cv::Mat> NoiseNeutralizer::processImage(std::pair<cv::Mat, cv::Mat> incomingImg) {
    std::pair<cv::Mat, cv::Mat> neutralizedImage;

    // Yellow cone - Fill holes in objects
    cv::dilate(incomingImg.first, neutralizedImage.first,
               cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(8, 8)));
    cv::erode(neutralizedImage.first, neutralizedImage.first,
              cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(8, 8)));

    // Yellow cone - Remove extra objects
    cv::erode(neutralizedImage.first, neutralizedImage.first,
              cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
    cv::dilate(neutralizedImage.first, neutralizedImage.first,
               cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7)));

    // Blue cone - Fill holes in objects
    cv::dilate(incomingImg.second, neutralizedImage.second,
               cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(8, 8)));
    cv::erode(neutralizedImage.second, neutralizedImage.second,
              cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(8, 8)));

    // Blue cone - Remove extra objects
    cv::erode(neutralizedImage.second, neutralizedImage.second,
              cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
    cv::dilate(neutralizedImage.second, neutralizedImage.second,
               cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7)));

    return neutralizedImage;
}
