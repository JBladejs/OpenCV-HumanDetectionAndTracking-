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

#include "PersonTracker.h"

using namespace cv;
using namespace std;

HOGDescriptor hog;
CascadeClassifier custom_cascade;

bool turned_on;
bool track;
bool debug;
bool detect;

double hit_threshold;
int win_stride;
int padding;
double scale;
double final_threshold;

void interpretKey(int key)
{
	switch (key) 
	{
	case 27:
		turned_on = false;
		break;
	case 't':
		track = !track;
		break;
	case 'd':
		debug = !debug;
		break;
	}
}

void setDefaultBooleans()
{
	turned_on = true;
	track = true;
	detect = true;
	debug = false;
}

double getFrameDelay(VideoCapture capture)
{
	double frame_delay = 1000 / capture.get(CV_CAP_PROP_FPS);
	if (frame_delay == 0) frame_delay = 33.3;
	return frame_delay;
}

void showFPS(Mat frame, double frame_time)
{
	double fps = 1000 / frame_time;
	stringstream ss;
	ss << setprecision(3) << fps;
	rectangle(frame, Point(0, frame.rows), Point(40, frame.rows - 15), Scalar(255, 255, 255), CV_FILLED);
	putText(frame, ss.str(), Point(0, frame.rows), FONT_HERSHEY_PLAIN, 1.0, Scalar(0, 0, 0));
}

void drawPersons(Mat image, double arrowScale, PersonTracker *tracker)
{
	(*tracker).draw(image, arrowScale);
}

void detectPersonsROI(Mat image, string window_name, PersonTracker *tracker)
{
	vector<Rect> rectangles;
	selectROIs(window_name, image, rectangles, true, false);
	(*tracker).trackAll(rectangles);
}

void detectPersonsHOG(Mat image, string window_name, PersonTracker *tracker)
{
	Mat gray_image;
	cvtColor(image, gray_image, COLOR_RGB2GRAY);
	vector<Rect> rectangles;
	hog.detectMultiScale(image, rectangles, hit_threshold, Size(win_stride, win_stride), Size(padding, padding), scale, final_threshold);
	(*tracker).track(rectangles);
}

void detectPersonsHAAR(Mat image, string window_name, PersonTracker *tracker)
{
	Mat gray_image;
	cvtColor(image, gray_image, COLOR_RGB2GRAY);
	vector<Rect> rectangles;
	custom_cascade.detectMultiScale(image, rectangles);
	bool update = false;
	(*tracker).track(rectangles);
}

void detectAndTrack(string video_name, int type)
{
	setDefaultBooleans();
	VideoCapture cap(video_name);
	double frame_delay = getFrameDelay(cap);
	double frame_time = frame_delay;
	Mat frame;
	vector<Person> persons;

	double arrowScale;

	int arrow_scale_sli = 30;

	string main_window_name = "preview";
	string param_window_name = "params";


	int hit_threshold_sli = 0;
	int win_stride_sli = 1;
	int padding_sli = 0;
	int scale_sli = 105;
	int final_threshold_sli = 10;


	namedWindow(main_window_name, CV_WINDOW_AUTOSIZE);

	custom_cascade.load("Samples/cascade.xml");
	hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());

	PersonTracker tracker = PersonTracker(10);

	while (cap.read(frame) && turned_on)
	{
		double start_time = clock();


		hit_threshold = (double)hit_threshold_sli / 10.0;
		win_stride = win_stride_sli * 8;
		padding = padding_sli;
		scale = (double)scale_sli / 100.0;
		final_threshold = (double)final_threshold_sli / 10.0;


		if (debug)
		{
			namedWindow(param_window_name, CV_WINDOW_AUTOSIZE);
			createTrackbar("Arrow Scale", param_window_name, &arrow_scale_sli, 90);
			createTrackbar("Hit Threshold", param_window_name, &hit_threshold_sli, 100);
			createTrackbar("Win Stride", param_window_name, &win_stride_sli, 5);
			createTrackbar("Padding", param_window_name, &padding_sli, 128);
			createTrackbar("Scale", param_window_name, &scale_sli, 150);
			createTrackbar("Final Threshold", param_window_name, &final_threshold_sli, 100);
		}
		else
		{
			destroyWindow(param_window_name);
		}
		arrowScale = 1 + (arrow_scale_sli / 10);

		resize(frame, frame, Size(frame.cols / 2, frame.rows / 2));
		if (track)
		{
			switch (type)
			{
			case 1:
				detectPersonsHOG(frame, main_window_name, &tracker);
				break;
			case 2:
				if (detect)
				{
					detectPersonsROI(frame, main_window_name, &tracker);
				}
				else
				{
					detectPersonsHOG(frame, main_window_name, &tracker);
				}
				break;
			case 3:
				detectPersonsHAAR(frame, main_window_name, &tracker);
				break;
			}
			drawPersons(frame, arrowScale, &tracker);
		}

		showFPS(frame, frame_time);
		imshow(main_window_name, frame);

		double delay = frame_delay - (clock() - start_time);
		int key;
		if (delay > 1) key = waitKey(delay);
		else key = waitKey(1);
		interpretKey(key);

		frame_time = clock() - start_time;
		detect = false;
	}
	persons.clear();
	cap.release();
	destroyAllWindows();
}

int main()
{
	int choice = 0;
	string video_file;
	while (choice != 4)
	{
		cout << "Input a number to select an excecution type:" << endl;
		cout << "1 - Selecting a person to track and than tracking it." << endl;
		cout << "2 - Users selects person to track and is is tracked by a HOG descriptor." << endl;
		cout << "3 - Detecting and tracking a person based on a haar clasifiaer trained on that person." << endl;
		cout << "Type anything else to close the program";

		cin >> choice;
		switch (choice)
		{
		default:
			choice = 4;
			break;
		case 1:
		case 2:
			cout << "Input a video file name:" << endl;
			cin >> video_file;
			detectAndTrack(video_file, choice);
			break;
		case 3:
			detectAndTrack("Samples/person.mp4", choice);
			break;
		}
	}
}
