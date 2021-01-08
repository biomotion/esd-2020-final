#include <iostream>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/face.hpp>
#include <time.h>
#include "gui.hpp"

// #define FACE_MODEL EigenFaceRecognizer
#define FACE_MODEL FisherFaceRecognizer
#define IMG_SIZE 128

int main(int argc, char *argv[])
{
    Layout layout;
    cv::Mat frame, frame_gray;
    cv::CascadeClassifier face_cascade;
    std::vector<cv::Rect> face_detections;
    cv::Ptr<cv::face::FACE_MODEL> model = cv::face::FACE_MODEL::create(10);

    if (argc != 4)
    {
        std::cout << "Usage: " << argv[0] << " <cam_number> <face_cascade_file> <model_path>" << std::endl;
        exit(-1);
    }

    cv::VideoCapture camera(atoi(argv[1]));
    face_cascade.load(argv[2]);
    model->read(argv[3]);

#ifdef USE_LCD_FB
    camera.set(cv::CAP_PROP_FPS, 1);
#endif
    int downsample_rate = 4;
    clock_t t1, t2, t3, t4, t5, t6, t7, t8, t9;
    while (1)
    {
        std::vector<std::pair<int, double> > labels;

        t1 = clock();
        camera >> frame;
        t2 = clock();
        // cv::resize(frame, frame, cv::Size(frame.cols/downsample_rate, frame.rows/downsample_rate));
        layout.updateCamera(frame);
        t3 = clock();
        cv::cvtColor(frame, frame_gray, cv::COLOR_RGBA2GRAY);
        cv::resize(frame_gray, frame_gray, cv::Size(frame_gray.cols/downsample_rate, frame_gray.rows/downsample_rate));
        t4 = clock();
        cv::equalizeHist(frame_gray, frame_gray);
        t5 = clock();
        face_cascade.detectMultiScale(frame_gray, face_detections);
        // for(int i=0; i<face_detections.size(); i++){
        //     cv::Rect r = face_detections.at(i);
        //     printf("%d, %d, %d, %d\n", r.x, r.y, r.width, r.height);
        //     face_detections.at(i).x *= downsample_rate;
        //     face_detections.at(i).y *= downsample_rate;
        //     face_detections.at(i).width *= downsample_rate;
        //     face_detections.at(i).height *= downsample_rate;
        //     printf("%d, %d, %d, %d\n", r.x, r.y, r.width, r.height);

        // }
        t6 = clock();
        layout.updateFaces(face_detections);
        t7 = clock();
        for(std::vector<cv::Rect>::iterator face=face_detections.begin(); face!=face_detections.end(); face++){
            int label;
            double confidence;
            cv::Mat resized_face;
            cv::resize(frame_gray(*face), resized_face, cv::Size(IMG_SIZE, IMG_SIZE));
            model->predict(resized_face, label, confidence);
            
            std::cout << "label: " << label << ", confidence: " << confidence << std::endl;
            labels.push_back(std::pair<int, double>(label, confidence));
        }
        // printf("predicted\n");
        t8 = clock();
        layout.updateLabels(face_detections, labels);
        t9 = clock();

        printf("time read img: \t\t%lf\n", (double)(t2 - t1)/CLOCKS_PER_SEC);
        printf("time updateCamera: \t%lf\n", (double)(t3 - t2)/CLOCKS_PER_SEC);
        printf("time cvtColor: \t\t%lf\n", (double)(t4 - t3)/CLOCKS_PER_SEC);
        printf("time equalizeHist: \t%lf\n", (double)(t5 - t4)/CLOCKS_PER_SEC);
        printf("time detect: \t\t%lf\n", (double)(t6 - t5)/CLOCKS_PER_SEC);
        printf("time updateFace: \t%lf\n", (double)(t7 - t6)/CLOCKS_PER_SEC);
        printf("time predicting: \t%lf\n", (double)(t8 - t7)/CLOCKS_PER_SEC);
        printf("time updateLabels: \t%lf\n", (double)(t9 - t8)/CLOCKS_PER_SEC);
        printf("Total Time: \t\t%lf\n", (double)(t9 - t1)/CLOCKS_PER_SEC);
        // layout.display();
        cv::waitKey(10);
    }
}