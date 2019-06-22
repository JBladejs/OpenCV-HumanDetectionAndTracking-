#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/core.hpp"
#include "opencv2/video.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/imgcodecs.hpp"

#include <windows.h>
#include <iomanip>  
#include <chrono> 
#include <iostream>
#include <conio.h>
#include <time.h>
#include <ctime>

using namespace cv;
using namespace std;

//CascadeClassifier person_cascade;
//CascadeClassifier upper_cascade;

//if(fullbody) person_cascade.detectMultiScale(gray_frame, rectangles, scaleFactor, minNeighbors, 0, Size(minSize, minSize));
//else upper_cascade.detectMultiScale(gray_frame, rectangles, scaleFactor, minNeighbors, 0, Size(minSize, minSize));
//if (rectangles.size() == 0) fullbody = !fullbody;

//person_cascade.load("C:/opencv/build/etc/haarcascades/haarcascade_fullbody.xml");
//upper_cascade.load("C:/opencv/build/etc/haarcascades/haarcascade_upperbody.xml");

HOGDescriptor hog;
bool turned_on;
bool fullbody;
bool track;

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
		case 't':
			track = !track;
			break;
	}
}

void showFPS(Mat frame, double frame_time) {
	double fps = 1000 / frame_time;
	stringstream ss;
	ss << setprecision(3) << fps;
	rectangle(frame, Point(0, frame.rows), Point(40, frame.rows - 15), Scalar(255, 255, 255), CV_FILLED);
	putText(frame, ss.str(), Point(0, frame.rows), FONT_HERSHEY_PLAIN, 1.0, Scalar(0, 0, 0));
}

void execute(Mat frame, string window_name, float scaleFactor, int minNeighbors, int minSize) {
	Mat gray_frame;
	cvtColor(frame, gray_frame, COLOR_RGB2GRAY);
	vector<Rect> rectangles;
	hog.detectMultiScale(gray_frame, rectangles);
	for (int i = 0; i < rectangles.size(); i++) {
		rectangle(frame, rectangles[i], Scalar(255, 255, 0));
	}
}

int play(string video_name) {
	turned_on = true;
	fullbody = true;
	track = true;
	VideoCapture cap(video_name);
	double frame_delay = getFrameDelay(cap);
	double frame_time = frame_delay;
	Mat frame;
	string window_name = "preview";
	namedWindow(window_name, CV_WINDOW_AUTOSIZE);
	namedWindow("params", CV_WINDOW_AUTOSIZE);

	int minNeighbors = 2, minSize = 15;
	float scaleFactor = 1.1;
	int scaleInt = 11;
	createTrackbar("scale factor", "params", &scaleInt, 200);
	createTrackbar("min Neigh", "params", &minNeighbors, 10);
	createTrackbar("min Size", "params", &minSize, 100);

	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
	while (cap.read(frame) && turned_on && getWindowProperty(window_name, 0) >= 0) {

		scaleFactor = (float)scaleInt / 100;
		if (scaleFactor < 1.01)
			scaleFactor = 1.1;
		if (minNeighbors == 0)
			minNeighbors = 1;

		double start_time = clock();
		resize(frame, frame, Size(640, 360));
		if(track) execute(frame, window_name, scaleFactor, minNeighbors, minSize);
		showFPS(frame, frame_time);
		imshow(window_name, frame);
		double delay = frame_delay - (clock() - start_time);
		int key;
		if(delay > 1) key = waitKey(delay);
		else key = waitKey(1);
		interpretKey(key);
		frame_time = clock() - start_time;
	}
	cap.release();
	cvDestroyAllWindows();
	return 0;
}

int main() {
	//faceRecognition();
	return play("Samples/person.mp4");
}