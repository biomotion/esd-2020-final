#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "features.hpp"

int main(int argc, char* argv[]){
    cv::VideoCapture camera(0);
    cv::Mat frame;
    struct HogOptions opt;

    cv::Rect roi;
    roi.x = 80;
    roi.y = 0;
    roi.height = 480;
    roi.width = 480;
    while(1){
        std::vector<float> descriptor;
        camera >> frame;
        get_hog(frame(roi), descriptor, opt);
    }
}