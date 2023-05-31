//
// Scribbled on by Group 16, Class of 2023
//

#include <fstream>
#include <iostream>
#include <csignal>

#include "DataStream.hpp"

std::ofstream dataFile;
int firstRun = 0;

void signalHandler(int signum) {
    std::cout << "Interrupt signal (" << signum << ") received.\n";

    // Cleanup and close up stuff here, terminate file writing.
    if (dataFile.is_open()) {
        dataFile.close();
        std::cout << "File closed successfully.\n";
    }

    // Terminate program  
    exit(signum);  
}

void DataStream::openOutputStream(uint32_t iteration) {
    // get iteration file
    std::string iteration_string = std::to_string(iteration);
    std::string filename = "data/group16_" + iteration_string + ".csv";

    // Register signal and handler  
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    if (firstRun == 0) {
        dataFile.open(filename, std::ios::trunc); /* Flaw finder: ignore */
        dataFile << "TimeStamp, GroundSteeringRequest, \n";
    } else {
        dataFile.open(filename, std::ios::app); /* Flaw finder: ignore */
    }

    firstRun++;
}

void DataStream::closeOutputStream() { dataFile.close(); }

void DataStream::writeVehicleData(cluon::data::TimeStamp ts) {
    int64_t ts2 = cluon::time::toMicroseconds(ts);
    std::string timeStamp = std::to_string(ts2);
    dataFile << timeStamp + ",";
}

void DataStream::writeGroundSteeringRequest(float groundSteeringRequest) {
    std::string computedGroundSteeringRequest = std::to_string(groundSteeringRequest);
    dataFile << computedGroundSteeringRequest + ", \n";
}
