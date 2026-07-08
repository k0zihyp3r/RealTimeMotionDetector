#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video.hpp>
#include <iostream>

int main(){
    std::cout << "ongoing \npress esc to leave" << std::endl;
    std::string path = "../res/test.mp4";
    cv::VideoCapture cap(path);

    if (!cap.isOpened()) {
        std::cerr << "PATH ERROR" << path << std::endl;
        return -1;
    }

    cv::Mat img, firstFrame, imgGray, imgBlur, imgDiff;
    cv::Ptr<cv::BackgroundSubtractorMOG2> sub = cv::createBackgroundSubtractorMOG2(500, 50, true);
    
    while (true) {
        cap.read(img);

        if (img.empty()) {
            std::cout << "video has ended" << std::endl;
            break;
        }
        cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(imgGray, imgBlur, cv::Size(3,3),0);

        sub->apply(imgBlur, imgDiff);
        cv::threshold(imgDiff, imgDiff, 130, 255, cv::THRESH_BINARY);
        cv::Mat kernelNoise = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));
        cv::erode(imgDiff, imgDiff, kernelNoise);
        cv::Mat kernelCars = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(21,21));
        cv::morphologyEx(imgDiff, imgDiff, cv::MORPH_CLOSE, kernelCars);
                
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(imgDiff, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        for (const auto& contour : contours) {
            if (cv::contourArea(contour) < 700) {
                continue;
            }
            cv::Rect boundingRect = cv::boundingRect(contour);
            cv::rectangle(img, boundingRect, cv::Scalar(0, 255, 0), 2);
        }
        cv::imshow("Image", img);


        if (cv::waitKey(1) == 27) {
            std::cout << "leaving..." << std::endl;
            break;
        }
    }
    return 0;
}