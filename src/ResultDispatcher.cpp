//
// Scribbled on by Group 16, Class of 2023
//

#include "ResultDispatcher.hpp"
#include "opendlv-standard-message-set.hpp"

// Calculating the ground steering request
float ResultDispatcher::computeGroundSteeringRequest (float angleOfRoad, float pedalPos) {
    float groundSteeringRequest = 0;
    if (((angleOfRoad <= ANGLE_THRESHOLD_LOW) || (angleOfRoad >= ANGLE_THRESHOLD_HIGH)) && pedalPos != -0) {
        groundSteeringRequest = (angleOfRoad * ANGLE_MULTIPLIER) - GROUND_STEERING_OFFSET;
    }

    return groundSteeringRequest;
}

void ResultDispatcher::logGroundSteeringRequest (
        float groundSteeringRequest,
        std::pair<bool, cluon::data::TimeStamp> ts_pair) {

    // Log group name, timestamp and calculated ground steering request
    std::string gsrString = std::to_string(groundSteeringRequest);
    int64_t ts2 = cluon::time::toMicroseconds(ts_pair.second);
    std::string timestampString = std::to_string(ts2);

    std::cout << "group_16;" << timestampString << ";" << gsrString << ";"
              << std::endl;
}
