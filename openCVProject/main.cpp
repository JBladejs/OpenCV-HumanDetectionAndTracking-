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

String person_cascade_name = "Samples/haarcascade_fullbody.xml";
CascadeClassifier person_cascade;
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

void showFPS(Mat frame, double frame_time) {
	double fps = 1000 / frame_time;
	stringstream ss;
	ss << setprecision(3) << fps;
	rectangle(frame, Point(0, frame.rows), Point(40, frame.rows - 15), Scalar(255, 255, 255), CV_FILLED);
	putText(frame, ss.str(), Point(0, frame.rows), FONT_HERSHEY_PLAIN, 1.0, Scalar(0, 0, 0));
}

void execute(Mat frame, string window_name, double frame_time) {
	resize(frame, frame, Size(640, 360));
	Mat gray_frame;
	//cvtColor(frame, gray_frame, COLOR_RGB2GRAY);
	//vector<Rect> rectangles;
	//person_cascade.detectMultiScale(gray_frame, rectangles);
	//for (int i = 0; i < rectangles.size(); i++) {
	//	rectangle(frame, rectangles[i], Scalar(255, 255, 0));
	//}
	showFPS(frame, frame_time);
	imshow(window_name, frame);
}

int play(string video_name) {
	turned_on = true;
	VideoCapture cap(video_name);
	double frame_delay = getFrameDelay(cap);
	double frame_time = frame_delay;
	Mat frame;
	string window_name = "preview";
	namedWindow(window_name, CV_WINDOW_AUTOSIZE);
	if (!person_cascade.load(person_cascade_name)) { printf("--(!)Error loading\n"); return -1; };
	while (cap.read(frame) && turned_on && getWindowProperty(window_name, 0) >= 0) {
		double start_time = clock();
		execute(frame, window_name, frame_time);
		double delay = frame_delay - (clock() - start_time);
		int key;
		if(delay > 0) key = waitKey(delay);
		else key = waitKey(1);
		interpretKey(key);
		frame_time = clock() - start_time;
	}
	cap.release();
	cvDestroyAllWindows();
	return 0;
}

int main() {
	return play("Samples/TownCentreXVID.avi");
}