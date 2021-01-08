
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/face.hpp>

#include <iostream>
#include <vector>
#include <dirent.h>

// #define FACE_MODEL EigenFaceRecognizer
#define FACE_MODEL FisherFaceRecognizer

void get_files(const char *dir, std::vector<const char *> &files);
void train_model(const char* path,const std::vector<const char*>& files, cv::Ptr<cv::face::FACE_MODEL>& model);


int main(int argc, char* argv[]){
    if(argc != 3){
        printf("usage: %s <training_data_path> <output_model_path>\n", argv[0]);
        exit(-1);
    }
    cv::Ptr<cv::face::FACE_MODEL> model = cv::face::FACE_MODEL::create(10);
    std::vector<const char *> files;
    get_files(argv[1], files);
    train_model(argv[1], files, model);
    model->save(argv[2]);
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
    if((strcmp(dirp->d_name, ".") == 0) || (strcmp(dirp->d_name, "..") == 0)){
      continue;
    }
    files.push_back(dirp->d_name);
    std::cout << "found file: " << dirp->d_name << std::endl;
  }
}

void train_model(const char* path,const std::vector<const char*>& files, cv::Ptr<cv::face::FACE_MODEL>& model){
    std::vector<cv::Mat> images;
    std::vector<int> labels;
    std::cout << "training" << std::endl;
    char file_str[100];
    for(std::vector<const char*>::const_iterator it=files.begin(); it!=files.end(); it++){
        int label = 0;
        char *token;
        cv::Mat img;
        sprintf(file_str, "%s/%s", path, *it);
        img = cv::imread(file_str);
        cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
        std::cout << "size = " << img.cols << "x" << img.rows << "x" << img.channels();
        images.push_back(img);

        sprintf(file_str, "%s", *it);
        token = strtok(file_str, "_");
        label = atoi(token);
        std::cout << ", label = " << label << std::endl;
        labels.push_back(label);
    }
    model->train(images, labels);
    std::cout << "shape of mean: " << model->getMean().size() << std::endl;
    std::cout << "shape of eigenValues: " << model->getEigenValues().size() << std::endl;
    std::cout << "shape of eigenvectors: " << model->getEigenVectors().size() << std::endl;
    int predicted = 0;
    predicted = model->predict(images[0]);
    std::cout << "predict: " << predicted  << " gt: " << labels[0]<< std::endl;
}
