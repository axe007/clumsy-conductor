//
// Scribbled on by Group 16, Class of 2023
//

#ifndef COLORCRAFTER
#define COLORCRAFTER

#include <utility>
#include <opencv2/core/core.hpp>

class ColorCrafter {
    public: 
        std::pair<cv::Mat, cv::Mat> processImage(cv::Mat image);

};

#endif //COLORCRAFTER
