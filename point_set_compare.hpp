#include <Eigen/Dense>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio/videoio.hpp>

Eigen::Affine2d get_svd_transform(const Eigen::Matrix<double, -1, 2> &src, const Eigen::Matrix<double, -1, 2> &tgt)
{

  assert(src.rows() == tgt.rows());
  Eigen::Affine2d result = Eigen::Affine2d::Identity();
  Eigen::Matrix<double, -1, 2> src_ = src;
  Eigen::Matrix<double, -1, 2> tgt_ = tgt;
  Eigen::Vector2d centroid_src, centroid_tgt;
  // std::cout << "getting mean" << std::endl;

  centroid_src = src.colwise().mean();
  centroid_tgt = tgt.colwise().mean();

  std::cout << centroid_src << std::endl
            << centroid_tgt << std::endl;

  src_.colwise() -= centroid_src;
  tgt_.colwise() -= centroid_tgt;
  // std::cout << "getting svd" << std::endl;
  Eigen::Matrix2d mat = tgt_.transpose() * src_;
  Eigen::JacobiSVD<Eigen::Matrix2d> svd(mat, Eigen::ComputeFullU | Eigen::ComputeFullV);
  Eigen::Matrix2d u, v;
  u = svd.matrixU();
  v = svd.matrixV();
  // std::cout << "getting rot" << std::endl;
  result.linear() = u * v.transpose();
  if (result.linear().determinant() < 0)
  {
    Eigen::Matrix<double, 2, 2> inv;
    inv << 1, 0,
           0, -1;
    std::cout << "special case" << std::endl;
    // v.transpose().rowwise() *= v;
    v = v * inv;
    result.linear() = u * v.transpose();
  }
  // std::cout << "getting trans" << std::endl;
  result.translation() = centroid_tgt - result.linear() * centroid_src;
  return result;
}

// Return l2 distance of transformed points
double compare_point_sets(const Eigen::Matrix<double, -1, 2> &src, const Eigen::Matrix<double, -1, 2> &tgt, Eigen::Affine2d &result)
{
  int rows = src.rows();

  // std::cout << "src: \n" << src << std::endl;
  // std::cout << "tgt: \n" << tgt << std::endl;
  // std::cout << "getting tf" << std::endl;

  result = get_svd_transform(src, tgt);
  std::cout << "rot:" << std::endl
            << result.linear() << std::endl;
  std::cout << "trans:" << std::endl
            << result.translation() << std::endl;

  Eigen::Matrix<double, -1, 2> transformed_src = (result * src.transpose()).transpose();
  // transformed_src.resize(rows, 2);
  // for(int i=0; i<rows; i++){
  //   transformed_src.row(i) = result * src.row(i);
  // }

  std::cout << "before \n"
            << src << std::endl;
  std::cout << "after \n"
            << transformed_src << std::endl;

  Eigen::Matrix<double, -1, 2> diff = tgt - transformed_src.topLeftCorner(rows, 2);
  Eigen::Matrix<double, -1, 1> err = diff.rowwise().norm();

  cv::Mat before = cv::Mat::zeros(cv::Size(640, 480), CV_8UC3);
  cv::Mat after = cv::Mat::zeros(cv::Size(640, 480), CV_8UC3);
  for (int i = 0; i < src.rows(); i++)
  {
    cv::circle(before, cv::Point(src(i, 0), src(i, 1)), 2, cv::Scalar(0, 0, 255), 2);
  }

  for (int i = 0; i < tgt.rows(); i++)
  {
    cv::circle(before, cv::Point(tgt(i, 0), tgt(i, 1)), 2, cv::Scalar(255, 255, 0), 2);
    cv::circle(after, cv::Point(tgt(i, 0), tgt(i, 1)), 2, cv::Scalar(255, 255, 0), 2);
  }

  for (int i = 0; i < transformed_src.rows(); i++)
  {
    cv::circle(after, cv::Point(transformed_src(i, 0), transformed_src(i, 1)), 2, cv::Scalar(0, 0, 255), 2);
  }

  cv::imshow("before", before);
  cv::imshow("after", after);
  cv::waitKey(2);
  return err.sum();
}