//
// Scribbled on by Group 16, Class of 2023
//

// Include the single-file, header-only middleware libcluon to create high-performance microservices
#include "cluon-complete.hpp"
// Include the OpenDLV Standard Message Set that contains messages that are usually exchanged for automotive or robotic applications 
#include "opendlv-standard-message-set.hpp"

#include "AngleCalculator.hpp"
#include "ColorCrafter.hpp"
#include "DataStream.hpp"
#include "NoiseNeutralizer.hpp"
#include "ShapeSniffer.hpp"
#include "ResultDispatcher.hpp"

// Include the GUI and image processing header files from OpenCV
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

std::string getTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto itt = std::chrono::system_clock::to_time_t(now);
    std::ostringstream ss;
    ss << std::put_time(gmtime(&itt), "%Y-%m-%dT%H:%M:%SZ");
    return ss.str();
}

void drawTxtOverlay(cv::Mat &img, const cluon::data::TimeStamp &sampleTimeStamp) {
    auto now = std::chrono::system_clock::now();
    auto itt = std::chrono::system_clock::to_time_t(now);
    std::ostringstream now_ss;
    now_ss << std::put_time(gmtime(&itt), "%Y-%m-%dT%H:%M:%SZ");
    std::string now_str = now_ss.str();

    auto sample_itt = cluon::time::toMicroseconds(sampleTimeStamp);

    std::ostringstream ss, ss2;
    ss << "Now: " << now_str << "; ts: " << sample_itt << ";";
    ss2 << "Tumenjargal, Altansukh";
    std::string text = ss.str();
    std::string text2 = ss2.str();

    int font = cv::FONT_HERSHEY_SIMPLEX;
    double fontSize = 0.7;
    int thickness = 1;
    cv::Point textOrg(10, 30);
    cv::Point textOrg2(10, 60);

    // Text drop shadow
    cv::putText(img, text, textOrg + cv::Point(1, 1), font, fontSize, cv::Scalar(0, 0, 0), thickness, 8);
    cv::putText(img, text2, textOrg2 + cv::Point(1, 1), font, fontSize, cv::Scalar(0, 0, 0), thickness, 8);

    // Draw the white text
    cv::putText(img, text, textOrg, font, fontSize, cv::Scalar(255, 255, 255), thickness, 8);
    cv::putText(img, text2, textOrg2, font, fontSize, cv::Scalar(255, 255, 255), thickness, 8);
}


