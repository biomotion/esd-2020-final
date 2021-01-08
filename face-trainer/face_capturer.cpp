#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/face.hpp>

#include <iostream>
#include <vector>
#include <dirent.h>

#define IMG_SIZE 128

int get_last_label(const char *path, const char *prefix);

int main(int argc, char *argv[])
{
  int i;
  char file_str[100];
  cv::Mat image, image_gray;
  cv::CascadeClassifier face_cascade;
  std::vector<cv::Rect> detections;
  // cv::
  if (argc != 5)
  {
    printf("usage: %s <camera_num> <face_cascade_file> <image_save_path> <image_prefix>\n", argv[0]);
    exit(-1);
  }

  cv::VideoCapture camera(atoi(argv[1]));
  face_cascade.load(argv[2]);
  i = get_last_label(argv[3], argv[4]) + 1;
  std::cout << "New image will start from " << i << std::endl;
  std::cout << "Press C to capture a face" << std::endl;
  std::cout << "Press ESC to stop capturing" << std::endl;
  char key = 0;
  while (key != 27)
  {
    camera >> image;
    cv::cvtColor(image, image_gray, cv::COLOR_RGBA2GRAY);
    cv::equalizeHist(image_gray, image_gray);
    face_cascade.detectMultiScale(image_gray, detections);
    if (detections.size() > 0)
    {
      cv::Mat resized;
      cv::resize(image(detections[0]), resized, cv::Size(IMG_SIZE, IMG_SIZE));
      cv::imshow("FACES", resized);
      cv::resize(image_gray(detections[0]), resized, cv::Size(IMG_SIZE, IMG_SIZE));
      key = cv::waitKey(10);
      // std::cout << key << std::endl;
      if (key == 'c')
      {
        std::cout << "save " << i << std::endl;
        sprintf(file_str, "%s/%s_%d.jpg", argv[3], argv[4], i++);
        cv::imwrite(file_str, resized);
      }
    }
  }
}

int get_last_label(const char *path, const char *prefix)
{
  int max = -1;
  char s[10], *tok;
  DIR *dp;
  struct dirent *dirp;

  sprintf(s, "%s_", prefix);

  if ((dp = opendir(path)) == NULL)
  {
    std::cerr << "Can't open directory" << std::endl;
  }
  while ((dirp = readdir(dp)) != NULL)
  {
    if ((strcmp(dirp->d_name, ".") == 0) || (strcmp(dirp->d_name, "..") == 0))
    {
      continue;
    }
    // std::cout << "found file: " << dirp->d_name << std::endl;
    tok = strstr(dirp->d_name, s);
    if(tok == NULL) {
      continue;
    }
    tok += strlen(s);
    max = (atoi(tok) > max) ? atoi(tok) : max;
  }
  return max;
}