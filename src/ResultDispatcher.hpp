//
// Scribbled on by Group 16, Class of 2023
//

#ifndef RESULTDISPATCHER
#define RESULTDISPATCHER

// Import the header-only middleware library, libcluon, to build high-performance microservices.
#include "cluon-complete-v0.0.127.hpp"

// Incorporate the GUI and image processing headers from OpenCV.
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// Named constants
const float ANGLE_THRESHOLD_LOW = 89.0;
const float ANGLE_THRESHOLD_HIGH = 91.0;
const float ANGLE_MULTIPLIER = 0.003333f;
const float GROUND_STEERING_OFFSET = 0.3f;

class ResultDispatcher {
    public:
        float computeGroundSteeringRequest(float angleOfRoad, float pedalPos);
        void logGroundSteeringRequest(float groundSteeringRequest, std::pair<bool, cluon::data::TimeStamp>);
};

#endif // RESULTDISPATCHER
