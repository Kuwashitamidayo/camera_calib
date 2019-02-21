<p align="center">
  <img src="https://cdn-images-1.medium.com/max/1600/1*Mu7_d3e1qPtW1e7EgsX7LQ.png">
</p>

# Camera calibration using OpenCV - project
Alghoritm in C++ for camera calibration using OpenCV. The plan is to create a camera calibration program with some nice GUI (using Qt or cvui library). This is the small part of my master's dissertation that I will show here. The other part, which uses calibration parameters to undistort the camera and recognize pose of elements on the picture will not be shown nor avalable.
I will think of making both C++ and Python version (just to have some comparison) and maybe I will make some processing time comparison.
The base alghoritm is based on YouTube manual, see comments in the code.
Here I will also put some interesting links with useful information needed to do camera calibration correctly.

# To-do list
- make it able to run both in GUI and non-GUI mode,
- in GUI mode everything is set in window, no command window is shown,
- in non-GUI mode parameters must be passed via command window, displaying images with detected chessboard corners is only optional (minimum displaying),
- make the alghoritm more flexible; after compilation it must be able to be used with any calibration chessboard,
- find out how cv::CALIB_USE_INTRINSIC_GUESS works,
- make some kind of indicator which shows how good calibration was,
- optimize Makefile - for now it has some unnecesary stuff,
- more thing soon...

# Libraries
- OpenCV 3.4.4
