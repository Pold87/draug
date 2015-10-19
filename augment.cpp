// Include standard libraries
#include <iostream>

// Include OpenCV libraries

#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui.hpp"

#include <random>


using namespace std;
using namespace cv;


/// Global Variables

cv::Mat original_img, transformed_img, tracking_img;

std::string transformed_img_win = "Transformed Image";
std::string trackbars_win = "Trackbars";

const int alpha_slider_max = 100;
const int scale_slider_max = 1000;
int angle, brightness, scale;
int alpha_g, beta_g, gamma_g;
int dx_g, dy_g, dz_g;
int f_g;
int kernel_size;

double alpha_d, beta_d, gamma_d;
double dx_d, dy_d, dz_d;
double f_d;


void rotateImage(const Mat &input, Mat &output, double alpha, double beta, double gamma, double dx, double dy, double dz, double f)
  {
    alpha = (alpha - 90) * CV_PI/180.;
    beta =  (beta - 90) * CV_PI/180.;
    gamma = (gamma - 90) * CV_PI/180.;

    // get width and height for ease of use in matrices
    double w = (double) input.cols;
    double h = (double) input.rows;

    // Projection 2D -> 3D matrix
    Mat A1 = (Mat_<double>(4,3) <<
              1, 0, -w/2,
              0, 1, -h/2,
              0, 0,    0,
              0, 0,    1);
    // Rotation matrices around the X, Y, and Z axis
    Mat RX = (Mat_<double>(4, 4) <<
              1,          0,           0, 0,
              0, cos(alpha), -sin(alpha), 0,
              0, sin(alpha),  cos(alpha), 0,
              0,          0,           0, 1);
    
    Mat RY = (Mat_<double>(4, 4) <<
              cos(beta), 0, -sin(beta), 0,
              0, 1,          0, 0,
              sin(beta), 0,  cos(beta), 0,
              0, 0,          0, 1);
    Mat RZ = (Mat_<double>(4, 4) <<
              cos(gamma), -sin(gamma), 0, 0,
              sin(gamma),  cos(gamma), 0, 0,
              0,          0,           1, 0,
              0,          0,           0, 1);
    // Composed rotation matrix with (RX, RY, RZ)
    Mat R = RX * RY * RZ;
    // Translation matrix
    Mat T = (Mat_<double>(4, 4) <<
             1, 0, 0, dx,
             0, 1, 0, dy,
             0, 0, 1, dz,
             0, 0, 0, 1);
    // 3D -> 2D matrix
    Mat A2 = (Mat_<double>(3,4) <<
              f, 0, w/2, 0,
              0, f, h/2, 0,
              0, 0,   1, 0);
    // Final transformation matrix
    Mat trans = A2 * (T * (R * A1));
    // Apply matrix transformation
    warpPerspective(input, output, trans, input.size());

    cv::blur(output, output, Size(kernel_size + 1, kernel_size + 1), Point(-1,-1));
    
  }



void on_trackbar( int, void* )
{

  // ROTATION

  // get rotation matrix for rotating the image around its center
  cv::Point2f center(original_img.cols / 2.0,
                     original_img.rows / 2.0);
  
  cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);

  cv::warpAffine(original_img,
                 transformed_img,
                 rot,
                 original_img.size());

  
  // SCALE

  Size size(100 + scale, 100 + scale);

  cout << "Size " << size;
  cout << "scale" << scale;
  
  resize(transformed_img, transformed_img, size);


  // SHOW
  imshow(transformed_img_win, transformed_img);
}



/**
 * @function on_trackbar
 * @brief Callback for trackbar
 */
void on_trackbar2( int, void* )
{

  alpha_d = (double) alpha_g;
  beta_d = - (double) beta_g;
  gamma_d = (double) gamma_g;

  dx_d = (double) - dx_g + original_img.rows;
  dy_d = (double) - dy_g + original_img.cols;
  dz_d = (double) dz_g;

  f_d = (double) f_g;

  rotateImage(original_img, transformed_img,
              alpha_d, beta_d, gamma_d,
              dx_d, dy_d, dz_d,
              f_d);

  // SHOW
  imshow(transformed_img_win, transformed_img);
}