int32_t main(int32_t argc, char **argv) {
    AngleCalculator angleCalculator;
    ColorCrafter colorCrafter;
    DataStream dataStream;
    NoiseNeutralizer noiseNeutralizer;
    ShapeSniffer shapeSniffer;
    ResultDispatcher steeringCalculator;

    int32_t retCode{1};
    // Parse the command line parameters as we require the user to specify some mandatory information on startup.
    auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
    if ( (0 == commandlineArguments.count("cid")) ||
         (0 == commandlineArguments.count("name")) ||
         (0 == commandlineArguments.count("width")) ||
         (0 == commandlineArguments.count("height")) ) {
        std::cerr << argv[0] << " attaches to a shared memory area containing an ARGB image." << std::endl;
        std::cerr << "Usage:   " << argv[0] << " --cid=<OD4 session> --name=<name of shared memory area> [--verbose]" << std::endl;
        std::cerr << "         --cid:    CID of the OD4Session to send and receive messages" << std::endl;
        std::cerr << "         --name:   name of the shared memory area to attach" << std::endl;
        std::cerr << "         --width:  width of the frame" << std::endl;
        std::cerr << "         --height: height of the frame" << std::endl;
        std::cerr << "Example: " << argv[0] << " --cid=253 --name=img --width=640 --height=480 --verbose" << std::endl;
    } else {
        // Extract the values from the command line parameters
        const std::string NAME{commandlineArguments["name"]};
        const uint32_t WIDTH{static_cast<uint32_t>(std::stoi(commandlineArguments["width"]))};
        const uint32_t HEIGHT{static_cast<uint32_t>(std::stoi(commandlineArguments["height"]))};
        const bool VERBOSE{commandlineArguments.count("verbose") != 0};
        uint32_t ITERATION;

        // check if the application will be run in iteration
        if (commandlineArguments.count("iteration") != 0) {
            ITERATION = static_cast<uint32_t>(std::stoi(commandlineArguments["iteration"]));
        } else {
            // If no iteration argument provided, then init with zero
            ITERATION = 0;
        }

        // Attach to the shared memory.
        std::unique_ptr<cluon::SharedMemory> sharedMemory{ new cluon::SharedMemory{NAME} };

        if (sharedMemory && sharedMemory->valid()) {
            std::clog << argv[0] << ": Attached to shared memory '" << sharedMemory->name() << " (" << sharedMemory->size() << " bytes)." << std::endl;

            // Interface to a running OpenDaVINCI session where network messages are exchanged.
            // The instance od4 allows you to send and receive messages.
            cluon::OD4Session od4{static_cast<uint16_t>(std::stoi(commandlineArguments["cid"]))};

            cluon::data::TimeStamp sampleTimeStamp;

            // Receive Pedal position message
            opendlv::proxy::PedalPositionRequest ppr;
            std::mutex pprMutex;
            auto onPedalPositionRequest = [&ppr, &pprMutex](cluon::data::Envelope &&env) {
                std::lock_guard<std::mutex> lck(pprMutex);
                ppr = cluon::extractMessage<opendlv::proxy::PedalPositionRequest>(
                        std::move(env));
            };

            od4.dataTrigger(opendlv::proxy::PedalPositionRequest::ID(), onPedalPositionRequest);

            // Endless loop; End the program by pressing Ctrl-C
            while (od4.isRunning()) {
                cv::Mat img;

                // Wait for a notification of a new frame.
                sharedMemory->wait();

                sampleTimeStamp = cluon::time::now();

                // Lock the shared memory.
                sharedMemory->lock();
                {
                    // Copy the pixels from the shared memory into our own data structure.
                    cv::Mat wrapped(HEIGHT, WIDTH, CV_8UC4, sharedMemory->data());
                    img = wrapped.clone();
                }
                {
                    // Read notification timestamp.
                    auto r = sharedMemory->getTimeStamp();
                    sampleTimeStamp = (r.first ? r.second : sampleTimeStamp);
                }

                // Get timeStamp from shared memory
                std::pair<bool, cluon::data::TimeStamp> ts = sharedMemory->getTimeStamp();
                int64_t ts2 = cluon::time::toMicroseconds(ts.second);
                std::string ts_string = std::to_string(ts2);

                // set timestamp string
                std::string timestamp_string = "Timestamp: " + ts_string;
                std::string ts_seconds = std::to_string(sampleTimeStamp.seconds());
                std::ostringstream oss;
                oss << std::setw(6) << std::setfill('0') << sampleTimeStamp.microseconds();
                std::string microsecs = oss.str();
                std::string sampleTimeStamp_string = " | SampleTimestamp: " + ts_seconds + "." + microsecs;
                timestamp_string += sampleTimeStamp_string;

                sharedMemory->unlock();

                // Display timestamp on top left
                cv::putText(img,                                   // target image
                            timestamp_string,                      // text
                            cv::Point(10, img.rows - 80), // top-left position
                            cv::FONT_HERSHEY_PLAIN, 1.0,
                            CV_RGB(255, 255, 255), // font color
                            2);

                float pedalPos;

                // Access the latest received pedal position and lock the mutex
                {
                    std::lock_guard<std::mutex> lck(pprMutex);
                    pedalPos = ppr.position();
                }

                cv::Mat original = img.clone();
                std::pair<cv::Mat, cv::Mat> processedImage =
                        colorCrafter.processImage(img);
                processedImage = noiseNeutralizer.processImage(processedImage);

                // set string
                std::string height = std::to_string(processedImage.first.size().height);
                std::string width = std::to_string(processedImage.first.size().width);
                std::string colorCrafter_string =
                        "Image has height: " + height + ". Width: " + width;

                // after colorCrafter
                cv::putText(img,                           // target image
                            colorCrafter_string,                    // text
                            cv::Point(4, img.rows - 60),   // top-left position
                            cv::FONT_HERSHEY_PLAIN, 1.0,
                            CV_RGB(255, 255, 255),                  // font color
                            2);

                // Finding the coordinates of the mass centre points
                // Putting these as yellow and blue vector into a pair called massCentreCoordinates
                std::pair<std::vector<cv::Point2f>, std::vector<cv::Point2f>>
                    massCentreCoordinates = shapeSniffer.findContours(processedImage, original);

                // set string
                std::string nr_of_yellow_cones =
                        std::to_string(massCentreCoordinates.first.size());
                std::string nr_of_blue_cones =
                        std::to_string(massCentreCoordinates.second.size());
                std::string contourFinder_string = "Found " + nr_of_yellow_cones +
                                                   " of yellow cones. Found " +
                                                   nr_of_blue_cones + " of blue cones.";

                // after contourFinder
                cv::putText(img,                         // target image
                            contourFinder_string,                 // text
                            cv::Point(6, img.rows - 40), // top-left position
                            cv::FONT_HERSHEY_PLAIN, 1.0,
                            CV_RGB(255, 255, 255), // font color
                            2);

                // Calculate the angle of the road, based on center of mass
                float angleOfRoad = angleCalculator.getAngleOfRoad(
                        massCentreCoordinates.first, massCentreCoordinates.second);

                // set string
                std::string angle_string = std::to_string(angleOfRoad);
                std::string angleCalculatorString =
                        "Angle for calculation is: " + angle_string;

                // after angleCalculator
                cv::putText(img,                           // target image
                            angleCalculatorString,                  // text
                            cv::Point(8, img.rows - 20),   // top-left position
                            cv::FONT_HERSHEY_PLAIN, 1.0,
                            CV_RGB(255, 255, 255),                  // font color
                            2);

                // Calculate the groundSteeringRequest
                float groundSteeringRequest =
                        steeringCalculator.computeGroundSteeringRequest(angleOfRoad, pedalPos);
                dataStream.openOutputStream(ITERATION);
                dataStream.writeVehicleData(ts.second);
                dataStream.writeGroundSteeringRequest(groundSteeringRequest);

                // Closing output data stream
                dataStream.closeOutputStream();

                // Send ground steering request to the Kiwi car
                steeringCalculator.sendGroundSteeringRequest(groundSteeringRequest, ts);

                // Display image on your screen.
                if (VERBOSE) {
                    cv::imshow(sharedMemory->name().c_str(), img);
                    cv::waitKey(1);
                }
            }
        } else {
            std::cout << ITERATION << std::endl;
            std::cout << "Could not attach to shared memory. Application will be "
                         "terminated."
                      << std::endl;
            retCode = 0;
        }
    }
    return retCode;
}
