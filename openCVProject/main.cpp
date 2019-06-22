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

#include "Person.h"

using namespace cv;
using namespace std;

HOGDescriptor hog;
CascadeClassifier custom_cascade;
vector<Person> persons;

bool turned_on;
bool fullbody;
bool track;
bool debug;

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
	fullbody = true;
	track = true;
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

void drawPersons(Mat frame, double arrowScale)
{
	for (int i = 0; i < persons.size(); i++)
	{
		persons[i].draw(frame, arrowScale);
	}
}

void detectPersons(Mat frame, string window_name)
{
	Mat gray_frame;
	cvtColor(frame, gray_frame, COLOR_RGB2GRAY);
	vector<Rect> rectangles;
	hog.detectMultiScale(gray_frame, rectangles);
	//custom_cascade.detectMultiScale(gray_frame, rectangles);
	bool update = false;
	if(persons.size() > 0) update = true;
	for (int i = 0; i < rectangles.size(); i++)
	{
		if (update) persons[i].update(rectangles[i]);
		else persons.push_back(Person(rectangles[i]));
	}
}

int detectAndTrack(string video_name)
{
	setDefaultBooleans();
	VideoCapture cap(video_name);
	double frame_delay = getFrameDelay(cap);
	double frame_time = frame_delay;
	int debugging_iterator = 0;
	Mat frame;
	vector<Person> persons;

	double arrowScale;

	int arrowScaleSli = 30;

	string main_window_name = "preview";
	string param_window_name = "params";

	namedWindow(main_window_name, CV_WINDOW_AUTOSIZE);

	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
	//custom_cascade.load("Samples/cascade.xml");

	while (cap.read(frame) && turned_on)
	{
		double start_time = clock();

		if (debugging_iterator > 0 && getWindowProperty(main_window_name, 0) < 0) debug = false;
		if (debug)
		{
			namedWindow(param_window_name, CV_WINDOW_AUTOSIZE);
			createTrackbar("Arrow Scale", param_window_name, &arrowScaleSli, 90);
			debugging_iterator++;
		}
		else
		{
			destroyWindow(param_window_name);
			debugging_iterator = 0;
		}
		arrowScale = 1 + (arrowScaleSli / 10);

		resize(frame, frame, Size(frame.cols / 3, frame.rows / 3));
		if (track)
		{
			detectPersons(frame, main_window_name);
			drawPersons(frame, arrowScale);
		}

		showFPS(frame, frame_time);
		imshow(main_window_name, frame);

		double delay = frame_delay - (clock() - start_time);
		int key;
		if (delay > 1) key = waitKey(delay);
		else key = waitKey(1);
		interpretKey(key);

		frame_time = clock() - start_time;
	}
	cap.release();
	cvDestroyAllWindows();
	return 0;
}

int main()
{
	return detectAndTrack("Samples/person.mp4");
}

//CascadeClassifier person_cascade;
//CascadeClassifier upper_cascade;

//if(fullbody) person_cascade.detectMultiScale(gray_frame, rectangles, scaleFactor, minNeighbors, 0, Size(minSize, minSize));
//else upper_cascade.detectMultiScale(gray_frame, rectangles, scaleFactor, minNeighbors, 0, Size(minSize, minSize));
//if (rectangles.size() == 0) fullbody = !fullbody;

//person_cascade.load("C:/opencv/build/etc/haarcascades/haarcascade_fullbody.xml");
//upper_cascade.load("C:/opencv/build/etc/haarcascades/haarcascade_upperbody.xml");
