#ifndef __GUI_HPP__
#define __GUI_HPP__

#define USE_LCD_FB

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <iostream>
#ifdef USE_LCD_FB
#include "display_utils.h"
#endif

#define FRAME_SIZE_X 800
#define FRAME_SIZE_Y 480
#define IMAGE_SIZE_X 640
#define IMAGE_SIZE_Y 480
#define FACE_SIZE 160

const char WINDOW_NAME[] = "result";
class Layout
{
#ifdef USE_LCD_FB
  framebuffer_info fb_info;
  std::ofstream fb;
#endif
  cv::Mat frame;
  cv::Mat camera_frame;
  cv::Rect camera_roi;

public:
  Layout() : frame(FRAME_SIZE_Y, FRAME_SIZE_X, 16)
  {
#ifdef USE_LCD_FB
    fb_info = get_framebuffer_info("/dev/fb0");
    fb.open("/dev/fb0");
#endif

    camera_roi.x = 0;
    camera_roi.y = 0;
    camera_roi.height = IMAGE_SIZE_Y;
    camera_roi.width = IMAGE_SIZE_X;
  }
  void updateCamera(cv::InputArray in_frame)
  {
#ifdef USE_LCD_FB
    draw_on_screen(in_frame, fb, fb_info, camera_roi);
#endif
    // cv::resize(in_frame, frame(camera_roi), cv::Size2i(camera_roi.width, camera_roi.height));
    in_frame.copyTo(camera_frame);
  }
  void updateFaces(std::vector<cv::Rect> detections)
  {
    int offset = 0;
    cv::Rect target_roi;
    for (std::vector<cv::Rect>::iterator face=detections.begin(); face!=detections.end(); face++)
    {
      target_roi.x = IMAGE_SIZE_X;
      target_roi.y = offset;
      target_roi.height = FACE_SIZE;
      target_roi.width = FACE_SIZE;
      // std::cout << "display roi: " << target_roi << std::endl;
      // std::cout << "face roi: " << *face << std::endl;
#ifdef USE_LCD_FB
    draw_on_screen(camera_frame(*face), fb, fb_info, target_roi);
#endif
      // cv::resize(camera_frame(face), frame(target_roi), cv::Size2i(FACE_SIZE, FACE_SIZE));
      offset += FACE_SIZE;
    }

    // cv::rectangle(frame, cv::Rect(offset, IMAGE_SIZE_Y, FRAME_SIZE_X - offset - FACE_SIZE, FACE_SIZE), cv::Scalar(0, 0, 0));
  }
  void updateMessages(const char* message)
  {
    // char buf[40];
    // cv::rectangle(frame, cv::Rect(IMAGE_SIZE_X, 0, FRAME_SIZE_X - IMAGE_SIZE_X, IMAGE_SIZE_Y), cv::Scalar(0, 0, 0));
    // sprintf(buf, "# faces: %ld", detections.size());
    // cv::putText(frame, buf, cv::Point(IMAGE_SIZE_X + 5, 20), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255, 255, 0));
  }
  // void updateFaceFeature(const std::vector<cv::Point2d> &points)
  // {
  //   frame(face_roi).copyTo(frame(face_feat_roi));
  //   for (std::vector<cv::Point2d>::const_iterator it = points.begin(); it != points.end(); it++)
  //   {
  //     cv::circle(frame(face_feat_roi), *it, 1, cv::Scalar(0, 255, 0));
  //   }
  // }
  // void display()
  // {
  //   cv::imshow(WINDOW_NAME, frame);
  //   cv::waitKey(1);
  // }
};

#endif