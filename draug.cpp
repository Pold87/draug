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


static void help() {

  cout << "*****\ndraug: Drone data augmentation based on one map image\n*****\n" << endl;
  cout << "Usage:" << endl;
  cout << "- Interactive data visualizer:\ndraug gui [image]\nimage is the location of an image, for example picture1.png" << endl;
  cout << "- Random view generation:\ndraug rnd [int] [image]\nint specifies the number of samples (for example: draug rnd 1000 picture1.png). The corresponding coordinates are saved in targets.csv" << endl;
}


int main(int argc, char* argv[] ){

  if (argc < 2){
    help();
  } else if (argv[1] == string("gui")) {
    return gui_main(argv[2]);
  } else if (argv[1] == string("rnd")) {
    return random_main(atoi(argv[2]), argv[3], argv[4]);
  } else {
    help();
  }
}
