#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <Windows.h>

bool update = false;
std::vector<cv::Point2i> v;
const double coef = 0.99;//可以根据手机屏幕大小进行调节

void get_screen();//获取手机截屏
void jump(double distance, double coeff=coef );//控制手机按压屏幕跳动
void my_mouse_callback(int event, int x, int y, int flags, void *param);//处理鼠标事件的函数

int main(int argc, char* argv[])
{
	get_screen();
	cv::namedWindow("Test",cv::WINDOW_NORMAL);
	cv::setMouseCallback("Test", my_mouse_callback, 0);
	cv::Mat img = cv::imread("screen.png");
	cv::imshow("Test", img);
	while (true)
	{
		if (update)
		{
			double distance = (v[0] - v[1]).dot(v[0] - v[1]);
			distance = sqrt(distance);
			jump(distance);
			Sleep(1500);
			v.clear();
			get_screen();
			img = cv::imread("screen.png");
			if (img.empty()) std::cout << "error\n";
			cv::imshow("Test", img);
			update = false;
		}		
		if (cv::waitKey(10) == 27) break;
	}
	return 0;
}

void get_screen()
{
	system("adb shell screencap -p /sdcard/screen.png");
	system("adb pull /sdcard/screen.png");
}

void jump(double distance,double coeff)
{
	std::cout << "d: " << distance << std::endl;
	int press_time = distance * coeff;
	std::string cmd = "adb shell input swipe 320 410 320 410 " + std::to_string(press_time);
	std::cout << cmd << std::endl;
	system(cmd.c_str());
}

void my_mouse_callback(int event, int x, int y, int flags, void *param)
{
	if (event == cv::EVENT_LBUTTONDOWN)
	{
		if (v.size() == 1)
		{
			v.push_back(cv::Point2i(x, y));
			update = true;
		}
		else
			v.push_back(cv::Point2i(x, y));
	}
}


