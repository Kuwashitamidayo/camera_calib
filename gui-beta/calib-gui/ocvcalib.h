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
        String pathToCalibPics;         // Path to pictures
        float calibrationSquareSize;    // Size of square, in milimeters
        int chessboardWidth;            // Width of calib chessboard as number of squares
        int chessboardHeight;           // Height of calib chessboard as number of squares

        /* Camera parameters */
        String header;                  // name of the camera (optional)
        Point2d pixelSize;              // Size of pixel in x and y axis in mm
        Point matrixMaxRes;             // Max resolution of the camera
        Point matrixCurrRes;            // Current set resolution of the camera
        Point2d matrixSize;             // Size of the camera sensor in x and y axis in mm
        double focalLength;             // Focal length in mm

        CalibParams() {
            pathToCalibPics = "";
            calibrationSquareSize = 0.0;
            chessboardWidth = 0;
            chessboardHeight = 0;
            header = "";
            pixelSize.x = 0;
            pixelSize.y = 0;
            matrixMaxRes.x = 0;
            matrixMaxRes.y = 0;
            matrixCurrRes.x = 0;
            matrixCurrRes.y = 0;
            matrixSize.x = 0;
            matrixSize.y = 0;
            focalLength = 0.0;
        };
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
double computeReprojectionErrors( const vector<vector<Point3f> >& objectPoints,
    const vector<vector<Point2f> >& imagePoints,
    const vector<Mat>& rvecs, const vector<Mat>& tvecs,
    const Mat& cameraMatrix , const Mat& distCoeffs,
    vector<float>& perViewErrors);

string createJpgFile(int &savedImageCount);
inline bool exists_file(const std::string &name);

void saveIntrinsicCameraParameters(cv::Mat &cameraMatrix);
void inline parseParameters(int argc, char** argv, cv::String &keys);
void loadParametersFromXml(cv::String filename, CalibParams &camera);
void saveParametersToXml(cv::String filename, CalibParams camera, cv::String header);

void setCameraParameters(CalibParams camera);
cv::Mat getCameraMatrix(CalibParams camera);
double getReprojectionError();
vector<float> getPerViewErrors();
void clearPerViewErrors();



#endif // OCVCALIB

