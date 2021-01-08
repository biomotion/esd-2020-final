#include "gui.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
using namespace cv;

#include <time.h>

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    Layout layout;
    cv::Mat frame, frame_gray;
    CascadeClassifier face_cascade;
    std::vector<cv::Rect> face_detections;
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " <cam number> <face_cascade_file>" << endl;
        exit(-1);
    }

    cv::VideoCapture camera(atoi(argv[1]));
    face_cascade.load(argv[2]);

    // camera.set(cv::CAP_PROP_FRAME_WIDTH, IMAGE_SIZE_X);
    // camera.set(cv::CAP_PROP_FRAME_HEIGHT, IMAGE_SIZE_Y);

    // std::vector<cv::Point2d> points;
    // points.push_back(cv::Point2d(80, 80));
    // points.push_back(cv::Point2d(80, 95));
    // points.push_back(cv::Point2d(80, 90));
    // points.push_back(cv::Point2d(60, 70));
    // points.push_back(cv::Point2d(100, 70));
    // points.push_back(cv::Point2d(80, 90));
    clock_t t1, t2, t3, t4;
    clock_t t5, t6, t7, t8;
    while (1)
    {
        t1 = clock();
        camera >> frame;
        t2 = clock();
        // cout << frame.size() << endl;
        // printf("drawing camera\n");
        t3 = clock();
        layout.updateCamera(frame);
        t4 = clock();
        cv::cvtColor(frame, frame_gray, cv::COLOR_RGBA2GRAY);
        t5 = clock();
        cv::equalizeHist(frame_gray, frame_gray);
        t6 = clock();
        face_cascade.detectMultiScale(frame_gray, face_detections);
        t7 = clock();
        layout.updateFaces(face_detections);
        t8 = clock();
        printf("time read img: \t\t%lf\n", (double)(t2 - t1)/CLOCKS_PER_SEC);
        printf("time updateCamera: \t%lf\n", (double)(t4 - t3)/CLOCKS_PER_SEC);
        printf("time cvtColor: \t\t%lf\n", (double)(t5 - t4)/CLOCKS_PER_SEC);
        printf("time equalizeHist: \t%lf\n", (double)(t6 - t5)/CLOCKS_PER_SEC);\
        printf("time detect: \t\t%lf\n", (double)(t7 - t6)/CLOCKS_PER_SEC);
        printf("time updateFace: \t%lf\n", (double)(t8 - t7)/CLOCKS_PER_SEC);
        // layout.updateMessages(face_detections);
        // layout.display();
    }
}