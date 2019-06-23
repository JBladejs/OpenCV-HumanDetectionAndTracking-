#include "PersonTracker.h"
#include "Person.h"

using namespace std;
using namespace cv;

void PersonTracker::add(Rect bounding_box)
{
	persons.push_back(Person(bounding_box));
	last_seen.push_back(0);
}

void PersonTracker::update(int index, cv::Rect bounding_box)
{
	persons[index].update(bounding_box);
	last_seen[index] = 0;
}

void PersonTracker::remove(int index)
{
	persons.erase(persons.begin() + index);
	last_seen.erase(last_seen.begin() + index);
}

bool PersonTracker::vectorContains(vector<int> vect, int element)
{
	return find(vect.begin(), vect.end(), element) != vect.end();
}

PersonTracker::PersonTracker(double frames_to_removal)
{
	this->frames_to_removal = frames_to_removal;
	next_id = 0;
}

PersonTracker::~PersonTracker()
{
	persons.clear();
}

void PersonTracker::track(std::vector<Rect> rectangles)
{
	vector<Point> center_points;

	for (int i = 0; i < rectangles.size(); i++)
	{
		Rect rectangle = rectangles[i];
		center_points.push_back(Point(rectangle.x + (rectangle.width / 2), rectangle.y + (rectangle.height / 2)));
	}

	vector<int> updated_persons;
	vector<int> used_points;

	for (int i = 0; i < persons.size(); i++)
	{
		for (int j = 0; j < center_points.size(); j++)
		{
			if (!vectorContains(used_points, j) && persons[i].getBoundingBox().contains(center_points[j]))
			{
				update(i, rectangles[j]);
				used_points.push_back(j);
				updated_persons.push_back(i);
			}
		}
	}

	if (updated_persons.size() < persons.size())
	{
		for (int i = 0; i < persons.size(); i++)
		{
			if (!vectorContains(updated_persons, i))
			{
				last_seen[i]++;
				if (last_seen[i] >= frames_to_removal) remove(i);
			}
		}
	}

	/*if (used_points.size() < center_points.size())
	{
		for (int i = 0; i < center_points.size(); i++)
		{
			if (!vectorContains(used_points, i))
			{
				add(rectangles[i]);
			}
		}
	}*/
}

void PersonTracker::trackAll(vector<Rect> rectangles)
{
	for (int i = 0; i < rectangles.size(); i++)
	{
			add(rectangles[i]);
	}
}

void PersonTracker::draw(cv::Mat frame, int arrowScale)
{
	for (int i = 0; i < persons.size(); i++)
	{
		persons[i].draw(frame, arrowScale);
	}
}