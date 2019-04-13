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

namespace cv
{
    struct CalibParams {
        /* Calib init parameters */
        // Path to pictures
        String pathToCalibPics;
        // Size of square, in milimeters
        float calibrationSquareSize;
        // Width of calib chessboard as number of squares
        int chessboardWidth;
        // Height of calib chessboard as number of squares
        int chessboardHeight;

        /* Camera parameters */
        // name of the camera (optional)
        String header;
        // Size of pixel in x and y axis in mm
        Point2d pixelSize;
        // Max resolution of the camera
        Point matrixMaxRes;
        // Current set resolution of the camera
        Point matrixCurrRes;
        // Size of the camera sensor in x and y axis in mm
        Point2d matrixSize;
        // Focal length in mm
        double focalLength;
    };
}

using namespace std;
using namespace cv;
using namespace patch;


void createKnownChessboardPosition(Size boardSize, float squareEdgeLength,
  vector<Point3f>& corners);
void getChessboardCorners(vector<Mat> images,
    vector< vector< Point2f > > &allFoundCorners,
    CalibParams camera, bool showResults);
void cameraCalibration(vector<Mat> calibrationImages, Size boardSize,
    float squareEdgeLength, Mat &cameraMatrix, Mat &distanceCoefficients,
    vector<Mat> &rVectors, vector<Mat> &tVectors, CalibParams camera);
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
void loadParametersFromXml(cv::String filename, CalibParams &camera);
void saveParametersToXml(cv::String filename, CalibParams camera, cv::String header);
cv::Mat getCameraMatrix(CalibParams camera);



#endif // OCVCALIB

