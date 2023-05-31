//
// Scribbled on by Group 16, Class of 2023
//

#ifndef DATASTREAM
#define DATASTREAM

// Import the header-only middleware library, libcluon, to build high-performance microservices.
#include "cluon-complete-v0.0.127.hpp"

class DataStream {
    public:
        void openOutputStream(uint32_t iteration);
        void closeOutputStream();
        void writeVehicleData(cluon::data::TimeStamp ts);
        void writeGroundSteeringRequest(float groundSteeringRequest);
};

#endif