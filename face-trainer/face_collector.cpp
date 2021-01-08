#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/face.hpp>

#include <iostream>
#include <vector>
#include <dirent.h>

#define IMG_SIZE 128

void get_files(const char *dir, std::vector<const char *> &files);
int get_last_label(const char *path, const char *prefix);

int main(int argc, char *argv[])
{
  int i;
  char file_str[100];
  cv::Mat image, image_gray;
  cv::CascadeClassifier face_cascade;
  std::vector<cv::Rect> detections;
  std::vector<const char *> files;
  // cv::
  if (argc != 5)
  {
    printf("usage: %s <input_path> <face_cascade_file> <image_save_path> <image_prefix>\n", argv[0]);
    exit(-1);
  }

  get_files(argv[1], files);
  face_cascade.load(argv[2]);
  i = get_last_label(argv[3], argv[4]) + 1;
  std::cout << "New image will start from " << i << std::endl;
  char key = 0;
  for (std::vector<const char *>::iterator f = files.begin(); f != files.end(); f++)
  {
    sprintf(file_str, "%s/%s", argv[1], *f);
    image = cv::imread(file_str);

    std::cout << file_str << std::endl;
    std::cout << image.size() << std::endl;
    cv::cvtColor(image, image_gray, cv::COLOR_RGB2GRAY);
    cv::equalizeHist(image_gray, image_gray);
    face_cascade.detectMultiScale(image_gray, detections);
    if (detections.size() > 0)
    {
      cv::Mat resized;
      cv::resize(image(detections[0]), resized, cv::Size(IMG_SIZE, IMG_SIZE));
      cv::imshow("FACES", resized);
      cv::resize(image_gray(detections[0]), resized, cv::Size(IMG_SIZE, IMG_SIZE));
      key = cv::waitKey(10);
      std::cout << "save " << i++ << std::endl;
      sprintf(file_str, "%s/%s_%d.jpg", argv[3], argv[4], i++);
      cv::imwrite(file_str, resized);

    }
  }
}

void get_files(const char *dir, std::vector<const char *> &files)
{
  DIR *dp;
  struct dirent *dirp;
  if ((dp = opendir(dir)) == NULL)
  {
    // std::cerr << "Can't open directory" << std::endl;
  }
  while ((dirp = readdir(dp)) != NULL)
  {
    if ((strcmp(dirp->d_name, ".") == 0) || (strcmp(dirp->d_name, "..") == 0))
    {
      continue;
    }
    files.push_back(dirp->d_name);
    std::cout << "found file: " << dirp->d_name << std::endl;
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
    if (tok == NULL)
    {
      continue;
    }
    tok += strlen(s);
    max = (atoi(tok) > max) ? atoi(tok) : max;
  }
  return max;
}