vector<double> generate_random(string filename) {

  std::random_device rd;
  std::mt19937 gen(rd());
  
  std::uniform_real_distribution<double> translation(0, original_img.rows);

  std::uniform_real_distribution<double> yaw_rotation(0.0, 180.0);
  std::normal_distribution<double> rotation(90, 45);
  std::normal_distribution<double> height_dist(150, 90);

  double alpha_g = rotation(gen);
  double beta_g = rotation(gen);
  double gamma_g = yaw_rotation(gen);

  //cout << alpha_g << beta_g << gamma_g;
  
  double dx_g = translation(gen);
  double dy_g = translation(gen);
  double dz_g = height_dist(gen);

  //cout << dx_g << dy_g << dz_g;
  
  
  double f_g = 300;
  
  alpha_d = (double) alpha_g;
  beta_d = - (double) beta_g;
  gamma_d = (double) gamma_g;

  dx_d = (double) - dx_g + original_img.rows;
  dy_d = (double) - dy_g + original_img.cols;
  dz_d = (double) dz_g;

  f_d = (double) f_g;

  rotateImage(original_img, transformed_img,
              alpha_d, beta_d, gamma_d,
              dx_d, dy_d, dz_d,
              f_d);

  cv:imwrite(filename, transformed_img);

  vector<double> targets(6);
  targets[0] = alpha_d;
  targets[1] = beta_d;
  targets[2] = gamma_d;
  targets[3] = dx_d;
  targets[4] = dy_d;
  targets[5] = dz_d;

  return targets;
}


int main(int argc, char* argv[] ){
  
  
  // Path of map  
  std::string original_img_path = "img/dice.jpg";
  //std::string original_img_path = "img/maze_sep_s.jpg";
  std::string tracking_img_path = "img/tracking.png";
  
  original_img = cv::imread(original_img_path, CV_LOAD_IMAGE_COLOR);
  transformed_img = cv::imread(original_img_path, CV_LOAD_IMAGE_COLOR);
  
  tracking_img = cv::imread(tracking_img_path, CV_LOAD_IMAGE_COLOR);
  
  
  // Check for invalid input
  if (!original_img.data) {
    std::cout << "Could not open or find the image" << std::endl;
    return -1;
  }

  FILE* targets_file = fopen("targets.csv", "w+");

  std::fprintf(targets_file, "%s,%s,%s,%s,%s,%s\n",
               "Pitch", "Roll", "Yaw",
               "x", "y", "z");

  
  for (int i = 0; i < 1000; i++) {

    string file_dir = "genimgs/";
    string file_base = "";
    string ext = ".png";

    string file_name = file_dir + file_base + to_string(i) + ext;
        
    vector<double> targets = generate_random(file_name);

    std::fprintf(targets_file, "%f,%f,%f,%f,%f,%f\n",
                 targets[0], targets[1], targets[2],
                 targets[3], targets[4], targets[5]);
    
  }

  fclose(targets_file);
  
}

int main2(int argc, char* argv[] ){


  // Path of map  
  std::string original_img_path = "img/dice.jpg";
  //std::string original_img_path = "img/maze_sep_s.jpg";
  std::string tracking_img_path = "img/tracking.png";

  original_img = cv::imread(original_img_path, CV_LOAD_IMAGE_COLOR);
  transformed_img = cv::imread(original_img_path, CV_LOAD_IMAGE_COLOR);

  tracking_img = cv::imread(tracking_img_path, CV_LOAD_IMAGE_COLOR);


    // Check for invalid input
  if (!original_img.data) {
      std::cout << "Could not open or find the image" << std::endl;
        return -1;
    }


   /// Initialize values

  alpha_g = 90; beta_g = 90; gamma_g = 90;
  dx_g = original_img.rows; dy_g = original_img.cols; dz_g = 300;
  f_g = 200;

  brightness = 0;
  kernel_size = 0;
  
  cv::namedWindow(transformed_img_win, cv::WINDOW_AUTOSIZE);
  cv::namedWindow(trackbars_win, cv::WINDOW_AUTOSIZE);


  createTrackbar("Brightness",
                 trackbars_win,
                 &brightness,
                 alpha_slider_max,
                 on_trackbar2);


  createTrackbar("Blur",
                 trackbars_win,
                 &kernel_size,
                 20,
                 on_trackbar2);


  createTrackbar("Rotation Pitch",
                 trackbars_win,
                 &alpha_g,
                 180,
                 on_trackbar2);


  createTrackbar("Rotation Roll",
                 trackbars_win,
                 &beta_g,
                 180,
                 on_trackbar2);


  createTrackbar("Rotation Yaw",
                 trackbars_win,
                 &gamma_g,
                 180,
                 on_trackbar2);
  

  createTrackbar("Dx_g",
                 trackbars_win,
                 &dx_g,
                 scale_slider_max,
                 on_trackbar2);


  createTrackbar("Dy_g",
                 trackbars_win,
                 &dy_g,
                 scale_slider_max,
                 on_trackbar2);


  createTrackbar("Height",
                 trackbars_win,
                 &dz_g,
                 1000,
                 on_trackbar2);


  createTrackbar("f_g",
                 trackbars_win,
                 &f_g,
                 1000,
                 on_trackbar2);

  
  
  // Show some stuff
  //on_trackbar( alpha_slider, 0 );

  cv::imshow(transformed_img_win, original_img);
  cv::imshow(trackbars_win, tracking_img);

  cv::waitKey(0);
  
}
