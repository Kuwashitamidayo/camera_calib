<p align="center">
  <img src="https://cdn-images-1.medium.com/max/1600/1*Mu7_d3e1qPtW1e7EgsX7LQ.png">
</p>

# Camera calibration using OpenCV - project
The plan is to create a camera calibration program with some nice GUI (using Qt library). This is the small part of my master's dissertation that I will show here. The other part, which uses calibration parameters to undistort the camera and recognize pose of elements on the picture will not be shown nor avalable.\
I will think of making both C++ and Python versions (just to have some comparison) and maybe I will make some processing time comparison.\
The base alghoritm is based on YouTube manual, see comments in the code.\
Here I will also put some interesting links with useful information needed to do camera calibration correctly.

# To-do list
- (90%) make it able to run both in GUI and non-GUI mode (for now in seperate files, planned to do it in one file),
- (90%) in GUI mode everything is set in window, no command window is shown,
- (95%) in non-GUI mode parameters must be passed via command window, displaying images with detected chessboard corners is only optional (minimum displaying),
- (100%) make the alghoritm more flexible; after compilation it must be able to be used with any calibration chessboard,
- (20%) find out how *cv::CALIB_USE_INTRINSIC_GUESS* works,
- (90%) make some kind of indicator which shows how good calibration was,
- (100%) optimize Makefile - for now it has some unnecesary stuff,
- (60%) run GUI and terminal mode of the program within one file,
- more things soon...

# Sample screenshots of the program

<p align="center">
  <img src="https://raw.githubusercontent.com/Kuwashitamidayo/camera_calib/master/Screenshots/screen_003.png">
</p>

# How to run the program

## Using terminal mode
- open Terminal, go to folder *terminal_beta*,
- make the program using *make*,
- call program in terminal mode using *./calibration -terminal*

## Using gui mode
- open QT Creator,
- open the project in *gui_beta*,
- compile it and run.


# Libraries and used software
- OpenCV 3.4.4
- Qt Creator 3.5.1 based on Qt 5.5.1 or newer (Qt Creator 4.5.2 based on Qt 5.9.5 was also OK)
- Linux (here Linux Mint 18.3 and 19.1), after a few changes it can be converted to run under Windows
