#pragma once
#include "opencv2/core.hpp"

class Person
{
private:
	bool updated;
	cv::Point current_central_point;
	cv::Point previous_central_point;
	cv::Rect bounding_box;
public:
	Person(cv::Rect bounding_box);
	~Person();
	void update(cv::Rect bounding_box);
	void draw(cv::Mat image, double arrowScale);
};

