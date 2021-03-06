// Include standard libraries
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

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

const int scale_slider_max = 2000;
const int brightness_slider_max = 2000;
int angle, brightness, scale, contrast;
int alpha_g, beta_g, gamma_g;
int dx_g, dy_g, dz_g;
int f_g;
int kernel_size;

double alpha_d, beta_d, gamma_d;
double dx_d, dy_d, dz_d;
double f_d;


// Function rotateImage from:
// http://jepsonsblog.blogspot.nl/2012/11/rotation-in-3d-using-opencvs.html
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

    Mat tvec(4,1,cv::DataType<double>::type);

    tvec.at<double>(0) = dx;
    tvec.at<double>(1) = dy;
    tvec.at<double>(2) = dz;
    tvec.at<double>(3) = 1;

    //    cout << tvec.at<double>(0) << endl;
    //    cout << tvec.at<double>(1) << endl;
    //    cout << tvec.at<double>(2) << endl;

    R = R.t();  // rotation of inverse
    tvec = -R * tvec; // translation of inverse

    //    cout << tvec.at<double>(0) << endl;

    Mat T_new = (Mat_<double>(4, 4) <<
                 1, 0, 0, tvec.at<double>(0),
                 0, 1, 0, tvec.at<double>(1),
                 0, 0, 1, tvec.at<double>(2),
                 0, 0, 0, 1);

    // 3D -> 2D matrix
    Mat A2 = (Mat_<double>(3,4) <<
              f, 0, w/2, 0,
              0, f, h/2, 0,
              0, 0,   1, 0);
    // Final transformation matrix
    Mat trans = A2 * (T_new * (R * A1));

    // Apply matrix transformation
    warpPerspective(input, output, trans, input.size());

    cv::blur(output, output, Size(kernel_size + 1, kernel_size + 1), Point(-1,-1));
    
  }


/**
 * @function on_trackbar
 * @brief Callback for trackbar
 */
void on_trackbar( int, void* )
{

  alpha_d = (double) alpha_g;
  beta_d  = (double) beta_g;
  gamma_d = (double) gamma_g;


  cout << " cols " << original_img.cols << " rows " << original_img.rows << "\n";
  
  dx_d = (double) dx_g - (original_img.cols / 2);
  dy_d = (double) - dy_g + (original_img.rows / 2);
  dz_d = (double) - dz_g;

  f_d = (double) f_g;

  // Perform perspective transformations
  rotateImage(original_img, transformed_img,
              alpha_d, beta_d, gamma_d,
              dx_d, dy_d, dz_d,
              f_d);

  // Change brightness and contrast
  transformed_img.convertTo(transformed_img, -1, contrast, brightness);

  
  // SHOW
  imshow(transformed_img_win, transformed_img);
}

vector<double> generate_random(string filename) {

  std::random_device rd;
  std::mt19937 gen(rd());
  
  //  std::uniform_real_distribution<double> translation_x(0, original_img.cols);
  //  std::uniform_real_distribution<double> translation_y(0, original_img.rows);

  std::uniform_real_distribution<double> translation_x((210 / 2), original_img.cols - (210 / 2));
  std::uniform_real_distribution<double> translation_y((210 / 2), original_img.rows - (210 / 2));
  // such that 99 % (3 STD) of the translations are within the picture
  //std::normal_distribution<double> translation_x(original_img.cols / 2, 107);
  //std::normal_distribution<double> translation_y(original_img.rows / 2, 80);
  std::uniform_real_distribution<double> yaw_rotation(88.0, 92.0);
  std::normal_distribution<double> rotation(90, 1);
  std::uniform_real_distribution<double> height_dist(190, 210);
  std::uniform_real_distribution<double> blur_dist(0.0, 2.0);
  std::normal_distribution<double> brightness_dist(0.0, 2.0);
  std::uniform_real_distribution<double> contrast_dist(1.0, 1.3);

  double alpha_g = rotation(gen);
  double beta_g = rotation(gen);

  //double alpha_g = 90;
  //double beta_g = 90;
  double gamma_g = yaw_rotation(gen);
  //double gamma_g = 90;

  //cout << alpha_g << beta_g << gamma_g;
  
  double dx_g = translation_x(gen);
  double dy_g = translation_y(gen);
  //double dx_g = original_img.cols / 2;
  //double dy_g = original_img.rows / 2;
  double dz_g = height_dist(gen);
  //double dz_g = 1000;
  double f_g = 1000;
  
  alpha_d = (double) alpha_g;
  beta_d =  (double) beta_g;
  gamma_d = (double) gamma_g;

  dx_d = (double) dx_g - (original_img.cols / 2);
  dy_d = (double) - dy_g + (original_img.rows / 2);
  dz_d = (double) - dz_g;

  f_d = (double) f_g;

  rotateImage(original_img, transformed_img,
              alpha_d, beta_d, gamma_d,
              dx_d, dy_d, dz_d,
              f_d);


  contrast = contrast_dist(gen);
  brightness = brightness_dist(gen);
  kernel_size = blur_dist(gen);


  // Change brightness and contrast
  //  transformed_img.convertTo(transformed_img, -1, contrast, brightness);

  //cv::blur(transformed_img, transformed_img, Size(kernel_size + 1, kernel_size + 1), Point(-1,-1));

  resize(transformed_img, transformed_img, Size(640, 480));

  cv:imwrite(filename, transformed_img);

  vector<double> targets(6);
  targets[0] = alpha_d;
  targets[1] = beta_d;
  targets[2] = gamma_d;
  targets[3] = dx_g;
  targets[4] = original_img.rows - dy_g;
  targets[5] = - dz_d;

  return targets;
}



