#include "Person.h"
#include "opencv2/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

Person::Person(cv::Rect bounding_box)
{
	update(bounding_box);
}

Person::~Person() {}

void Person::update(Rect bounding_box)
{
	this->bounding_box = bounding_box;
	previous_central_point = current_central_point;
	current_central_point = Point(bounding_box.x + (bounding_box.width / 2), bounding_box.y + (bounding_box.height / 2));
}

void Person::draw(Mat image)
{
	rectangle(image, bounding_box, Scalar(255, 255, 0));
	if (previous_central_point != Point()) 
	{
		double current_x = current_central_point.x;
		double current_y = current_central_point.y;
		double prev_x = previous_central_point.x;
		double prev_y = previous_central_point.y;
		Point next_central_point = Point(current_x + ((current_x - prev_x) * 4), current_y + ((current_y - prev_y) * 4));
		arrowedLine(image, current_central_point, next_central_point, Scalar(255, 0, 0), 3, 8, 0, 0.3);
	}
}
