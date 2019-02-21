#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sstream>

namespace patch
{
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

/*	Sources:
	https://www.youtube.com/watch?v=v7jutAmWJVQ
	https://www.youtube.com/watch?v=GYIQiV9Aw74
	https://docs.opencv.org/3.4.3/d4/d94/tutorial_camera_calibration.html
	calibration chessboard generator: https://calib.io/pages/camera-calibration-pattern-generator
*/

/* Calib init parameters */
const string pathToCalibPics = "Pics/";
const int chessboardWidth = 7;						//8 squares in x axis means 7 intersections
const int chessboardHeight = 10;					//11 squares in y axis means 10 intersections
const int minAmountOfPicsToCalibrate = 20;			//minimal amountof pics needed to start the calibration
const float calibrationSquareSize = 15.0f;			//size of square, in milimeters
const Size chessboardDimensions = Size(chessboardWidth, chessboardHeight);	//wymiry planszy, liczone są punkty przecięć białych kwadratów z czarnymi

/* Camera parameters */
const Point2d pixelSize = Point2d(0.0014, 0.0014);
const Point matrixMaxRes= Point(2592, 1944);
const Point matrixCurrRes= Point(640, 480);
const Point2d matrixSize= Point2d((double)matrixMaxRes.x * pixelSize.x, (double)matrixMaxRes.y * pixelSize.y);
const double focalLength = 6.0;

const double fx = (double)matrixCurrRes.x * focalLength / matrixSize.x;
const double fy = (double)matrixCurrRes.y * focalLength / matrixSize.y;
const double cx = matrixCurrRes.x / 2;
const double cy = matrixCurrRes.y / 2;

/* Beginning of the program */
void createKnownChessboardPosition(Size boardSize, float squareEdgeLength, vector<Point3f>& corners)
{
	for (int i = 0; i < boardSize.height; i++) {
		for (int j = 0; j < boardSize.width; j++) {
			corners.push_back(Point3f(j * squareEdgeLength, i * squareEdgeLength, 0.0f));
		}
	}
}

void getChessboardCorners(vector<Mat> images, vector< vector< Point2f > > &allFoundCorners, bool showResults = false)
{
	for (vector<Mat>::iterator iter = images.begin(); iter != images.end(); ++iter) {
		vector<Point2f> pointBuf;
		bool found = findChessboardCorners(*iter, chessboardDimensions, pointBuf, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE);
		
		if (found) { allFoundCorners.push_back(pointBuf); }
		if (showResults) {
			drawChessboardCorners(*iter, chessboardDimensions, pointBuf, found);
			imshow("Looking for corners", *iter);
			waitKey(0);
		}
	}
}

void cameraCalibration(vector<Mat> calibrationImages, Size boardSize, float squareEdgeLength, Mat &cameraMatrix, Mat &distanceCoefficients, vector<Mat> &rVectors, vector<Mat> &tVectors)
{
	vector< vector< Point2f > > checkerboardImageSpacePoints;
	cout << "Searching chessboard corners in progress..." << endl;
	getChessboardCorners(calibrationImages, checkerboardImageSpacePoints, false);

	vector< vector< Point3f > > worldSpaceCornerPoints(1);
	cout << "Creating known chessboard positions..." << endl;
	createKnownChessboardPosition(boardSize, squareEdgeLength, worldSpaceCornerPoints[0]);
	cout << "Creating vector of size equal to amount of chessboard corners..." << endl;
	worldSpaceCornerPoints.resize(checkerboardImageSpacePoints.size(), worldSpaceCornerPoints[0]);

	distanceCoefficients = Mat::zeros(8, 1, CV_64F);
	cout << "Camera calibration started." << endl;
	cout << cameraMatrix;
	calibrateCamera(worldSpaceCornerPoints, checkerboardImageSpacePoints, boardSize, cameraMatrix, distanceCoefficients, rVectors, tVectors/*, CALIB_USE_INTRINSIC_GUESS*/);
	cout << "Camera calibration finished." << endl;
}

bool saveCameraCalibration(string name, string nameCalibPic, Mat cameraMatrix, Mat distanceCoefficients, vector<Mat> rVectors, vector<Mat> tVectors)
{
	//Zapis parametrów kalibracji do pliku .xml
	FileStorage outStream(name, FileStorage::WRITE);
	FileStorage outStreamPic(nameCalibPic, FileStorage::APPEND);
	outStream << "cameraMatrix" << cameraMatrix;
	outStream << "distanceCoefficients" << distanceCoefficients;
	outStreamPic << "rVectors" << rVectors;
	outStreamPic << "tVectors" << tVectors;
	outStream.release();
	outStreamPic.release();
	return true;
}

/* Error reprojection estimation - https://docs.opencv.org/3.1.0/d4/d94/tutorial_camera_calibration.html */
static double computeReprojectionErrors( const vector<vector<Point3f> >& objectPoints,
                                         const vector<vector<Point2f> >& imagePoints,
                                         const vector<Mat>& rvecs, const vector<Mat>& tvecs,
                                         const Mat& cameraMatrix , const Mat& distCoeffs,
                                         vector<float>& perViewErrors)
{
    vector<Point2f> imagePoints2;
    size_t totalPoints = 0;
    double totalErr = 0, err;
    perViewErrors.resize(objectPoints.size());
    for(size_t i = 0; i < objectPoints.size(); ++i )
    {
        cv::projectPoints(objectPoints[i], rvecs[i], tvecs[i], cameraMatrix, distCoeffs, imagePoints2);
        err = norm(imagePoints[i], imagePoints2, NORM_L2);
        size_t n = objectPoints[i].size();
        perViewErrors[i] = (float) std::sqrt(err*err/n);
        totalErr        += err*err;
        totalPoints     += n;
    }
    return std::sqrt(totalErr/totalPoints);
}


string createJpgFile(int &savedImageCount) {
	std::string filename = pathToCalibPics;
	filename += "calib_pic_";
	if (savedImageCount < 100) filename += "0";
	if (savedImageCount < 10) filename += "0";
	filename += patch::to_string(savedImageCount);
	filename += ".png";
	savedImageCount++;
	return filename;
}

inline bool exists_file(const std::string &name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

void saveIntrinsicCameraParameters(cv::Mat &cameraMatrix) {
	cameraMatrix.at<double>(0, 0)= fx;		cameraMatrix.at<double>(0, 1) = 0;      cameraMatrix.at<double>(0, 2)= cx;
	cameraMatrix.at<double>(1, 0)= 0;		cameraMatrix.at<double>(1, 1) = fy;		cameraMatrix.at<double>(1, 2)= cy;
	cameraMatrix.at<double>(2, 0)= 0;		cameraMatrix.at<double>(2, 1) = 0;		cameraMatrix.at<double>(2, 2)= 1;
		
	//do {
		FileStorage outCamStream("camera_intrinsic.xml", FileStorage::WRITE);
		outCamStream << "header" 		<< "Raspberry Pi Camera 2.0 B";
		outCamStream << "pixelSize" 	<< pixelSize;
		outCamStream << "matrixMaxRes" 	<< matrixMaxRes;
		outCamStream << "matrixCurrRes"	<< matrixCurrRes;
		outCamStream << "matrixSize" 	<< matrixSize;
		outCamStream << "focalLength" 	<< focalLength;
		outCamStream << "cameraMatrix" 	<< cameraMatrix;
		outCamStream.release();		
	//} while ( exists_file("camera_intrinsic.xml") );
}

int main()
{
	Mat frame;
	Mat drawToFrame;

	Mat distanceCoefficients;
	vector<Mat> tVectors, rVectors;

	vector<Mat> savedImages;
	vector< vector< Point2f > > markerCorners, rejectedCandidates;
	
	cv::Mat cameraMatrix = Mat::eye(3, 3, CV_64F);
	saveIntrinsicCameraParameters(cameraMatrix);
	
	VideoCapture vid(0);

	if (!vid.isOpened()) { return 0; }

	int framesPerSecond = 1;
	cout << "Lokalizacja zdjec: " << pathToCalibPics << "" << endl;
	cout << fx << std::endl;
	cout << fy << std::endl;
	cout << cx << std::endl;
	cout << cy << std::endl;
	cout << matrixCurrRes.x << std::endl;
	cout << matrixCurrRes.y << std::endl;
	cout << matrixSize.x << std::endl;
	cout << matrixSize.y << std::endl;

	namedWindow("Kamerka internetowa", CV_WINDOW_AUTOSIZE);


	int savedImageCount = 0;

	while (true) 
	{
		
		string filename;
		if (!vid.read(frame)) { break; }

		vector<Vec2f> foundPoints;
		bool found = false;

		found = findChessboardCorners(frame, chessboardDimensions, foundPoints, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE | CV_CALIB_CB_FAST_CHECK);		// | CV_CALIB_CB_FAST_CHECK
		frame.copyTo(drawToFrame);
		drawChessboardCorners(drawToFrame, chessboardDimensions, foundPoints, found);
		if (found)
			imshow("Kamerka internetowa", drawToFrame); 
		else
			imshow("Kamerka internetowa", frame);
		char character = waitKey(1000 / framesPerSecond);

		switch (character) 
		{
		case ' ':
			//spacja - zapisz obraz
			if (found) {
				Mat temp;

				 do {
					filename = createJpgFile(savedImageCount);
					cout << filename << std::endl;
				} while (exists_file(filename));
				cout << filename << std::endl;
				frame.copyTo(temp);
				imwrite(filename, temp);
				cout << "Picture " << filename << " saved." << endl;
				savedImages.push_back(temp);
			}
			break;
		case 9: 
			{
			//tab - read calibration pics from calib_pictures folder w folderze programu
			savedImageCount = 0;
			cout << "Loading pics from: " << pathToCalibPics << " to the memory in progress..." << endl;
			vector<string> calibFiles;

			do {
				Mat tempPic;
				filename = createJpgFile(savedImageCount);
				cout << "Check if " << filename << " exists..." << endl;

				if (exists_file(filename)) {
					Mat resultPic;
					tempPic = imread(filename, 1);
					//imshow(filename, tempPic);
					savedImages.push_back(tempPic);
					calibFiles.push_back(filename);
					found = findChessboardCorners(tempPic, chessboardDimensions, foundPoints, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE);		// | CV_CALIB_CB_FAST_CHECK
					frame.copyTo(drawToFrame);
					drawChessboardCorners(tempPic, chessboardDimensions, foundPoints, found);
					imshow(filename, tempPic);
					cout << "Zaladowano " << filename << "." << endl;
				}
			
				
			} while (exists_file(filename));
			cout << "Loading picture finished." << endl;

			FileStorage outStream("camera_calibration_pic_data.xml", FileStorage::WRITE);
			outStream << "picsUsedToCalib" << calibFiles;
			outStream.release();
			}
			break;
		case 13:
			//enter - starts the calibration
			cout << "Calibration has started..." << endl;

			if (savedImages.size() > minAmountOfPicsToCalibrate) {
				cameraCalibration(savedImages, chessboardDimensions, calibrationSquareSize, cameraMatrix, distanceCoefficients, rVectors, tVectors);
				saveCameraCalibration("camera_calibration.xml", "camera_calibration_pic_data.xml", cameraMatrix, distanceCoefficients, rVectors, tVectors);
				cout << "Calibration ended successfully!" << endl;
				break;
			}
			cout << "Not enough pictures for calibration! At least " << minAmountOfPicsToCalibrate <<" of pics are needed." << endl;
			break;
		case 27:
			//escape - exit program
			return 0;
			break;
		}
	}

	return 0;
}

