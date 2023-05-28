#ifndef DATASTREAM
#define DATASTREAM

// Include the single-file, header-only middleware libcluon to create
// high-performance microservices
#include "cluon-complete-v0.0.127.hpp"

class DataStream {
    public:
        void openOutputStream(uint32_t iteration);
        void closeOutputStream();
        void writeVehicleData(cluon::data::TimeStamp ts);
        void writeGroundSteeringRequest(float groundSteeringRequest);
};

#endif