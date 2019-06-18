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

bool turned_on;

double getFrameDelay(VideoCapture capture) {
	double frame_delay = 1000 / capture.get(CV_CAP_PROP_FPS);
	if (frame_delay == 0) frame_delay = 33.3;
	return frame_delay;
}

void interpretKey(int key) {
	switch(key) {
		case 27:
			turned_on = false;
			break;
		case -1:
			break;
		default:
			cout << key << endl;
			break;
	}
}

void execute(Mat frame, string window_name) {
	imshow(window_name, frame);
}

void play(string video_name) {
	turned_on = true;
	VideoCapture cap(video_name);
	double frame_delay = getFrameDelay(cap);
	Mat frame;
	String window_name;
	namedWindow(window_name, CV_WINDOW_AUTOSIZE);
	while (cap.read(frame) && turned_on) {
		double time = clock();
		execute(frame, window_name);
		time -= clock();
		double delay = frame_delay - time;
		int key;
		if(delay > 0) key = waitKey(delay);
		else key = waitKey(1);
		interpretKey(key);
	}
	cap.release();
	cvDestroyAllWindows();
}

int main() {
	play("TownCentreXVID.avi");
	return 0;
}