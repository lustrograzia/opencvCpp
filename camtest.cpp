#include "opencv2/opencv.hpp"
#include <stdio.h>
#include "SerialComm.h"

cv::Mat color_object_extract(cv::Mat img, int* area, int* center);

int main()
{
	bool extract_object = false;
	int area = 0;
	int center[2] = { 0 };

	cv::namedWindow("img_color");
	cv::Mat img_color;

	// Camera
	cv::VideoCapture cap(-1);
	if (!cap.isOpened()) {
		printf("Can't open camera\n");
		return -1;
	}

	// Serial Communication
	CString buffer;
	CSerialComm serialComm;

	if (!serialComm.connect("COM6"))
	{
		std::cout << "connect filed" << "\n";
		return -1;
	}
	else
		std::cout << "connect successed" << "\n";

	while (1) {
		cap.read(img_color);
		cv::flip(img_color, img_color, 1);
		cv::flip(img_color, img_color, 0);

		char k = cv::waitKey(1);
		if (k == 27)
			break;
		else if (k == 's') {
			extract_object = !extract_object;
		}

		if (extract_object)
		{
			img_color = color_object_extract(img_color, &area, center);
			printf("%d, %d\n", center[0], center[1]);
		}

		cv::imshow("img_color", img_color);
	}
	cv::destroyAllWindows();
	return 0;
}

cv::Mat color_object_extract(cv::Mat img, int* area, int* center) {
	cv::Mat gray_img;
	cv::cvtColor(img, gray_img, cv::COLOR_BGR2GRAY);

	cv::Mat hsv_img;
	cv::cvtColor(img, hsv_img, cv::COLOR_BGR2HSV);

	cv::Mat hsv_thresh;
	cv::inRange(hsv_img, cv::Scalar(165, 140, 0), cv::Scalar(180, 255, 255), hsv_thresh);

	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(hsv_thresh, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

	int contour_num = 0;
	int temp_area = 0;

	if (!contours.empty())
	{
		for (int i = 0; i < contours.size(); i++)
		{
			if (temp_area < cv::contourArea(contours[i]))
			{
				temp_area = cv::contourArea(contours[i]);
				contour_num = i;
			}
		}
	}
	else
	{
		printf("Not detected object\n");
		return img;
	}

	cv::drawContours(img, contours, contour_num, cv::Scalar(255, 255, 0), 2);

	cv::Moments mo;
	mo = cv::moments(contours[contour_num]);

	cv::Point2i mc;
	mc = cv::Point2i(int(mo.m10 / mo.m00), int(mo.m01 / mo.m00));
	center[0] = mc.x;
	center[1] = mc.y;
	
	cv::circle(img, mc, 2, cv::Scalar(0, 255, 255), 2);

	return img;
}
