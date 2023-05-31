//
// Created by Altan on 5/28/2023.
//

#define X_POSITION 320 // Car position in the middle of 640 width
#define Y_POSITION 480 // Car position in the bottom of 480 height

#include "AngleCalculator.hpp"
constexpr double PI = 3.14159265358979323846;

float AngleCalculator::distanceBetweenPoints(float xInPoint1, float yInPoint1, float xInPoint2, float yInPoint2) {
    float distance = std::sqrt(static_cast<float>(std::pow(xInPoint2 - xInPoint1, 2) +
                                                  std::pow(yInPoint2 - yInPoint1, 2)) * 1.0f);

    return distance;
}

float AngleCalculator::getAngleBetweenCones(std::vector<cv::Point2f> points) {
    // Check if there are at least two points
    if(points.size() < 2) {
        // Return some error value or handle this case appropriately
        throw std::invalid_argument("Not enough points to calculate angle");
    }
    // Detecting the two mass centres closest to the car by sorting the vector by its y values
    // Reference: https://stackoverflow.com/questions/18253065/is-there-any-function-to-sort-a-stdvector-cvpoint2f-vector
    std::sort(points.begin(), points.end(),
              [](const cv::Point2f &a, const cv::Point2f &b) {
                  return a.y > b.y;
              });

    float distance = distanceBetweenPoints(
            X_POSITION, Y_POSITION, points[0].x, points[0].y);

    if (distance < 200) {
        // Getting angles in radians
        float radians =
                std::atan2(points[0].y - points[1].y, points[0].x - points[1].x);

        // Convert radians to degreesBetween, while casting to float
        float degreesBetween = radians * 180.0f / static_cast<float>(PI);
        float adjustedAngles = 180 - degreesBetween;

        if (adjustedAngles > 60 && adjustedAngles < 120) {
            return 90;
        } else {
            return adjustedAngles;
        }
    }
    return 90;
}

float AngleCalculator::getAngleOfRoad(std::vector<cv::Point2f> yellow, std::vector<cv::Point2f> blue) {
    float yellowAngle;
    float blueAngle;

    // Checking if there are more than 2 blue cones and 2 yellow cones
    if ((yellow.size() >= 2) && (blue.size() >= 2)) {
        yellowAngle = getAngleBetweenCones(yellow);
        blueAngle = getAngleBetweenCones(blue);
        // Taking the mean value of the angles
        float meanAngle = (yellowAngle + blueAngle) / 2;
        return meanAngle;
    } else if (yellow.size() >= 2) {
        yellowAngle = getAngleBetweenCones(yellow);
        return yellowAngle;
    } else if (blue.size() >= 2) {
        blueAngle = getAngleBetweenCones(blue);
        return blueAngle;
    } else {
        // Return 90 if there is only one cone
        return 90;
    }
}
