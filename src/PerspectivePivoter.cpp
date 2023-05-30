//
// Scribbled on by Group 16, Class of 2023
//

#include "PerspectivePivoter.hpp"
#include <iostream>

PerspectivePivoter::PerspectivePivoter() {
    cv::Point2f sourceVertices[4];
    sourceVertices[0] = cv::Point(207, 285);
    sourceVertices[1] = cv::Point(364, 285);
    sourceVertices[2] = cv::Point(476, 350);
    sourceVertices[3] = cv::Point(89, 353);

    cv::Point2f dstVertices[4];

    // Close-up perspective view
    dstVertices[0] = cv::Point(125, 130);
    dstVertices[1] = cv::Point(390, 130);
    dstVertices[2] = cv::Point(390, 395);
    dstVertices[3] = cv::Point(125, 395);

    // Calculate perspective matrix transform
    this->perspectiveMatrix = getPerspectiveTransform(sourceVertices, dstVertices);
}

cv::Mat PerspectivePivoter::changeView(const cv::Mat &image) {
    // Check if image is empty
    if (image.empty()) {
        std::cerr << "Error: Image is empty. Skipping this image.\n";
        return image; // Return same image
    }

    // Destination image size
    cv::Mat destImg(480, 640, CV_8UC3);

    // Change perspective to birds-eye
    cv::warpPerspective(image, destImg, this->perspectiveMatrix, destImg.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT);

    return destImg;
}

std::vector<cv::Point2f> PerspectivePivoter::convertPoints(const std::vector<cv::Point2f> &coordinates) {
    // Check if vector is empty
    if (coordinates.empty()) {
        std::cerr << "Error: Vector is empty. Skipping this image.\n";
        return coordinates; // Return empty vector
    }

    std::vector<cv::Point2f> dstPoints;

    // Change perspective to birds-eye
    cv::perspectiveTransform(coordinates, dstPoints, this->perspectiveMatrix);
    return dstPoints;
}
