#ifndef DRAUG_HELPERS_H
#define DRAUG_HELPERS_H


// Include standard libraries
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <boost/filesystem.hpp>

// Include OpenCV libraries

#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui.hpp"

#include <random>

void rotateImage(const cv::Mat &input, cv::Mat &output, double alpha, double beta, double gamma, double dx, double dy, double dz, double f);
void on_trackbar( int, void* );
std::vector<double> generate_random(std::string filename);
std::vector<double> generate_random_multiple(std::string original_img_path,  std::string filename);
int random_main(int samples, std::string original_img_path);
int gui_main(std::string original_img_path);

#endif
