
#include "opencv2/opencv.hpp"	// opencv를 사용하기 위한 헤더파일
#include <string>
#include <iostream>
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
									// 이전 버전의 filesystem을 사용하기 위한 경고 무시
#include <experimental/filesystem>	// filesystem을 사용하기 위한 헤더파일
#include "SerialComm.h"	// serial 통신을 위한 헤더파일

namespace fs = std::experimental::filesystem;
int pic_num = 0;
cv::Mat img_color;
bool a = false;

cv::Mat color_object_extract(cv::Mat img);	// 물체 추출 함수 선언

int main()
{
	/*
	// Serial Communication
	std::string buffer;
	CSerialComm serialComm;

	if (!serialComm.connect("COM6"))
	{
		std::cout << "connect filed" << "\n";
		return -1;
	}
	else
		std::cout << "connect successed" << "\n";*/

	// test 폴더 내 img file들의 경로를 읽기 위한 filesystem
	std::string dir = "d:/doc/pic/test";
	std::string dir_arr[10];

	int i = 0;
	for (const auto& p : fs::directory_iterator(dir)) {	// dir 위치의 파일 확인(하위 무시)
		dir_arr[i] = p.path().string();
		i++;
	}
	
	cv::namedWindow("img_color");

	cv::Mat img_hsv;
	
	//cv::VideoCapture cap(-1);
	//if (!cap.isOpened()) {

	//	std::cout << "Can't open Camera" << std::endl;
	//	return -1;
	//}

	while (1)
	{
		/*
		// serial communication
		std::cin >> buffer;

		if (buffer == "exit") {
			break;
		}

		if (!serialComm.sendCommand(buffer.c_str()))
		{
			printf("send command failed\n");
		}
		else {
			printf("send command success\n");
		}*/

		// cap.read(img_color);
		img_color = cv::imread(dir_arr[pic_num]);

		cv::imshow("img_color", img_color);

		char k = cv::waitKey(1);
		if (k == 27)
			break;
		else if (k == char('n')) {
			pic_num++;
			if (dir_arr[pic_num] == "")
				pic_num = 0;
		}
		else if (k == 'c') {
			a = !a;
			printf("%d\n", a);
		}

		cv::Mat result;
		result = color_object_extract(img_color);
		cv::imshow("result", result);

		std::vector<uchar> buf; 
		std::vector<int> encode_params;
		encode_params.push_back(cv::IMWRITE_JPEG_QUALITY);
		encode_params.push_back(90);
		cv::imencode(".jpg", result, buf, encode_params);
	}
	/*
	// serial communication
	serialComm.disconnect();
	printf("end connect\n");*/

	cv::destroyAllWindows();
	return 0;
}

cv::Mat color_object_extract(cv::Mat img) {
	
	cv::Mat gray_img;
	cv::cvtColor(img, gray_img, cv::COLOR_BGR2GRAY);

	cv::Mat hsv_img;
	cv::cvtColor(img, hsv_img, cv::COLOR_BGR2HSV);

	cv::Mat hsv_thresh;
	cv::inRange(hsv_img, cv::Scalar(165, 140, 0, 0), cv::Scalar(180, 255, 255, 0), hsv_thresh);

	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(hsv_thresh, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
	
	int contour_num = 0;
	int temp_area = 0;
	if (!contours.empty()) {
		for (int i = 0; i < contours.size(); i++) {
			if (temp_area < cv::contourArea(contours[i])) {
				contour_num = i;
			}
		}
	}
	else {
		printf("Not detected object\n");
		return img;
	}

	cv::drawContours(img, contours, contour_num, cv::Scalar(255, 255, 0), 2);

	// Get the moment
	cv::Moments mo;
	mo = cv::moments(contours[contour_num]);

	// Get the mass center
	cv::Point2i mc;
	mc = cv::Point2i(int(mo.m10 / mo.m00), int(mo.m01 / mo.m00));

	cv::circle(img, mc, 2, cv::Scalar(0, 255, 255), 2);

	return img;
}