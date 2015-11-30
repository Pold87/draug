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

#include "draug_helpers.h"

using namespace std;
using namespace cv;

using namespace boost::filesystem;


int main(int argc, char* argv[]) {

  int samples = atoi(argv[1]);
  string original_img_path = argv[2];
  path p(original_img_path);

  boost::filesystem::remove_all("genimgs_folder");
  boost::filesystem::create_directory("genimgs_folder");

  string file_dir = "genimgs_folder/";
  string file_base = "";
  string ext = ".png";

  // Iterate over files in folder

  for (auto i = directory_iterator(p); i != directory_iterator(); i++) {
    if (!is_directory(i->path())) {
      string original_img_path = i->path().string();


      string filenumber = i->path().stem().string();
      int k = stoi(filenumber);
      
      cout << "Image path: " << original_img_path << std::endl;
      
      Mat original_img, transformed_img;
      original_img = cv::imread(original_img_path, CV_LOAD_IMAGE_COLOR);
      transformed_img = cv::imread(original_img_path, CV_LOAD_IMAGE_COLOR);  
      // Check for invalid input
      if (!original_img.data) {
        std::cout << "Could not open or find the image" << std::endl;
        return -1;
      }

      for (int j = 0; j < samples; j++) {

        string file_name = file_dir + to_string(k) + "_" + to_string(j) + ext;
        
        generate_random_multiple(original_img_path, file_name);

  }

    } else {
      continue;
    }
  } 
  
}
