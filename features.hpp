// Implement hog descriptor extration using opencv
// Implement svm traning and detection using opencv
// Visualize hog descriptor
#include <opencv2/objdetect.hpp>
#include <opencv2/ml/ml.hpp>  
#include <vector>
#include <iostream>

struct HogOptions{
    HogOptions():win_size(48),win_stride(48),block_size(16),block_stride(8),cell_size(8),nbins(9) {}
    HogOptions(int _win_size, int _win_stride, int _block_size, int _block_stride, int _cell_size, int _nbins):
                        win_size(_win_size),
                        win_stride(_win_stride),
                        block_size(_block_size),
                        block_stride(_block_stride),
                        cell_size(_cell_size),
                        nbins(nbins) {}
    int win_size;
    int win_stride;
    int block_size;
    int block_stride;
    int cell_size;
    int nbins;
};


void get_hog(cv::InputArray in, 
            std::vector<float>& descriptors,
            struct HogOptions& opt){
    cv::HOGDescriptor hog(cv::Size(opt.win_size, opt.win_size), 
                        cv::Size(opt.block_size, opt.block_size), 
                        cv::Size(opt.block_stride, opt.block_stride), 
                        cv::Size(opt.cell_size, opt.cell_size), opt.nbins);
    std::cout << "size = " << in.size() << std::endl;
    hog.compute(in, descriptors, cv::Size(opt.win_stride, opt.win_stride));
    std::cout << descriptors.size() << std::endl;
    return;
}

void visualize_hog(cv::InputArray in, cv::OutputArray out, std::vector<float>& descriptors, struct HogOptions& opt, int show=false){


    if(show){
        cv::imshow("HOG visualization", out);
    }
}