vector<double> generate_random_multiple(string original_img_path,  string filename) {

  original_img = cv::imread(original_img_path, CV_LOAD_IMAGE_COLOR);
  transformed_img = cv::imread(original_img_path, CV_LOAD_IMAGE_COLOR);

  return generate_random(filename);
  
  
}


int random_main(int samples, string original_img_path, string out_folder){
  
  // Path of map  

  std::string tracking_img_path = "img/tracking.png";
  
  original_img = cv::imread(original_img_path, CV_LOAD_IMAGE_COLOR);
  transformed_img = cv::imread(original_img_path, CV_LOAD_IMAGE_COLOR);

  //resize(original_img, original_img, Size(224, 224));
  //resize(transformed_img, transformed_img, Size(224, 224));
  
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


  //boost::filesystem::remove_all("genimgs");
  boost::filesystem::create_directory(out_folder);
  
  for (int i = 0; i < samples; i++) {

    string ext = ".png";

    boost::format fmter("%s/img_%05d%s");

    string file_name = str(fmter % out_folder % i % ext);
        
    vector<double> targets = generate_random(file_name);

    std::fprintf(targets_file, "%f,%f,%f,%f,%f,%f\n",
                 targets[0], targets[1], targets[2],
                 targets[3], targets[4], targets[5]);
    
  }

  fclose(targets_file);
  
}


int gui_main(string original_img_path){

  // Path of map  
  //std::string original_img_path = "img/dice.jpg";
  //std::string original_img_path = "img/maze_sep_s.jpg";
  std::string tracking_img_path = "img/tracking.png";

  original_img = cv::imread(original_img_path, CV_LOAD_IMAGE_COLOR);
  transformed_img = cv::imread(original_img_path, CV_LOAD_IMAGE_COLOR);


  //resize(original_img, original_img, Size(1224, 1224));
  //resize(transformed_img, transformed_img, Size(1224, 1224));


  tracking_img = cv::imread(tracking_img_path, CV_LOAD_IMAGE_COLOR);


  // Check for invalid input
  if (!original_img.data) {
      std::cout << "Could not open or find the image" << std::endl;
        return -1;
    }


   /// Initialize values

  alpha_g = 90; beta_g = 90; gamma_g = 90;
  dx_g = original_img.rows / 2; dy_g = original_img.cols / 2; dz_g = 200;
  f_g = 1000;
  
  contrast = 1;
  brightness = 0;
  kernel_size = 0;
  
  cv::namedWindow(transformed_img_win, cv::WINDOW_AUTOSIZE);
  cv::namedWindow(trackbars_win, cv::WINDOW_AUTOSIZE);


  createTrackbar("Brightness",
                 trackbars_win,
                 &brightness,
                 brightness_slider_max,
                 on_trackbar);


  createTrackbar("Blur",
                 trackbars_win,
                 &kernel_size,
                 20,
                 on_trackbar);


  createTrackbar("Rotation Pitch",
                 trackbars_win,
                 &alpha_g,
                 180,
                 on_trackbar);


  createTrackbar("Rotation Roll",
                 trackbars_win,
                 &beta_g,
                 180,
                 on_trackbar);


  createTrackbar("Rotation Yaw",
                 trackbars_win,
                 &gamma_g,
                 180,
                 on_trackbar);
  

  createTrackbar("X",
                 trackbars_win,
                 &dx_g,
                 scale_slider_max,
                 on_trackbar);


  createTrackbar("Y",
                 trackbars_win,
                 &dy_g,
                 scale_slider_max,
                 on_trackbar);


  createTrackbar("Height",
                 trackbars_win,
                 &dz_g,
                 1000,
                 on_trackbar);


  createTrackbar("Focal length",
                 trackbars_win,
                 &f_g,
                 1000,
                 on_trackbar);

  
  cv::imshow(transformed_img_win, original_img);
  cv::imshow(trackbars_win, tracking_img);

  cv::waitKey(0);

  return 0;
  
}
