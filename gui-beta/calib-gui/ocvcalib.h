#ifndef OCVCALIB
#define OCVCALIB

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sstream>

namespace patch
{
    //Converts any type to string.
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

using namespace std;
using namespace cv;
using namespace patch;

void createKnownChessboardPosition(Size boardSize, float squareEdgeLength,
  vector<Point3f>& corners);
void getChessboardCorners(vector<Mat> images,
    vector< vector< Point2f > > &allFoundCorners, bool showResults);
void cameraCalibration(vector<Mat> calibrationImages, Size boardSize,
    float squareEdgeLength, Mat &cameraMatrix, Mat &distanceCoefficients,
    vector<Mat> &rVectors, vector<Mat> &tVectors);
bool saveCameraCalibration(string name, string nameCalibPic, Mat cameraMatrix,
    Mat distanceCoefficients, vector<Mat> rVectors, vector<Mat> tVectors);
/*static double computeReprojectionErrors( const vector<vector<Point3f> >& objectPoints,
    const vector<vector<Point2f> >& imagePoints,
    const vector<Mat>& rvecs, const vector<Mat>& tvecs,
    const Mat& cameraMatrix , const Mat& distCoeffs,
    vector<float>& perViewErrors);*/
string createJpgFile(int &savedImageCount);
inline bool exists_file(const std::string &name);
void saveIntrinsicCameraParameters(cv::Mat &cameraMatrix);
void inline parseParameters(int argc, char** argv, cv::String &keys);


#endif // OCVCALIB

