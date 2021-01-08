
#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/core/hal/interface.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
// #include <dlib/gui_widgets.h>
#include <vector>
#include <Eigen/Dense>
#include "point_set_compare.hpp"

using namespace dlib;
using namespace std;

int main(int argc, char *argv[])
{
    cv::Mat image;
    image_window win;
    std::vector<dlib::rectangle> detections;
    frontal_face_detector detector = get_frontal_face_detector();
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " <cam number> <face_landmark_shape_predictor>" << endl;
        exit(-1);
    }
    cv::VideoCapture camera(atoi(argv[1]));
    shape_predictor sp;
    deserialize(argv[2]) >> sp;
    while (1)
    {
        camera >> image;
        cv_image<bgr_pixel> cimage(image);
        detections = detector(cimage);

        std::vector<full_object_detection> shapes;
        for (std::vector<dlib::rectangle>::iterator it = detections.begin(); it != detections.end(); it++)
        {
            shapes.push_back(sp(cimage, *it));
        }
        std::vector<Eigen::Matrix<double, -1, 2>> face_point_sets;
        for(std::vector<dlib::full_object_detection>::iterator it=shapes.begin(); it!=shapes.end(); it++){
            Eigen::Matrix<double, -1, 2> points;
            // printf("drawing %ld points\n", it->num_parts());
            points.resize(it->num_parts(), 2);
            for(int i=0; i<it->num_parts(); i++){
                cv::Point p;
                // printf("<%d>\n", i);
                p.x = it->part(i).begin()[0];
                p.y = it->part(i).begin()[1];
                points(i, 0) = (double)p.x;
                points(i, 1) = (double)p.y;
                cv::circle(image, p, 1, cv::Scalar(0 ,255, 0));
            }
            // printf("finish drawing\n");
            face_point_sets.push_back(points);
        }

        if(face_point_sets.size() >= 2){
            Eigen::Matrix3d result;
            printf("compute similarity\n");
            printf("err = %lf\n", compare_point_sets(face_point_sets[0], face_point_sets[1], result));
        }
        
        cv::imshow("Faces", image);
        cv::waitKey(2);
        // win.clear_overlay();
        // win.set_image(cimage);
        // win.add_overlay(render_face_detections(shapes));

    }
}