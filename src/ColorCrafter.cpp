//
// Scribbled on by Group 16, Class of 2023
//

#include "ColorCrafter.hpp"

std::pair<cv::Mat, cv::Mat> ColorCrafter::processColors(cv::Mat image) {

    int iLowHYellow = 20;
    int iHighHYellow = 45;
    int iLowSYellow = 100;
    int iHighSYellow = 235;
    int iLowVYellow = 124;
    int iHighVYellow = 255;

    int iLowHBlue = 109;
    int iHighHBlue = 135;
    int iLowSBlue = 68;
    int iHighSBlue = 255;
    int iLowVBlue = 52;
    int iHighVBlue = 255;

    int iReflectionLowHBlue = 0;
    int iReflectionHighHBlue = 179;
    int iReflectionLowSBlue = 50;
    int iReflectionHighSBlue = 80;
    int iReflectionLowVBlue = 0;
    int iReflectionHighVBlue = 255;

    cv::Mat yellowImgInRange;
    cv::Mat blueImgInRange;
    cv::Mat yellowImg;
    cv::Mat blueImg;
    cv::Mat blueReflectionImg;
    cv::Mat blueImgInRangeReflection;
    cv::Mat blueImgAnd;
    cv::Mat finalBlueImg;

    // Divide image height in half as cones are in bottom half only
    cv::Mat croppedImage =
            image(cv::Rect(0, image.rows / 2, image.cols, image.rows / 2));

    cv::cvtColor(croppedImage, yellowImg, CV_BGR2HSV);
    blueImg = yellowImg.clone();
    blueReflectionImg = yellowImg.clone();

    // Process colors to detect yellow cones
    cv::inRange(yellowImg, cv::Scalar(iLowHYellow, iLowSYellow, iLowVYellow),
                cv::Scalar(iHighHYellow, iHighSYellow, iHighVYellow),
                yellowImgInRange);

    // Process colors to detect blue cones
    cv::inRange(blueImg, cv::Scalar(iLowHBlue, iLowSBlue, iLowVBlue),
                cv::Scalar(iHighHBlue, iHighSBlue, iHighVBlue), blueImgInRange);

    // Process colors to detect reflection of cones
    cv::inRange(
            blueReflectionImg,
            cv::Scalar(iReflectionLowHBlue, iReflectionLowSBlue, iReflectionLowVBlue),
            cv::Scalar(iReflectionHighHBlue, iReflectionHighSBlue,
                       iReflectionHighVBlue),
            blueImgInRangeReflection);

    // Bitwise AND operator to find reflections of blue cones
    cv::bitwise_and(blueImgInRange, blueImgInRangeReflection, blueImgAnd);

    // Bitwise XOR operator to remove found reflections of blue cones
    cv::bitwise_xor(blueImgAnd, blueImgInRange, finalBlueImg);

    std::pair<cv::Mat, cv::Mat> pairs = std::make_pair(yellowImgInRange, finalBlueImg);

    return pairs;
}
