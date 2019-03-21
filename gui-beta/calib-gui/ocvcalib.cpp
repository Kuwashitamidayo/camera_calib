#include <ocvcalib.h>





/*	Sources:
    https://www.youtube.com/watch?v=v7jutAmWJVQ
    https://www.youtube.com/watch?v=GYIQiV9Aw74
    https://docs.opencv.org/3.4.3/d4/d94/tutorial_camera_calibration.html

    calibration chessboard generator:
    https://calib.io/pages/camera-calibration-pattern-generator
*/

/* Calib init parameters */
// Path to pictures
string pathToCalibPics;
// Width of calib chessboard as number of squares
int chessboardWidth;
// Height of calib chessboard as number of squares
int chessboardHeight;
// Minimal amount of pics needed to start the calibration
unsigned int minAmountOfPicsToCalibrate = 20;
// Size of square, in milimeters
float calibrationSquareSize;
// Size of chessboard = square intersections in each axis
Size chessboardDimensions;

/* Camera parameters */
// Size of pixel in x and y axis in mm
Point2d pixelSize;
// Max resolution of the camera
Point matrixMaxRes;
// Current set resolution of the camera
Point matrixCurrRes;
// Size of the camera sensor in x and y axis in mm
Point2d matrixSize;
// Focal length in mm
double focalLength;// = 	6.0;

double fx = (double)matrixCurrRes.x * focalLength / matrixSize.x;
double fy = (double)matrixCurrRes.y * focalLength / matrixSize.y;
double cx = matrixCurrRes.x / 2;
double cy = matrixCurrRes.y / 2;



/* Beginning of the program */
void createKnownChessboardPosition(Size boardSize, float squareEdgeLength,
  vector<Point3f>& corners)
{
    for (int i = 0; i < boardSize.height; i++) {
        for (int j = 0; j < boardSize.width; j++) {
            corners.push_back(Point3f(j * squareEdgeLength,
              i * squareEdgeLength, 0.0f));
        }
    }
}

void getChessboardCorners(vector<Mat> images,
        vector< vector< Point2f > > &allFoundCorners, bool showResults = false)
{
    for (vector<Mat>::iterator iter = images.begin(); iter != images.end();
            ++iter) {
        vector<Point2f> pointBuf;
        bool found = findChessboardCorners(*iter, chessboardDimensions,
          pointBuf, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE);

        if (found) { allFoundCorners.push_back(pointBuf); }
        if (showResults) {
            drawChessboardCorners(*iter, chessboardDimensions,
                                    pointBuf, found);
            cv::imshow("Looking for corners", *iter);
            cv::waitKey(0);
        }
    }
}

void cameraCalibration(vector<Mat> calibrationImages, Size boardSize,
  float squareEdgeLength, Mat &cameraMatrix, Mat &distanceCoefficients,
  vector<Mat> &rVectors, vector<Mat> &tVectors)
{
    vector< vector< Point2f > > checkerboardImageSpacePoints;
    cout << "Searching chessboard corners in progress..." << endl;
    getChessboardCorners(calibrationImages,
      checkerboardImageSpacePoints, false);

    vector< vector< Point3f > > worldSpaceCornerPoints(1);
    cout << "Creating known chessboard positions..." << endl;
    createKnownChessboardPosition(boardSize, squareEdgeLength,
      worldSpaceCornerPoints[0]);
    cout << "Creating vector of size of chessboard corners amount..." << endl;
    worldSpaceCornerPoints.resize(checkerboardImageSpacePoints.size(),
      worldSpaceCornerPoints[0]);

    distanceCoefficients = Mat::zeros(8, 1, CV_64F);
    cout << "Camera calibration started." << endl;
    cout << cameraMatrix;
    calibrateCamera(worldSpaceCornerPoints, checkerboardImageSpacePoints,
      boardSize, cameraMatrix, distanceCoefficients, rVectors, tVectors
      /*, CALIB_USE_INTRINSIC_GUESS*/);
    cout << "Camera calibration finished." << endl;
}

bool saveCameraCalibration(string name, string nameCalibPic, Mat cameraMatrix,
  Mat distanceCoefficients, vector<Mat> rVectors, vector<Mat> tVectors)
{
    //Save calib parameters to .xml
    FileStorage outStream(name, FileStorage::WRITE);
    FileStorage outStreamPic(nameCalibPic, FileStorage::APPEND);
    outStream       << "cameraMatrix" << cameraMatrix;
    outStream       << "distanceCoefficients" << distanceCoefficients;
    outStreamPic    << "rVectors" << rVectors;
    outStreamPic    << "tVectors" << tVectors;
    outStream.release();
    outStreamPic.release();
    return true;
}

/* Error reprojection estimation - https://docs.opencv.org/3.1.0/d4/d94/tutorial_camera_calibration.html
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
        cv::projectPoints(objectPoints[i], rvecs[i], tvecs[i], cameraMatrix,
                            distCoeffs, imagePoints2);
        err = norm(imagePoints[i], imagePoints2, NORM_L2);
        size_t n = objectPoints[i].size();
        perViewErrors[i] = (float) std::sqrt(err*err/n);
        totalErr        += err*err;
        totalPoints     += n;
    }
    return std::sqrt(totalErr/totalPoints);
}*/


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

