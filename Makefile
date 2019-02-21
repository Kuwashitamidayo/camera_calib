#
# Basic OpenCV example tested on Ubuntu
#

CPPINCLUDE_DIRS =  -I. -I/usr/local/include/opencv-lopencv_features2d
CPPLIBS = -L/usr/local/lib/opencv -lopencv_core -lopencv_features2d -lopencv_video -lopencv_ccalib
LIBS =

CPP = g++ 
CFLAGS = -std=c++11 -Wall -c -I.
CPPFLAGS = -Wall $(INCLUDE_DIRS)
LFLAGS = -Wall 

CPPSOURCE = calibration.cpp
CPPOUTFILE = calibration
CPPOBJS = $(CPPSOURCE:.cpp=.o)

all: $(CPPOUTFILE)

$(CPPOUTFILE): $(CPPOBJS)
	$(CPP) $(CPPFLAGS) $(CPPOBJS) -o $(CPPOUTFILE) `pkg-config --libs opencv` $(CPPLIBS)

clean:
	rm -f *.o $(CPPOUTFILE)


#include <vector>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/core/ocl.hpp>
