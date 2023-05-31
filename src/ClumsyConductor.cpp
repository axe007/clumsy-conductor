//
// Scribbled on by Group 16, Class of 2023
//

// Import the header-only middleware library, libcluon, to build high-performance microservices.
#include "cluon-complete.hpp"
// Import the OpenDLV Standard Message Set which includes typical messages for automotive or robotic applications.
#include "opendlv-standard-message-set.hpp"

#include "AngleCalculator.hpp"
#include "ColorCrafter.hpp"
#include "DataStream.hpp"
#include "NoiseNeutralizer.hpp"
#include "ResultDispatcher.hpp"
#include "ShapeSniffer.hpp"

// Incorporate the GUI and image processing headers from OpenCV.
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

/*std::string getTimestamp() {
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
}*/

int32_t main(int32_t argc, char **argv) {
    AngleCalculator angleCalculator;
    ColorCrafter colorCrafter;
    DataStream dataStream;
    NoiseNeutralizer noiseNeutralizer;
    ShapeSniffer shapeSniffer;
    ResultDispatcher resultDispatcher;

    int32_t returnCode{1};
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
        std::cerr << "         Optional argument:" << std::endl;
        std::cerr << "         --iteration: Integer number for iteration file naming" << std::endl;
        std::cerr << "         -v ~/data:/usr/bin/data: Path to save data folder. /HOME/data needs to exist to save CSV files." << std::endl;
        std::cerr << "Example: " << argv[0] << " --cid=253 --name=img --width=640 --height=480 --verbose" << std::endl;
    } else {
        // Extract the values from the command line parameters
        const std::string NAME{commandlineArguments["name"]};
        const uint32_t WIDTH{static_cast<uint32_t>(std::stoi(commandlineArguments["width"]))};
        const uint32_t HEIGHT{static_cast<uint32_t>(std::stoi(commandlineArguments["height"]))};
        const bool VERBOSE{commandlineArguments.count("verbose") != 0};
        uint32_t iterationId;

        // check if the application will be run in iteration
        if (commandlineArguments.count("iteration") != 0) {
            iterationId = static_cast<uint32_t>(std::stoi(commandlineArguments["iteration"]));
        } else {
            // If no iteration argument provided, then init with zero
            iterationId = 0;
        }

        // Attach to the shared memory.
        std::unique_ptr<cluon::SharedMemory> sharedMemory{ new cluon::SharedMemory{NAME} };

        if (sharedMemory && sharedMemory->valid()) {
            std::clog << argv[0] << ": Attached to shared memory '" << sharedMemory->name() << " (" << sharedMemory->size() << " bytes)." << std::endl;

            // Interface to OpenDaVinci session id
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

            // Endless loop. End the program by pressing Ctrl-C
            while (od4.isRunning()) {
                cv::Mat img;
                float pedalPos;

                // Wait for a notification of a new frame
                sharedMemory->wait();

                // Lock the shared memory
                sharedMemory->lock();
                {
                    // Clone the img data from the shared memory
                    cv::Mat wrapped(HEIGHT, WIDTH, CV_8UC4, sharedMemory->data());
                    img = wrapped.clone();
                }

                // Read timestamp from img in shared memory
                std::pair<bool, cluon::data::TimeStamp> ts = sharedMemory->getTimeStamp();
                int64_t ts2 = cluon::time::toMicroseconds(ts.second);
                std::string ts_string = std::to_string(ts2);

                // set timestamp string
                std::string timestampString = "SampleTimestamp: " + ts_string;
                std::string ts_seconds = std::to_string(sampleTimeStamp.seconds());

                sharedMemory->unlock();

                // Access the latest received pedal position and lock the mutex
                {
                    std::lock_guard<std::mutex> lck(pprMutex);
                    pedalPos = ppr.position();
                }

                // Display timestamp on top left
                cv::putText(img,timestampString,
                            cv::Point(10, img.rows - 60),
                            cv::FONT_HERSHEY_PLAIN, 1.0,
                            CV_RGB(255, 255, 255), 2);

                cv::Mat original = img.clone();
                std::pair<cv::Mat, cv::Mat> processedImage =
                        colorCrafter.processColors(img);
                processedImage = noiseNeutralizer.reduceNoise(processedImage);

                // Finding the coordinates of the mass centre points
                // Putting these as yellow and blue vector into a pair called massCentreCoordinates
                std::pair<std::vector<cv::Point2f>, std::vector<cv::Point2f>>
                        massCentreCoordinates = shapeSniffer.findContours(processedImage, original);

                // set string
                std::string colorCrafterString = "Detected cones: Yellow - " + std::to_string(massCentreCoordinates.first.size()) +
                                                 ". Blue - " + std::to_string(massCentreCoordinates.second.size());

                // after colorCrafter
                cv::putText(img,
                            colorCrafterString,
                            cv::Point(10, img.rows - 40),
                            cv::FONT_HERSHEY_PLAIN,1.0,
                            CV_RGB(255, 255, 255),2);

                // Calculate the angle of the road, based on center of mass
                float angleOfRoad = angleCalculator.getAngleOfRoad(massCentreCoordinates.first, massCentreCoordinates.second);

                // Calculate the groundSteeringRequest
                float groundSteeringRequest =
                        resultDispatcher.computeGroundSteeringRequest(angleOfRoad, pedalPos);

                // debug string after groundSteeringRequest calculation
                cv::putText(img,
                            "Calculated groundSteeringRequest: " + std::to_string(groundSteeringRequest),
                            cv::Point(10, img.rows - 20),
                            cv::FONT_HERSHEY_PLAIN,1.0,
                            CV_RGB(255, 255, 255),2);

                // Output result to the console
                resultDispatcher.logGroundSteeringRequest(groundSteeringRequest, ts);

                dataStream.openOutputStream(iterationId);
                dataStream.writeVehicleData(ts.second);
                dataStream.writeGroundSteeringRequest(groundSteeringRequest);

                // Closing output data stream
                dataStream.closeOutputStream();

                // Display image on your screen.
                if (VERBOSE) {
                    cv::imshow(sharedMemory->name().c_str(), img);
                    cv::waitKey(1);
                }
            }
        } else {
            std::cout << iterationId << std::endl;
            std::cout << "Could not attach to the shared memory. Application will terminate now." << std::endl;
            returnCode = 0;
        }
    }
    return returnCode;
}
