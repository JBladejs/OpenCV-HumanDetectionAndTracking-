#include<opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/video.hpp>
#include "opencv2/videoio.hpp"
#include "opencv2/imgcodecs.hpp"

#include<iostream>
#include<conio.h>
#include <time.h>

using namespace cv;
using namespace std;

double getFrameDelay(VideoCapture capture) {
	double frame_delay = 1000 / capture.get(CV_CAP_PROP_FPS);
	if (frame_delay == 0) frame_delay = 33.3;
	return frame_delay;
}

int display(string video_name) {
	//opening videofile and getting deley between frames
	VideoCapture cap(video_name);
	double frame_delay = getFrameDelay(cap);

}

int main() {
	display("TownCentreXVID.avi");
	return 0;
}