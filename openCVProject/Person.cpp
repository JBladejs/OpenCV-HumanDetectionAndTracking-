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
}
