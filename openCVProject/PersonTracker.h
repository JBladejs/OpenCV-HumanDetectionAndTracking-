#pragma once
#include "Person.h"

class PersonTracker
{
private:
	std::vector<Person> persons;
	std::vector<int> last_seen;
	int next_id;
	int frames_to_removal;
	void add(cv::Rect bounding_box);
	void update(int index, cv::Rect bounding_box);
	void remove(int index);
	bool vectorContains(std::vector<int> vector, int element);
public:
	PersonTracker(double frames_to_disapear);
	~PersonTracker();
	void track(std::vector<cv::Rect>);
	void trackAll(std::vector<cv::Rect> rectangles);
	void draw(cv::Mat frame, int arrowScale);
};

