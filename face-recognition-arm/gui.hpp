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

class Layout
{
#ifdef USE_LCD_FB
  framebuffer_info fb_info;
  std::ofstream fb;
#else
  cv::Mat display_frame;
  const char *WINDOW_NAME = "Debugging Window";
#endif
  cv::Mat camera_frame;
  cv::Rect camera_roi, message_roi;

public:
  Layout()
  {
#ifdef USE_LCD_FB
    fb_info = get_framebuffer_info("/dev/fb0");
    fb.open("/dev/fb0");

    draw_on_screen(cv::Mat::zeros(FRAME_SIZE_Y, FRAME_SIZE_X, CV_8UC3), fb, fb_info, cv::Rect(0, 0, FRAME_SIZE_X, FRAME_SIZE_Y));
#else
    display_frame = cv::Mat::zeros(FRAME_SIZE_Y, FRAME_SIZE_X, CV_8UC3);

#endif
    camera_roi.x = 0;
    camera_roi.y = 0;
    camera_roi.width = IMAGE_SIZE_X;
    camera_roi.height = IMAGE_SIZE_Y;

    message_roi.x = IMAGE_SIZE_X;
    message_roi.y = FRAME_SIZE_Y - FACE_SIZE;
    message_roi.width = FACE_SIZE;
    message_roi.height = FACE_SIZE;
  }

  void updateCamera(cv::InputArray in_frame)
  {
#ifdef USE_LCD_FB
    in_frame.copyTo(camera_frame);
    // draw_on_screen(in_frame, fb, fb_info, camera_roi);
#else
    // std::cout << "resize" << std::endl;
    cv::resize(in_frame, camera_frame, cv::Size2i(camera_roi.width, camera_roi.height));
    // std::cout << "copy" << std::endl;
    camera_frame.copyTo(display_frame(camera_roi));
#endif
  }

  void updateFaces(std::vector<cv::Rect> detections)
  {
    // TODO: draw on camera frame
    int offset = 0;
    cv::Rect target_roi;
    for (std::vector<cv::Rect>::iterator face = detections.begin(); face != detections.end() && offset < FRAME_SIZE_Y; face++)
    {
      cv::Rect r = *face;
      r.x *= 4;
      r.y *= 4;
      r.width *= 4;
      r.height *= 4;
      target_roi.x = IMAGE_SIZE_X;
      target_roi.y = offset;
      target_roi.height = FACE_SIZE;
      target_roi.width = FACE_SIZE;
#ifdef USE_LCD_FB
      draw_on_screen(camera_frame(r), fb, fb_info, target_roi);
#else
      cv::Mat tmp;
      cv::resize(camera_frame(*face), tmp, cv::Size2i(target_roi.width, target_roi.height));
      tmp.copyTo(display_frame(target_roi));
#endif
      offset += FACE_SIZE;

    }
  }
  void updateLabels(std::vector<cv::Rect>& detections, std::vector<std::pair<int, double> >& labels)
  {
    char msg[40];
    for (int i=0; i<detections.size(); i++)
    {
      if(labels[i].first >= 1){
        sprintf(msg, "id=%d(%.2lf)", labels[i].first, labels[i].second);
      }else{
        sprintf(msg, "id=unknown(%.2lf)", labels[i].second);
      }

#ifdef USE_LCD_FB
      cv::Rect r = detections[i];
      r.x *= 4;
      r.y *= 4;
      r.width *= 4;
      r.height *= 4;
      cv::rectangle(camera_frame, r, cv::Scalar(0, 255, 0));
      cv::putText(camera_frame, msg, cv::Point(r.x, r.y - 5), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0));
#else
      cv::rectangle(display_frame(camera_roi), detections[i], cv::Scalar(0, 255, 255));
      cv::putText(display_frame(camera_roi), msg, cv::Point(detections[i].x, detections[i].y - 5), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255,255));
#endif

    }
    draw_on_screen(camera_frame, fb, fb_info, camera_roi);
  }

  void updateMessages(const char *message)
  {
    cv::Mat message_frame = cv::Mat::zeros(FACE_SIZE, FACE_SIZE, CV_8UC3);
    cv::putText(message_frame, message, cv::Point(IMAGE_SIZE_X + 5, 20), cv::FONT_HERSHEY_PLAIN, 2., cv::Scalar(255, 255, 0));
#ifdef USE_LCD_FB
    draw_on_screen(message_frame, fb, fb_info, message_roi);
#else
    cv::resize(message_frame, display_frame(message_roi), cv::Size2i(FACE_SIZE, FACE_SIZE));
#endif
  }

#ifndef USE_LCD_FB
  void display()
  {
    cv::imshow(WINDOW_NAME, display_frame);
    cv::waitKey(10);
  }
#endif
};

#endif