/*
Creates matrix of camera parameters which looks like this:
|	fx		0		0	|
|	0		fy		0	|
|	cx		cy		1	|
and saves all camera parameters to camera_intrinsic.xml
*/
void saveIntrinsicCameraParameters(cv::Mat &cameraMatrix) {

    cameraMatrix.at<double>(0, 0)	= fx;
    cameraMatrix.at<double>(1, 0)	= 0;
    cameraMatrix.at<double>(2, 0)	= 0;
    cameraMatrix.at<double>(0, 1)	= 0;
    cameraMatrix.at<double>(1, 1)	= fy;
    cameraMatrix.at<double>(2, 1)	= 0;
    cameraMatrix.at<double>(0, 2)	= cx;
    cameraMatrix.at<double>(1, 2)	= cy;
    cameraMatrix.at<double>(2, 2)	= 1;


    FileStorage outCamStream("camera_intrinsic.xml", FileStorage::WRITE);
    outCamStream << "header" 		<< "Raspberry Pi Camera 2.0 B";
    outCamStream << "pixelSize" 	<< pixelSize;
    outCamStream << "matrixMaxRes" 	<< matrixMaxRes;
    outCamStream << "matrixCurrRes"	<< matrixCurrRes;
    outCamStream << "matrixSize" 	<< matrixSize;
    outCamStream << "focalLength" 	<< focalLength;
    outCamStream << "cameraMatrix" 	<< cameraMatrix;
    outCamStream.release();

}

/*
Method to parse parameters from command line or xml file to a program.
It can also write loaded parameters to another xml file.
*/
void inline parseParameters(int argc, char** argv, cv::String &keys) {
    CommandLineParser parser(argc, argv, keys);
    if (parser.has("help"))
    {
        parser.printMessage();
        //return 0;
    }

    if (parser.has("terminal") && ~parser.has("loadconf")) {
        /*   Calib parameters   */
        pathToCalibPics 		= parser.get<string>("path");
        cout << "Current path: " << pathToCalibPics << std::endl;
        calibrationSquareSize 	= parser.get<float>("squaresize");;
        //corners in x or y = amount of squares minus 1
        chessboardWidth 		= parser.get<int>("w") - 1;
        chessboardHeight 		= parser.get<int>("h") - 1;

        /*   Camera parameters   */
        pixelSize   	= Point2d(parser.get<double>("px"),
          parser.get<double>("py"));
        matrixMaxRes  	= Point(parser.get<int>("maxresx"),
          parser.get<int>("maxresy"));
        matrixCurrRes 	= Point(parser.get<int>("currresx"),
          parser.get<int>("currresy"));
        matrixSize  	= Point2d((double)matrixMaxRes.x * pixelSize.x,
          (double)matrixMaxRes.y * pixelSize.y);
        focalLength 	= parser.get<double>("focal");
    }

    if (parser.has("terminal") && parser.has("loadconf")) {
        cv::FileStorage readParams("calib_conf.xml", FileStorage::READ);
        readParams["pathToCalibPics"] 		>>	pathToCalibPics;
        readParams["calibrationSquareSize"] >>	calibrationSquareSize;
        readParams["chessboardWidth"] 		>>	chessboardWidth;
        readParams["chessboardHeight"] 		>>	chessboardHeight;
        readParams["pixelSize"] 			>>	pixelSize;
        readParams["matrixMaxRes"] 			>>	matrixMaxRes;
        readParams["matrixCurrRes"] 		>>	matrixCurrRes;
        readParams["matrixSize"] 			>>	matrixSize;
        readParams["focalLength"] 			>>	focalLength;
        readParams.release();
    }

    if (parser.has("terminal") && parser.has("createconf")) {
        cv::FileStorage outCalibStream("calib_conf.xml", FileStorage::WRITE);
        outCalibStream << "pathToCalibPics"			<< pathToCalibPics;
        outCalibStream << "calibrationSquareSize"	<< calibrationSquareSize;
        outCalibStream << "chessboardWidth"			<< chessboardWidth;
        outCalibStream << "chessboardHeight"		<< chessboardHeight;
        outCalibStream << "pixelSize"				<< pixelSize;
        outCalibStream << "matrixMaxRes"			<< matrixMaxRes;
        outCalibStream << "matrixCurrRes"			<< matrixCurrRes;
        outCalibStream << "matrixSize"				<< matrixSize;
        outCalibStream << "focalLength"				<< focalLength;
        outCalibStream.release();
    }
    chessboardDimensions = Size(chessboardWidth, chessboardHeight);

    fx = (double)matrixCurrRes.x * focalLength / matrixSize.x;
    fy = (double)matrixCurrRes.y * focalLength / matrixSize.y;
    cx = matrixCurrRes.x / 2;
    cy = matrixCurrRes.y / 2;
}
